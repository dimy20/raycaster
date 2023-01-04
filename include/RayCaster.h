#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "Render.h"
#include "Map.h"
#include "utils.h"
class Player;

class Ray{
	public:
		Ray(glm::vec2 start_pos, glm::vec2 dir) : m_dir(dir), m_start_pos(start_pos) {};
		void set_Hinter(double x_component, double y_component);
		//void set_interY(double x_component, double y_component);
		Ray(Ray && other);
		Ray& operator = (Ray && other);
		void draw(Render& render, const Map& map);

		constexpr glm::vec2 direction() const { return m_dir; };
		constexpr glm::vec2 start() const { return m_start_pos; };
	private:
		/*Ray's direction vector*/
		glm::vec2 m_dir;
		/* Ray's starting positions*/
		glm::vec2 m_start_pos;
		/*First horizontal intersection (Setup for DDA)*/
		std::pair<double, double> m_Hinter_components;
		/*First vertical intersection (Setup for DDA)*/
		std::pair<double, double> m_Vinter_components;

		bool m_draw_h_step = false;
		bool m_draw_v_step = false;
};

class RayCaster{
	public:
		RayCaster() = default;
		std::vector<Ray> apply(const Map& map, const Player& player);
};
