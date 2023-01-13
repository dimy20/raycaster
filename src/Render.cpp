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

void Render::use_viewport(const std::string& id){
	assert(m_viewports.find(id) != m_viewports.end());
	SDL_Rect viewport = m_viewports[id];
	SDL_RenderSetViewport(renderer(), &viewport);
};

void Render::set_viewport(SDL_Rect viewport, const std::string& id){ m_viewports[id] = viewport; }

SDL_Rect Render::viewport() const {
	SDL_Rect rect;
	SDL_RenderGetViewport(m_renderer.get(), &rect);
	return rect;
}

void Render::prepare_scene(){
	set_draw_color(0, 0, 0);
	SDL_RenderClear(m_renderer.get());
}
void Render::present_scene(){ SDL_RenderPresent(m_renderer.get()); }
