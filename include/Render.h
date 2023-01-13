#pragma once

#include <memory>
#include <cassert>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "Window.h"

class Render{
	public:
		Render() = default;
		void init(Window& window);
		void set_draw_color(uint8_t r, uint8_t g , uint8_t b , uint8_t a = SDL_ALPHA_OPAQUE);
		void clear();
		void update();
		void use_viewport(const std::string& id);
		void set_viewport(SDL_Rect viewport, const std::string& id);
		SDL_Rect viewport() const;
		SDL_Renderer * renderer() const { return m_renderer.get(); };
	private:
		std::shared_ptr<SDL_Renderer> m_renderer;
		std::unordered_map<std::string, SDL_Rect> m_viewports;
};


