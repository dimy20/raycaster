#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "Render.h"
#include "Map.h"

class Player{
	public:
		Player(Render * render, glm::vec2 position, glm::vec2 direction);
		void draw(const Map& map);
		constexpr glm::vec2 direction () const { return m_direction; };
		constexpr glm::vec2 camera_plane () const { return m_camera_plane; };
		constexpr glm::vec2 position() const { return m_position; };
		constexpr void update_position(const glm::vec2 position){ m_position = position; };
	private:
		glm::vec2 m_position;
		glm::vec2 m_direction;
		glm::vec2 m_camera_plane;
		bool m_draw_dir = true;
		SDL_Rect m_rect;
		Render * m_render;
};
