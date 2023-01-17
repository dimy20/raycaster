#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "RayCaster.h"
#include "FrameBuffer.h"

#define PROJ_PLANE_W 1024
#define PROJ_PLANE_H 728
#define VIEWPORT_W Map::CELL_SIZE * 2
#define VIEWPORT_H Map::CELL_SIZE * 2

class Engine{
	public:
		Engine() = default;
		void init();
		void run();
		const uint8_t * keyboard_input() const { return m_keyboard_state; };
	private:
		void do_input();
	private:
		Render m_renderer;
		Window m_window;
		Map m_map;
		Player m_player;
		RayCaster m_caster;
		bool m_running = true;
		const uint8_t * m_keyboard_state = NULL;
};
