#pragma once
#include <iostream>
#include "Render.h"
#include "Map.h"
#include "Vec.h"
#include "FrameBuffer.h"
#include <SDL2/SDL.h>

#define FOV 60.0
#define PLAYER_HEIGHT 32

class Player;

class RayCaster{
	public:
		RayCaster() = default;
		RayCaster(Render * render, Map * map, FrameBuffer * framebuffer) 
			: m_render(render), m_map(map), m_framebuffer(framebuffer ){};
		void render(const Player& player, const Map& map);
		void init(int plane_w, int plane_h);

	private:
		/*Casts ray steping through horizontal intersections until hit.*/
		float cast_horizontal_intercept(float ray_angle, const int px, const int py, const Map& map);
		/*Casts ray steping through vertical intersections until hit.*/
		float cast_vertical_intercept(float ray_angle, const int px, const int py, const Map& map);

		void draw_wall_slice(const float dist_to_slice, int col, int cell_id, int offset);

		void column_map_texture(SDL_Surface * texture, int texture_offset, int column_height, int screen_column);
	private:
		Render * m_render;
		Map * m_map;
		FrameBuffer * m_framebuffer;

		int m_plane_dim;
		int m_plane_center;
		int m_plane_width;
		int m_plane_height;
		float m_angle_step; /* FOV / NUM_COLUMS = The angle each ray spans for a colum*/
		size_t m_player_plane_dist;
		float m_viewing_angle;

		float m_hitHx;
		float m_hitHy;

		float m_hitVx;
		float m_hitVy;

		int m_hcell_id = 0;
		int m_vcell_id = 0;

		bool m_draw_rays = true;
		std::vector<std::pair<int, int>> m_points;
};
