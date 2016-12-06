#include "objects/DistanceFieldFactory.h"
#include "geometry/DistSphere.hpp"
#include "objects/DefaultDistanceField.hpp"

using namespace ray_storm::objects;

ReflectorPtr DistanceFieldFactory::createSphere(
  const glm::vec3 &position, float radius, const materials::MaterialPtr &material
)
{
  return ReflectorPtr(
    new DefaultDistanceField(geometry::MarchablePtr(new geometry::DistSphere(position, radius)), material)
  );
} 