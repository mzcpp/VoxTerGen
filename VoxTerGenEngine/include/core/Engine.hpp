#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "core/Settings.hpp"
#include "world/World.hpp"

class Engine
{
private:
	Settings settings_;
	World world_;

public:
	Engine();
};

#endif

