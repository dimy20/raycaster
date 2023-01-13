#include <iostream>

#include "Engine.h"
#include "Player.h"

Player::Player(Engine * engine, Render * render, Map * map, Math::Vec2 position, Math::Vec2 direction){
	// TODO: position should never exceed map boundaries!
	m_position = position;
	m_direction = direction;
	m_render = render;
	m_direction.normalize();
	m_map = map;
	m_engine = engine;
}

void Player::draw(){
	SDL_Rect viewport = m_render->viewport();

	const size_t MAP_W = m_map->world_width();
	const size_t MAP_H = m_map->world_heigth();

	auto [x, y] = Math::map2_screen(MAP_W, MAP_H, viewport.w, viewport.h, m_position);

	m_render->set_draw_color(0, 0xff, 0);

	size_t rect_w = 5;
	size_t rect_h = 5;
	m_rect = {x - (int)(rect_w / 2), y - (int)(rect_h / 2), (int)rect_w, (int)rect_h};
	SDL_RenderFillRect(m_render->renderer(), &m_rect);
};

void Player::update(){

	Math::Vec2 new_pos;
	auto keyboard = m_engine->keyboard_input();
	if(keyboard[SDL_SCANCODE_D]){
		m_direction.rotate(Math::to_rad(-ROTATION_SPEED));
	}
	if(keyboard[SDL_SCANCODE_A]){
		m_direction.rotate(Math::to_rad(ROTATION_SPEED));
	}

	Math::Vec2 temp = m_direction;
	auto& [tempx, tempy] = temp.xy();
	(void)tempx;
	tempy *= -1;
	
	if(keyboard[SDL_SCANCODE_W]){
		new_pos = m_position + (temp * MOV_SPEED);
	}

	if(keyboard[SDL_SCANCODE_S]){
		new_pos = m_position + (temp * -MOV_SPEED);
	}

	int x = new_pos.x() / Map::CELL_SIZE;
	int y = new_pos.y() / Map::CELL_SIZE;
	if(m_map->at(x, y) == 0){
		m_position = new_pos;
	}

};
