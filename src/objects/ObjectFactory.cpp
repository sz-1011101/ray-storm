#include "objects/ObjectFactory.h"
#include "geometry/DistSphere.hpp"
#include "objects/DefaultDistanceField.hpp"
#include "objects/Sphere.hpp"
#include "objects/Box.hpp"
#include "geometry/MarchableUnion.hpp"

using namespace ray_storm::objects;

ReflectorPtr ObjectFactory::createDistSphere(
  const glm::vec3 &position, float radius, const materials::MaterialPtr &material
)
{
  return ReflectorPtr(
    new DefaultDistanceField(geometry::MarchablePtr(new geometry::DistSphere(position, radius)), material)
  );
}

ReflectorPtr ObjectFactory::createSphereUnion(
  const glm::vec3 &position, const materials::MaterialPtr &material
)
{
  geometry::MarchablePtr sphere1(new geometry::DistSphere(glm::vec3(-0.5f, 0.0f, 0.0f), 1.5f));
  geometry::MarchablePtr sphere2(new geometry::DistSphere(glm::vec3(0.5f, 0.0f, 0.0f), 1.5f));
  geometry::MarchableUnionPtr u(new geometry::MarchableUnion(position));
  u->add(sphere1);
  u->add(sphere2);
  return ReflectorPtr(
    new DefaultDistanceField(u, material)
  );
}

ReflectorPtr ObjectFactory::createBox(
  const glm::vec3 &origin, const glm::vec3 &extends, const materials::MaterialPtr &material
)
{
  return objects::ReflectorPtr(new objects::Box(origin, extends, material));
}

EmitterPtr ObjectFactory::createSphere(
  const glm::vec3 &position,
  float radius,
  const materials::MaterialPtr &material,
  const glm::vec3 &emittance
)
{
  return objects::EmitterPtr(new objects::Sphere(position, radius, material, emittance));
}

EmitterPtr ObjectFactory::createRectangle(
  const Rectangle::RectParams &rp,
  const materials::MaterialPtr &material,
  const glm::vec3 &emittance
)
{
  return objects::EmitterPtr(new objects::Rectangle(rp, material, emittance));
}