#include <iostream>
#include "Player.h"

Player::Player(Render * render, Map * map, Math::Vec2 position, Math::Vec2 direction){
	// TODO: position should never exceed map boundaries!
	m_position = position;
	m_direction = direction;
	m_camera_plane = Math::Vec2(0.0f, 0.66f);
	m_render = render;
	m_direction.normalize();
	m_map = map;
}

void Player::draw(){
	SDL_Rect viewport = m_render->viewport();
	// TODO: dont type this in.
	const size_t MAP_W = m_map->width();
	const size_t MAP_H = m_map->heigth();

	auto [x, y] = Math::map2_screen(MAP_W, MAP_H, viewport.w, viewport.h, m_position);

	m_render->set_draw_color(0, 0xff, 0);

	size_t rect_w = 5;
	size_t rect_h = 5;
	m_rect = {x - (int)(rect_w / 2), y - (int)(rect_h / 2), (int)rect_w, (int)rect_h};
	SDL_RenderFillRect(m_render->renderer(), &m_rect);
};


void Player::keypressed(const SDL_Keycode& key){
	Math::Vec2 new_pos;
	switch(key){
		case SDLK_w:
			new_pos = m_position + (m_direction * MOV_SPEED);
			break;
		case SDLK_s:
			new_pos = m_position + (m_direction * -MOV_SPEED);
			break;
		case SDLK_d:
			m_direction.rotate(Math::to_rad(ROTATION_SPEED));
			m_camera_plane.rotate(Math::to_rad(ROTATION_SPEED));
			break;
		case SDLK_a:
			m_direction.rotate(Math::to_rad(-ROTATION_SPEED));
			m_camera_plane.rotate(Math::to_rad(-ROTATION_SPEED));
			break;
	}

	if(m_map->at((int)new_pos.x(), (int)new_pos.y()) == 0){
		m_position = new_pos;
	}
};
