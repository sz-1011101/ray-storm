#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include <vector>

#include "utility/Observer.h"

namespace ray_storm
{
  namespace utility
  {
    class Observable
    {
    public:

      virtual ~Observable() {};

      void registerObserver(const ObserverPtr &observer);

    protected:

      void stateChanged();

    private:

      std::vector<ObserverPtr> observers;
      
    };
  }
}

#endif