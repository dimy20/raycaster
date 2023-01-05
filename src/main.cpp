#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <vector>
#include <cmath>

#include "RayCaster.h"
#include "Window.h"
#include "Render.h"
#include "Map.h"
#include "Player.h"
#include "utils.h"
#include "Vec.h"

#define VIEWPORT_W 1024
#define VIEWPORT_H 728
#define SPEED 0.1

/*
struct Point{
	Point(Math::Vec2 pos, Render * render) : m_position(pos), m_render(render) {};
	void draw(const size_t vp_w, const size_t vp_h);
	Math::Vec2 m_position;
	Render * m_render;
};

void Point::draw(const size_t vp_w, const size_t vp_h){
	const size_t MAP_W = 24;
	const size_t MAP_H = 24;

	int x, y;
	float map_2_screenX = (float)vp_w / (float)MAP_W;
	float map_2_screenY = (float)vp_h / (float)MAP_H;

	x = (int)(m_position.x * map_2_screenX); // mapping the map coordinate to viewport space.(proportion)
	y = (int)(m_position.y * map_2_screenY); // mapping the map coordinate to viewport space.(proportion)

	SDL_Rect rect;

	size_t rect_w = 5;
	size_t rect_h = 5;
	rect = {x - (int)(rect_w / 2), y - (int)(rect_h / 2), (int)rect_w, (int)rect_h};
	SDL_RenderFillRect(m_render->renderer(), &rect);
}

*/



void set_pixel(SDL_Surface * surface, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b){
	SDL_LockSurface(surface);
	uint8_t * pixels = (uint8_t *)surface->pixels;
	(void)pixels;

	size_t pixel_size = surface->format->BytesPerPixel;
	size_t offset = ((y * surface->w) + x) * pixel_size;

	int values[4] = {r, g, b, 0xff};
	for(size_t i = 0; i < pixel_size; i++){
		pixels[offset + i] = values[i];
	}

	SDL_UnlockSurface(surface);
};

void init(){
	int err = SDL_Init(SDL_INIT_VIDEO);
	if(err < 0){
		std::cout << "failed to initalize sdl" << std::endl;
		exit(EXIT_FAILURE);
	}

	err = IMG_Init(IMG_INIT_PNG);
	if(!(err & IMG_INIT_PNG)){
		std::cerr << "Error: " << IMG_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	if(TTF_Init() == -1){
		std::cerr << "Error: " << TTF_GetError() << "\n";
		exit(EXIT_FAILURE);
	}
};

int main(){

	bool run = true;
	init();
	std::string title("ray caster");
	Window window(title, (size_t)1024, (size_t)728, 0, 0);

	Render render;
	render.init(window);

	window.update();

	bool running = true;
	SDL_Event event;

	Map map(&render);
	Player player(&render, Math::Vec2(2.3f, 2.3f), Math::Vec2(1.0f, 0));

	render.set_viewport(0, 0, VIEWPORT_W, VIEWPORT_H);

	RayCaster raycaster;
	std::vector<Ray> casted_rays;
	while(running){
		SDL_PumpEvents(); // updates event queue

		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			};

			if(event.type == SDL_KEYDOWN){
				auto pos = player.position();
				auto& [posx, posy] = pos.xy();
				switch(event.key.keysym.sym){
					case SDLK_w:
						posy += -SPEED;
						player.update_position(pos);
						break;
					case SDLK_s:
						posy += SPEED;
						player.update_position(pos);
						break;
					case SDLK_d:
						posx += SPEED;
						player.update_position(pos);
						break;
					case SDLK_a:
						posx -= SPEED;
						player.update_position(pos);
						break;
				}
			}

		};

		std::pair<double, double> i_x_y;

		if(run){
			casted_rays = raycaster.apply(map, player);
			run = false;
		}



		render.set_draw_color(0, 0, 0);
		render.clear();


		map.draw();
		player.draw(map);
		//render.set_draw_color(204, 204, 0);

		int ok = 1; // draw half
		for(auto& r : casted_rays){
			if(ok){
				r.draw(render, map);
			}
			ok ^= 1;
		}

		render.update();

	}
	return 0;
};
