#pragma once
#include <iostream>
#include "Render.h"
#include "Map.h"
#include "Vec.h"
class Player;

struct HitPoint{
	float x; 
	float y;
	float distance;
};

class RayCaster{
	public:
		RayCaster() = default;
		RayCaster(Render * render) : m_render(render) {};
		std::vector<std::pair<int, int>> cast(const Player& player, const Map& map);
	private:
		/*Casts ray steping through horizontal intersections until hit.*/
		HitPoint cast_horizontal_intercept(float ray_angle, int px, int py, const Map& map);
		/*Casts ray steping through vertical intersections until hit.*/
		HitPoint cast_vertical_intercept(float ray_angle, int px, int py, const Map& map);
	private:
		Render * m_render;
};
