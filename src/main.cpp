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
#include "Vec.h"

#define VIEWPORT_W Map::CELL_SIZE * 8
#define VIEWPORT_H Map::CELL_SIZE * 8

std::pair<int, int> world2screen(int x, int y){
	const size_t MW = 8;
	const size_t MH = MW;
	int screen_x = x * (VIEWPORT_W / (MW * Map::CELL_SIZE));
	int screen_y = y * (VIEWPORT_H / (MH * Map::CELL_SIZE));
	return {screen_x, screen_y};
}

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
	init();
	std::string title("ray caster");
	Window window(title, (size_t)1024, (size_t)728, 0, 0);

	Render render;
	render.init(window);

	window.update();

	bool running = true;
	SDL_Event event;

	Map map(&render);
	Player player(&render, &map, Math::Vec2(96.0f, 224.0f), Math::Vec2(-1.0f, 0.0f));

	render.set_viewport(0, 0, VIEWPORT_W, VIEWPORT_H);

	RayCaster raycaster(&render);
	while(running){
		SDL_PumpEvents(); // updates event queue

		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			};

			if(event.type == SDL_KEYDOWN){
				player.keypressed(event.key.keysym.sym);
			}
		};

		render.set_draw_color(0, 0, 0);
		render.clear();

		map.draw();

		player.draw();
		auto points = raycaster.cast(player, map);
		for(auto [x, y] : points){
			(void)x;
			(void)y;
			render.set_draw_color(0xff, 0xff, 0xff);

			auto[x1, y1] = world2screen((int)player.position().x(), (int)player.position().y());
			auto[x2, y2] = world2screen(x, y);

			SDL_RenderDrawLine(render.renderer(), x1, y1, x2, y2);

		}
		render.update();
	}
	return 0;
};
