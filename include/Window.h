#pragma once

#include <SDL2/SDL.h>
#include <cstdio>
#include <memory>

class Window{
	public:
		Window() = default;
		Window(const std::string title, size_t w, size_t h, int x, int y);
		SDL_Window * window() const { return m_window.get(); };
		void update();
	private:
		size_t m_w = 0;
		size_t m_h = 0;
		int m_x = 0;
		int m_y = 0;
		std::string m_title;
		std::shared_ptr<SDL_Window> m_window;
};
