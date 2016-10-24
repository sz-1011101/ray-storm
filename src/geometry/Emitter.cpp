#include "geometry/Emitter.h"
#include "dispatchers/AbstractObjectDispatcher.h"

using namespace ray_storm::geometry;

void Emitter::accept(dispatchers::AbstractObjectDispatcher *dispatcher)
{
  dispatcher->dispatch(this);
}