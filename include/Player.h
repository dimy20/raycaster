#pragma once
#include <SDL2/SDL.h>
#include "Render.h"
#include "Map.h"
#include "Vec.h"

class Player{
	public:
		Player(Render * render, Math::Vec2 position, Math::Vec2 direction);
		void draw(const Map& map);
		Math::Vec2 direction () const { return m_direction; };
		Math::Vec2 camera_plane () const { return m_camera_plane; };
		Math::Vec2 position() const { return m_position; };
		void update_position(const Math::Vec2 position){ m_position = position; };
	private:
		Math::Vec2 m_position;
		Math::Vec2 m_direction;
		Math::Vec2 m_camera_plane;
		bool m_draw_dir = true;
		SDL_Rect m_rect;
		Render * m_render;
};
