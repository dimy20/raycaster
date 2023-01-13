#pragma once
#include <iostream>
#include "Render.h"
#include "Map.h"
#include "Vec.h"
#include <SDL2/SDL.h>

#define FOV 60.0
#define PLAYER_HEIGHT 32

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
		void init(int plane_w, int plane_h);
		void draw_wall_slice(const float dist_to_slice, int col);
	private:
		/*Casts ray steping through horizontal intersections until hit.*/
		HitPoint cast_horizontal_intercept(float ray_angle, const int px, const int py, const Map& map);
		/*Casts ray steping through vertical intersections until hit.*/
		HitPoint cast_vertical_intercept(float ray_angle, const int px, const int py, const Map& map);
	private:
		Render * m_render;
		int m_plane_dim;
		int m_plane_center;
		int m_plane_width;
		int m_plane_height;
		float m_angle_step; /* FOV / NUM_COLUMS = The angle each ray spans for a colum*/
		size_t m_player_plane_dist;
};
