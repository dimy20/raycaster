#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include "Window.h"

typedef struct{
	SDL_Rect rect;
	bool m_shown;
}Viewport_t;

class Render{
	public:
		Render() = default;
		void init(Window& window);
		void set_draw_color(uint8_t r, uint8_t g , uint8_t b , uint8_t a = SDL_ALPHA_OPAQUE);
		void clear();
		void update();
		void set_viewport(int x, int y, int w, int h);
		SDL_Renderer * renderer() const { return m_renderer.get(); };
	private:
		std::shared_ptr<SDL_Renderer> m_renderer;
};


