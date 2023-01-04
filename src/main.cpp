#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <vector>
#include <cmath>

#include "Texture.h"
#include "Timer.h"
#include "Window.h"
#include "Render.h"
#include "Map.h"
#include "Player.h"
#include "utils.h"


#define VIEWPORT_W 1024
#define VIEWPORT_H 728

void print_vec2(const glm::vec2& vec){
	std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}

void draw_line(Render& render, glm::vec2 point1, glm::vec2 point2){
	//std::cout << "rayDir: ";
	//print_vec2(point2);
	auto[x1_vp, y1_vp] = map2_screen(24, 24, VIEWPORT_W, VIEWPORT_H, point1);
	auto[x2_vp, y2_vp] = map2_screen(24, 24, VIEWPORT_W, VIEWPORT_H, point2);
	render.set_draw_color(0xff, 0xff, 0xff);
	SDL_RenderDrawLine(render.renderer(), x1_vp, y1_vp, x2_vp, y2_vp);
};

struct Point{
	Point(glm::vec2 pos, Render * render) : m_position(pos), m_render(render) {};
	void draw(const size_t vp_w, const size_t vp_h);
	glm::vec2 m_position;
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

std::pair<double, double> rc_test(const Render& render, const Map& map, const Player& player, glm::vec2& ray_dir){


	auto camera_plane = player.camera_plane();
	auto player_direction = player.direction();
	auto player_position = player.position();
	// horizontal interceptions
	// getting alpha

	double intersect_x;
	double intersect_y;

	double dx, dy;
	for(size_t x = 0; x < map.width(); x++){
		size_t TILE_I, TILE_J;
		TILE_I = (size_t)player_position.x;
		TILE_J = (size_t)player_position.y;
		//std::cout << "Tile i: " << TILE_I << "\n";
		//std::cout << "Tile j: " << TILE_J << "\n";
		//double alpha = atan2
		/*Map columns to camera space*/
		float camera_x = 2 *(x / (float)map.width())- 1;
		ray_dir = player_direction + (camera_plane * camera_x);
		double alpha = atan2(ray_dir.y, ray_dir.x);

		//double alpha = (tmp/ M_PI) * 180.0; // direction of the ray in angle form

		// setup the offsets



		/* Setup of the dy offset for the initial computation of intersection
		 * with the first horizontal line:
		 * */
		int step_dir_x, step_dir_y;
		if(ray_dir.y < 0){ // Remember that origin is at the top left corner, so negative direction means up.
			dy = player_position.y - TILE_J;
			step_dir_y = 1;
		}else{
			dy = (TILE_J + 1) - player_position.y;
			step_dir_y = -1;
		}

		if(ray_dir.x > 0){
			dx = player_position.x - TILE_I;
			step_dir_x = 1;
		}else{
			dx = (TILE_I + 1) - player_position.x;
			step_dir_x = -1;
		}


		 
		/*Step needed to take in the x direction to hit the first
		 * horizontal line on the grid.*/
		//intersect_x = player_position.x + dx + -dy / tan(alpha);
		intersect_x = TILE_I + dx + (-dy) / tan(alpha);
		intersect_y = player_position.y + dy + (dx * tan(alpha));

		//std::cout << "(" << intersect_x << ", " << dy << ")\n";

		//print_vec2(ray_dir);
		if(x == 9){
			std::cout << "Tan : " << (-1 * dy) / tan(alpha) << "\n";
			std::cout << "raydir: ";
			print_vec2(ray_dir);
			break;
		}


		(void)intersect_y;
		(void)step_dir_x;
		(void)step_dir_y;

		//std::cout << alpha << std::endl;

	};
	//SDL_RenderDrawLine(render.renderer(), (int)player_position.x, (int)player_position.y, (int)ray_dir.x + 10, (int)ray_dir.y + 10);

	return {intersect_x, -dy};
};

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
	//SDL_Window * window = SDL_CreateWindow("learning", 2000, 0, 1024, 728, SDL_WINDOW_SHOWN);

	Render render;
	render.init(window);

	Texture sheet(&render);

	window.update();

	bool running = true;
	SDL_Event event;
	int frame = 0;
	(void)frame;
	//(void)start_time;
	Timer timer;
	timer.start();
	int ok = 1;

	Map map(&render);
	Player player(&render, glm::vec2(2.3f, 2.3f), glm::vec2(1.0f, 0));

	render.set_viewport(0, 0, VIEWPORT_W, VIEWPORT_H);

	Point test_point(glm::vec2(10.0f, 1.0f), &render);
	while(running){
		SDL_PumpEvents(); // updates event queue

		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			};

			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE){
				timer.stop();
				timer.start();
			}

			if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p){
				if(ok){
					timer.pause();
				}else{
					timer.unpause();
				}
				ok ^= 1;
			}

			if(event.type == SDL_KEYDOWN){
				auto pos = player.position();
				switch(event.key.keysym.sym){
					case SDLK_w:
						pos.y += -1.0f;
						player.update_position(pos);
						break;
					case SDLK_s:
						pos.y += 1.0f;
						player.update_position(pos);
						break;
					case SDLK_d:
						pos.x += 1.0f;
						player.update_position(pos);
						break;
					case SDLK_a:
						pos.x -= 1.0f;
						player.update_position(pos);
						break;
				}
			}

		};

		std::pair<double, double> i_x_y;

		glm::vec2 ray_dir;
		if(run){
			i_x_y = rc_test(render, map, player, ray_dir);
			run = false;

			glm::vec2 tmp(i_x_y.first, i_x_y.second);
			print_vec2(tmp);

			std::cout << "Player position: ";
			print_vec2(player.position());

			test_point.m_position.x = i_x_y.first;
			test_point.m_position.y = player.position().y + i_x_y.second;

			std::cout << "point : ";
			print_vec2(test_point.m_position);

			// TODO: draw ray direction!
		}

		//(void)i_x_y;

		//std::string time = "Time: " + std::to_string(timer.get_ticks());
		//text.load_from_rendered_text(time, text_color);

		//render.set_draw_color(0, 0, 0);
		//render.clear();


		map.draw(VIEWPORT_W, VIEWPORT_H);
		player.draw(VIEWPORT_W, VIEWPORT_H);
		render.set_draw_color(204, 204, 0);
		test_point.draw(VIEWPORT_W, VIEWPORT_H);

		draw_line(render, player.position(), player.position() + ray_dir);

		render.update();

	}
	return 0;
};
