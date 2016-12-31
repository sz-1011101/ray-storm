#include "utility/Observable.h"

using namespace ray_storm::utility;

void Observable::registerObserver(const ObserverPtr &observer)
{
  this->observers.push_back(observer);
}

void Observable::stateChanged()
{
  for (const ObserverPtr &o : this->observers)
  {
    o->stateChanged();
  }
}