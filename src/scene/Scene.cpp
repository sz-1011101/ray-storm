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

void Scene::add(geometry::ObjectPtr &object)
{
  this->objects.push_back(object);
  puts("Object added");
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