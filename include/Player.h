#pragma once
#include <SDL2/SDL.h>
#include "Render.h"
#include "Map.h"
#include "Vec.h"

#define MOV_SPEED 2.0f
#define ROTATION_SPEED 2.0f

class Engine;

class Player{
	public:
		Player() = default;
		Player(Engine * engine, Render * render, Map * map, Math::Vec2 position, Math::Vec2 direction);
		void draw();
		Math::Vec2 direction () const { return m_direction; };
		Math::Vec2 position() const { return m_position; };
		void set_direction(Math::Vec2 new_dir) { m_direction = new_dir; };
		void update();
	private:
		Math::Vec2 m_position;
		Math::Vec2 m_direction;
		bool m_draw_dir = true;
		SDL_Rect m_rect;
		Render * m_render;
		Map * m_map;
		Engine * m_engine;
};
