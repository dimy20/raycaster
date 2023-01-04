#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "Render.h"

class Map{
	public:
		Map(Render * render);
		void draw();
		void temp_ray_casting();
		constexpr size_t width() const { return m_width; };
		constexpr size_t heigth() const { return m_heigth; };
	private:
		size_t m_width = 0;
		size_t m_heigth = 0;
		std::vector<std::vector<int>> m_map;
		Render * m_render;
};
