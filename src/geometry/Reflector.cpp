#include "geometry/Reflector.h"
#include "dispatchers/AbstractObjectDispatcher.h"

using namespace ray_storm::geometry;

void Reflector::accept(dispatchers::AbstractObjectDispatcher *dispatcher)
{
  dispatcher->dispatch(this);
}