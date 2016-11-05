#include "objects/Reflector.h"
#include "dispatchers/AbstractObjectDispatcher.h"

using namespace ray_storm::objects;

void Reflector::accept(dispatchers::AbstractObjectDispatcher *dispatcher)
{
  dispatcher->dispatch(this);
}