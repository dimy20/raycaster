#pragma once
#include <iostream>
#include "Render.h"
#include "Map.h"
#include "Vec.h"
class Player;

class Ray{
	public:
		Ray(Math::Vec2 start_pos, Math::Vec2 dir) : m_dir(dir), m_start_pos(start_pos) {};
		Ray(Ray && other);
		Ray& operator = (Ray && other);
		void draw(Render& render, const Map& map);
		Math::Vec2 direction() const { return m_dir; };
		Math::Vec2 start() const { return m_start_pos; };
		void set_hit(const Math::Vec2 hitpoint) { m_hitpoint = hitpoint; };
	private:
		/*Ray's direction vector*/
		Math::Vec2 m_dir;
		/* Ray's starting positions*/
		Math::Vec2 m_start_pos;

		Math::Vec2 m_hitpoint;

		bool m_draw_h_step = true;
		bool m_draw_v_step = true;

	public:
		Math::Vec2 rayX;
		Math::Vec2 rayY;

};

class RayCaster{
	public:
		RayCaster() = default;
		RayCaster(Render * render) : m_render(render) {};
		std::vector<Ray> apply(const Map& map, const Player& player);
		std::vector<std::pair<int, int>> cast(const Player& player, const Map& map);
	private:
		Render * m_render;
		bool ok = true;
};
