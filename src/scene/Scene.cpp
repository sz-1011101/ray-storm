#include "scene/Scene.h"
#include "datastructures/List.hpp"
#include "dispatchers/EmittanceDispatcher.hpp"

using namespace ray_storm::scene;

const float SKY_RAY_OFFSET = 100.0f; // TODO replace by scaling with scene bbox size

Scene::Scene() : dataStruct(new datastructures::List<objects::Object>()), boundingSphere(glm::vec3(0.0f), 0.0f)
{
}

bool Scene::intersect(const geometry::Ray &ray, geometry::Intersection<objects::Object> &intersection) const
{
  return this->dataStruct->intersect(ray, intersection);
}

void Scene::sampleLuminaire(const glm::vec3 &x, const glm::vec3 &n, random::RandomizationHelper &randHelper, LuminaireSample &light)
{
  const int lCnt = static_cast<int>(this->lights.size());
  int objIndex;
  bool objDrawn = false;

  if (lCnt == 0 && this->sky == nullptr)
  {
    return;
  }

  // draw from objects and sky...
  if (this->sky != nullptr)
  {
    objIndex = randHelper.drawUniformRandom(0, lCnt + 1);
    objDrawn = objIndex == lCnt ? false : true;
  }
  else //... or only objects
  {
    objIndex = randHelper.drawUniformRandom(0, lCnt);
    objDrawn = true;
  }

  if (objDrawn) // object luminaire
  {
    objects::Emitter *luminaire = this->lights.at(objIndex).get();
    const glm::vec3 pos = luminaire->drawRandomSurfacePoint(randHelper);
    light.direction = glm::normalize(pos - x);

    geometry::Ray shadowRay(x, light.direction);

    geometry::Intersection<objects::Object> intersectL;
    if (glm::dot(light.direction, n) > 0.0f // hemisphere
      && this->intersect(shadowRay, intersectL) // should happen except at edges
      && intersectL.intersected == luminaire // we hit the drawn object
      && glm::distance(intersectL.intersection.position, pos) < 0.001f // intersection is at sample pnt
    )
    {
      light.shadowed = false;
      light.emittance = luminaire->getEmittance();
      light.PDF = this->getLuminairePDF(luminaire, shadowRay, pos, intersectL.intersection.normal);
    }
  }
  else // sky light sampling
  {
    light.direction = randHelper.drawUniformRandomHemisphereDirection(n);
    geometry::Ray shadowRay(x, light.direction);

    geometry::Intersection<objects::Object> intersectL;
    if (!this->intersect(shadowRay, intersectL)) // skylight hit?
    {
      light.shadowed = false;
      light.emittance = this->sampleSky(shadowRay.direction);
      light.PDF = this->getSkyPDF();
    }
  }
}

bool Scene::sampleLuminaireRay(random::RandomizationHelper &randHelper, LuminaireRay &lumRay)
{
  const int lCnt = static_cast<int>(this->lights.size());

  if (lCnt == 0 && this->sky == nullptr)
  {
    return false;
  }

  int objIndex;
  bool objDrawn = false;
  float selectionPDF = 0.0f;

  if (this->sky != nullptr)
  {
    objIndex = randHelper.drawUniformRandom(0, lCnt + 1);
    objDrawn = objIndex == lCnt ? false : true;
    selectionPDF = 1.0f/(this->lights.size() + 1);
  }
  else
  {
    objIndex = randHelper.drawUniformRandom(0, lCnt);
    objDrawn = true;
    selectionPDF = 1.0f/(this->lights.size());
  }


  if (objDrawn)
  {
    objects::Emitter *luminaire = this->lights.at(objIndex).get();
    luminaire->drawRandomRay(randHelper, lumRay.randRay);
    lumRay.randRay.PDF *= selectionPDF;
    lumRay.emittance = luminaire->getEmittance();
  }
  else
  {
    // starting pos
    const glm::vec3 dir = randHelper.drawUniformRandomSphereDirection();
    lumRay.randRay.ray.origin = dir*SKY_RAY_OFFSET;
    lumRay.randRay.ray.direction = randHelper.drawCosineWeightedRandomHemisphereDirection(-dir, 1.0f);
    // pdf = cos-direction towards scene*selection of scene*sphere area
    lumRay.randRay.PDF = randHelper.cosineRandomHemispherePDF(glm::dot(-dir, lumRay.randRay.ray.direction), 1.0f)*
      selectionPDF/(4.0f*static_cast<float>(M_PI)*SKY_RAY_OFFSET*SKY_RAY_OFFSET);
    lumRay.emittance = this->sampleSky(dir);
    lumRay.directional = true;
  }

  return true;
}

