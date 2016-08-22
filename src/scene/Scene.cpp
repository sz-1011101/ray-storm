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

bool Scene::drawLuminareSample(random::RandomizationHelper &randHelper, LuminaireSample &light)
{
  const std::size_t lCnt = this->lights.size();
  if (lCnt == 0)
  {
    return false;
  }

  const int objIndex = randHelper.drawUniformRandom(0, static_cast<int>(lCnt));
  light.object = this->lights.at(objIndex).get();
  light.position = light.object->drawRandomSurfacePoint(randHelper);
  light.PDF = this->getLuminarePDF(light.object);
  return true;
}

float Scene::getLuminarePDF(geometry::Object *object)
{
  if (object != nullptr && object->isEmitting())
  {
    // PDF from http://cg.informatik.uni-freiburg.de/course_notes/graphics2_09_pathTracing.pdf
    return object->getPDF()/this->lights.size();
  }
  return 0.0f;
}

void Scene::add(geometry::ObjectPtr &object)
{
  this->objects.push_back(object);
  puts("Object added");
  // if the objects' material is emitting in some channel it is a light source
  if (object->isEmitting())
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