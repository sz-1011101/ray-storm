#include "objects/Emitter.h"
#include "dispatchers/AbstractObjectDispatcher.h"

using namespace ray_storm::objects;

void Emitter::accept(dispatchers::AbstractObjectDispatcher *dispatcher)
{
  dispatcher->dispatch(this);
}

const float Emitter::SURFACE_POINT_OFFSET = 0.00025f;