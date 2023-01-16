#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "Render.h"

enum CellType{
	EMPTY = 0,
	WALL
};

struct Cell{
	CellType type;
	SDL_Surface * texture;
	uint32_t color;
};

class Map{
	public:
		Map() = default;
		Map(Render * render);
		void draw();
		void temp_ray_casting();
		constexpr size_t width() const { return m_width; };
		constexpr size_t heigth() const { return m_heigth; };
		constexpr size_t world_width() const { return m_width * CELL_SIZE; };
		constexpr size_t world_heigth() const { return m_heigth * CELL_SIZE; };
		int at(int x, int y) const { return m_map[y][x]; };
		constexpr std::array<Cell, 10> cell_table() const { return m_cells; };
	private:
		size_t m_width = 0;
		size_t m_heigth = 0;
		std::vector<std::vector<int>> m_map;
		std::array<Cell, 10> m_cells;
		Render * m_render;
	public:
		static const int CELL_SIZE = 64; // The world if made of cubes with dimensions (64, 64, 64)
};
