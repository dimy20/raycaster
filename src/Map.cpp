#include "Map.h"
#include <cstring>

Map::Map(Render * render){
	m_render = render;
	m_width = 8;
	m_heigth = 8;
	m_map = std::vector<std::vector<int>>(m_heigth, std::vector<int>(m_width, 0));
	m_map = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 2, 0, 2, 0, 1},
		{1, 0, 0, 2, 0, 2, 0, 1},
		{1, 0, 0, 2, 0, 2, 0, 1},
		{1, 0, 0, 2, 2, 2, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
	};
}

void Map::draw(){
	SDL_Rect viewport = m_render->viewport();
	// Want to draw each cell relative to the viewports dimensions!
	// Independer to whatever the viewports dimensions might be.
	size_t rect_w = viewport.w / m_width;// each cell will ocupy k = vportw / map_w;
	size_t rect_h = viewport.h / m_heigth;

	// red gree blue
	uint32_t colors[6];
	memset(colors, 0, sizeof(uint32_t) * 6);
	colors[1] = 0xff000000;
	for(int i = 2; i < 6; i++){
		colors[i] = 0x0000ff00;
	}

	// draw the map
	SDL_Rect rect;
	m_render->set_draw_color(0xff, 0xff, 0xff);
	for(size_t i = 0; i < m_heigth; i++){
		for(size_t j = 0; j < m_width; j++){
			if(m_map[i][j] == 0) continue;
			int cell_value = m_map[i][j];
			uint32_t color = colors[cell_value];
			uint8_t r, g, b;
			r = (color & 0xff000000) >> 24;
			g = (color & 0x00ff0000) >> 16;
			b = (color & 0x0000ff00) >> 8;

			// world to screen convertion
			int x = j * rect_w;
			int y = i * rect_h;
			rect = {x, y, (int)rect_w, (int)rect_h};

			m_render->set_draw_color(r, g, b);
			SDL_RenderFillRect(m_render->renderer(), &rect);
		}
	}
	// draw the grid

	m_render->set_draw_color(0xff, 0xff, 0xff);
	// draw horizontal lines
	int y1 = 0, y2 = 0;
	for(size_t i = 0; i < m_heigth; i++){
		y1 = i *rect_h;
		y2 = i *rect_h;
		SDL_RenderDrawLine(m_render->renderer(), 0, y1, viewport.w - 1, y2);
	}

	int x1 = 0, x2 = 0;
	for(size_t j = 0; j < m_width; j++){
		x1 = j *rect_w;
		x2 = j *rect_w;
		SDL_RenderDrawLine(m_render->renderer(), x1, 0, x2, viewport.h -1);
	}

	m_render->set_draw_color(0xff, 0, 0);
	SDL_RenderDrawPoint(m_render->renderer(), viewport.w / 2, viewport.h /2);
};
