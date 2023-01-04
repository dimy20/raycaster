#include "Window.h"

Window::Window(const std::string title, size_t w, size_t h, int x, int y){
	SDL_Window * window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN);
	if(window == NULL){
		fprintf(stderr, "Window Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	m_window = std::shared_ptr<SDL_Window>(window, SDL_DestroyWindow);
	m_title = title;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}

void Window::update(){ SDL_UpdateWindowSurface(m_window.get()); };
