#include "scene/Scene.h"
#include "datastructures/List.hpp"

using namespace ray_storm::scene;

const glm::vec3 DEFAULT_SKY = glm::vec3(0.0, 0.0, 0.0);

Scene::Scene() : dataStruct(new datastructures::List<geometry::Object>())
{
  this->sky = DEFAULT_SKY;
}

bool Scene::intersect(const geometry::Ray &ray, geometry::Intersection<geometry::Object> &intersection) const
{
  return this->dataStruct->intersect(ray, intersection);
}

bool Scene::drawLuminareSample(random::RandomizationHelper &randHelper, LuminaireSample &light)
{
  const size_t lCnt = this->lights.size();
  if (lCnt == 0)
  {
    return false;
  }

  const int objIndex = randHelper.drawUniformRandom(0, static_cast<int>(lCnt));
  light.object = this->lights.at(objIndex).get();
  light.lightPos = light.object->drawRandomSurfacePoint(randHelper);
  light.PDF = (1.0f/lCnt)*light.object->getPDF(); // no idea what im doing ;_;
  return true;
}

bool Scene::getLuminarePDF(geometry::ObjectPtr &object, float &pdf)
{
  // TODO implement
  return false;
}

void Scene::add(geometry::ObjectPtr &object)
{
  this->objects.push_back(object);
  puts("Object added");
  // if the objects' material is emitting in some channel it is a light source
  if (object->getMaterial() != nullptr &&
    glm::any(glm::greaterThan(object->getEmittance(), glm::vec3(0.0f))))
  {
    this->lights.push_back(object);
    puts("Light added");
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

void Scene::setSky(const glm::vec3 &sky)
{
  this->sky = sky;
}

const glm::vec3 &Scene::getSky()
{
  return sky;
}