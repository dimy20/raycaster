#include "Player.h"
#include "utils.h"
#include <iostream>

Player::Player(Render * render, glm::vec2 position, glm::vec2 direction){
	// TODO: position should never exceed map boundaries!
	m_position = position;
	m_direction = glm::normalize(direction);
	m_camera_plane = glm::vec2(0.0, 0.66f);
	m_render = render;
}

void Player::draw(const size_t vp_w, const size_t vp_h){
	//

	/*
	if(m_draw_dir){
		glm::vec2 dest = (m_direction * 30.f + m_position);
		m_render->set_draw_color(0xff, 0xff, 0xff);
		SDL_RenderDrawLine(m_render->renderer(), 0, 0, (int)m_position.x , (int)m_position.y );
		SDL_RenderDrawLine(m_render->renderer(), (int)m_position.x, (int)m_position.y, (int)dest.x,(int)dest.y);
	};
	*/

	// TODO: dont type this in.
	const size_t MAP_W = 24;
	const size_t MAP_H = 24;

	auto [x, y] =  map2_screen(MAP_W, MAP_H, vp_w, vp_h, m_position);

	m_render->set_draw_color(0, 0xff, 0);

	size_t rect_w = 5;
	size_t rect_h = 5;
	m_rect = {x - (int)(rect_w / 2), y - (int)(rect_h / 2), (int)rect_w, (int)rect_h};
	SDL_RenderFillRect(m_render->renderer(), &m_rect);
};

