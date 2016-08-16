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

bool Scene::drawRandomEmittingObject(random::RandomizationHelper &randHelper, LightSource &light)
{
  const size_t lCnt = this->lights.size();
  if (lCnt == 0)
  {
    return false;
  }

  const int objIndex = randHelper.drawUniformRandom(0, static_cast<int>(lCnt));
  light.object = this->lights.at(objIndex).get();
  light.emittance = light.object->getMaterial()->getEmittance();
  light.lightPos = light.object->drawRandomSurfacePoint(randHelper);
  return true;
}

void Scene::add(geometry::ObjectPtr &object)
{
  this->objects.push_back(object);
  puts("Object added");
  // if the objects' material is emitting in some channel it is a light source
  if (object->getMaterial() != nullptr &&
    glm::any(glm::greaterThan(object->getMaterial()->getEmittance(), glm::vec3(0.0f))))
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