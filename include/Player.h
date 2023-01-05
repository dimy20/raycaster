#pragma once
#include <SDL2/SDL.h>
#include "Render.h"
#include "Map.h"
#include "Vec.h"

#define MOV_SPEED 0.1
#define ROTATION_SPEED 2.0f

class Player{
	public:
		Player(Render * render, Map * map, Math::Vec2 position, Math::Vec2 direction);
		void draw();
		Math::Vec2 direction () const { return m_direction; };
		Math::Vec2 camera_plane () const { return m_camera_plane; };
		Math::Vec2 position() const { return m_position; };
		void keypressed(const SDL_Keycode& key);
	private:
		Math::Vec2 m_position;
		Math::Vec2 m_direction;
		Math::Vec2 m_camera_plane;
		bool m_draw_dir = true;
		SDL_Rect m_rect;
		Render * m_render;
		Map * m_map;
};
