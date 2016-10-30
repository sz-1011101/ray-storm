#include "scene/Scene.h"
#include "datastructures/List.hpp"
#include "dispatchers/EmittanceDispatcher.hpp"

using namespace ray_storm::scene;

Scene::Scene() : dataStruct(new datastructures::List<geometry::Object>())
{
}

bool Scene::intersect(const geometry::Ray &ray, geometry::Intersection<geometry::Object> &intersection) const
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
    geometry::Emitter *luminaire = this->lights.at(objIndex).get();
    const glm::vec3 pos = luminaire->drawRandomSurfacePoint(randHelper);
    light.direction = glm::normalize(pos - x);

    geometry::Ray shadowRay(x, light.direction);

    geometry::Intersection<geometry::Object> intersectL;
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

    geometry::Intersection<geometry::Object> intersectL;
    if (!this->intersect(shadowRay, intersectL)) // skylight hit?
    {
      light.shadowed = false;
      light.emittance = this->sampleSky(shadowRay.direction);
      light.PDF = this->getSkyPDF();
    }
  }
}

float Scene::getLuminairePDF(geometry::Object *object, const geometry::Ray &ray, const glm::vec3 &x, const glm::vec3 &n)
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

void Scene::add(const geometry::ReflectorPtr &reflector)
{
  this->objects.push_back(reflector);
}

void Scene::add(const geometry::EmitterPtr &emitter)
{
  this->objects.push_back(emitter);
  dispatchers::EmittanceDispatcher ed;
  emitter->accept(&ed);
  if (ed.isEmitting()) {
    this->lights.push_back(emitter);
  }
}

void Scene::finalize()
{
  // build the thing
  for (geometry::ObjectPtr &o : this->objects)
  {
    this->dataStruct->add(o.get());
  }

  this->dataStruct->initialize();
  puts("Scene finalized");
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