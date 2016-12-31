#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <memory>

namespace ray_storm
{
  namespace utility
  {
    class Observer
    {
    public:

      virtual ~Observer() {};

      virtual void stateChanged() = 0;
      
    };

    typedef std::shared_ptr<Observer> ObserverPtr;

  }
}

#endif