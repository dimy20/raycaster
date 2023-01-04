#include "Render.h"

void Render::init(Window& window){
	uint32_t flags = SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
	SDL_Renderer * renderer = SDL_CreateRenderer(window.window(), -1, flags);
	if(renderer == NULL){
		fprintf(stderr, "Error Renderer: %s\n", SDL_GetError());
		return;
	}

	m_renderer = std::shared_ptr<SDL_Renderer>(renderer, SDL_DestroyRenderer);
}

void Render::set_draw_color(uint8_t r, uint8_t g , uint8_t b , uint8_t a){
	SDL_SetRenderDrawColor(m_renderer.get(), r, g, b, a);
}

void Render::clear(){ SDL_RenderClear(m_renderer.get()); };

void Render::update(){ SDL_RenderPresent(m_renderer.get()); };

void Render::set_viewport(int x, int y, int w, int h){
	SDL_Rect viewport = {x, y, w, h};
	SDL_RenderSetViewport(renderer(), &viewport);
	set_draw_color(0xff, 0xff, 0xff);
	//SDL_RenderDrawRect(m_renderer.get(), &viewport);
};