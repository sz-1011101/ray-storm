#include "scene/Scene.h"
#include "datastructures/List.hpp"

using namespace ray_storm::scene;

Scene::Scene() : dataStruct(new datastructures::List<geometry::Object>())
{

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