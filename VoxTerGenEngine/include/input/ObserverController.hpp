#ifndef OBSERVER_CONTROLLER_HPP
#define OBSERVER_CONTROLLER_HPP

#include "world/Observer.hpp"

class ObserverController
{
private:
	Observer& observer_;

public:
	ObserverController(Observer& observer);
};

#endif