float Scene::getLuminairePDF(objects::Object *object, const geometry::Ray &ray, const glm::vec3 &x, const glm::vec3 &n)
{
  const int lCnt = this->sky == nullptr ? static_cast<int>(this->lights.size()) : static_cast<int>(this->lights.size()) + 1;
  
  dispatchers::EmittanceDispatcher ed;
  object->accept(&ed);

  if (object != nullptr && ed.isEmitting())
  {
    // PDF from http://cg.informatik.uni-freiburg.de/course_notes/graphics2_09_pathTracing.pdf
    return (ed.getPDF()/lCnt)*
        // convert to directional pdf (see "Optimally Combining Sampling Techniques for Monte Carlo Rendering")
        (std::pow(glm::length(ray.origin - x), 2.0f)
        /dot(-ray.direction, n));
  }
  return 0.0f;
}

void Scene::add(const objects::ReflectorPtr &reflector)
{
  if (reflector == nullptr)
  {
    return;
  }
  this->objects.push_back(reflector);
  this->bbox.cover(reflector->getBBox());
}

void Scene::add(const objects::EmitterPtr &emitter)
{
  if (emitter == nullptr)
  {
    return;
  }
  this->objects.push_back(emitter);
  this->bbox.cover(emitter->getBBox());
  dispatchers::EmittanceDispatcher ed;
  emitter->accept(&ed);
  if (ed.isEmitting()) {
    this->lights.push_back(emitter);
  }
}

void Scene::finalize()
{
  // build the thing
  for (objects::ObjectPtr &o : this->objects)
  {
    this->dataStruct->add(o.get());
  }

  this->dataStruct->initialize();
  const glm::vec3 &bboxOrigin = this->bbox.getOrigin();
  const glm::vec3 &bboxUpperBounds = this->bbox.getUpperBounds();

  // compute scene bounding sphere
  const glm::vec3 bboxCenter = this->bbox.computeCenter();
  const float bSphereRad = glm::distance(bboxOrigin, bboxCenter);
  this->boundingSphere = geometry::SpherePrimitive(bboxCenter, bSphereRad);

  printf("Scene finalized, bbox: (%f, %f, %f) to (%f, %f, %f)\n", bboxOrigin.x, bboxOrigin.y, bboxOrigin.z, 
    bboxUpperBounds.x, bboxUpperBounds.y, bboxUpperBounds.z);
}

void Scene::setSky(const AbstractSkyPtr &sky)
{
  this->sky = sky;
}

glm::vec3 Scene::sampleSky(const glm::vec3 &direction)
{
  if (this->sky == nullptr)
  {
    return glm::vec3(0.0f);
  }

  return this->sky->sample(direction);
}

float Scene::getSkyPDF()
{
  if (this->sky == nullptr)
  {
    return 0.0f;
  }
  return random::RandomizationHelper::uniformRandomHemispherePDF()/(static_cast<int>(this->lights.size()) + 1);
}

bool Scene::visible(const glm::vec3 &origin, const glm::vec3 &target)
{
  geometry::Ray vRay(origin, glm::normalize(target - origin));
  geometry::Intersection<objects::Object> intersect;
  
  if (!this->intersect(vRay, intersect))
  {
    return true;
  }

  if (glm::distance(intersect.intersection.position, target) < 0.001f)
  {
    return true;
  }

  return glm::distance(origin, intersect.intersection.position) >= glm::distance(origin, target);

}

bool Scene::visible(const glm::vec3 &origin, const glm::vec3 &target, const glm::vec3 &targetNormal)
{
  geometry::Ray vRay(origin, glm::normalize(target - origin));
  geometry::Intersection<objects::Object> intersect;
  
  if (!this->intersect(vRay, intersect))
  {
    return true;
  }

  if (glm::distance(intersect.intersection.position, target) < 0.001f)
  {
    return true;
  }

  return glm::distance(origin, intersect.intersection.position) >= glm::distance(origin, target) &&
    glm::dot(intersect.intersection.normal, targetNormal) > 0.0f;
}