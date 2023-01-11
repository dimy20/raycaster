#pragma once
#include <iostream>
#include "Render.h"
#include "Map.h"
#include "Vec.h"
class Player;

class RayCaster{
	public:
		RayCaster() = default;
		RayCaster(Render * render) : m_render(render) {};
		std::vector<std::pair<int, int>> cast(const Player& player, const Map& map);
	private:
		Render * m_render;
		bool ok = true;
};
