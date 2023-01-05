#include "RayCaster.h"
#include "Player.h"

Ray& Ray::operator=(Ray && other){
	if(this == &other) return *this;

	m_dir = std::move(other.m_dir);
	m_start_pos = std::move(other.m_start_pos);
	m_Hinter_components = std::move(other.m_Hinter_components);
	m_Vinter_components = std::move(other.m_Vinter_components);

	return *this;
};

Ray::Ray(Ray && other){ *this = std::move(other); }
void Ray::set_Hinter(double x, double y){ m_Hinter_components = {x, y}; };
void Ray::set_Vinter(double x, double y){ m_Vinter_components = {x, y}; };

void Ray::draw(Render& render, const Map& map){
	SDL_Rect viewport = render.viewport();

	size_t map_W = map.width();
	size_t map_H = map.heigth();

	auto[x1_vp, y1_vp] = Math::map2_screen(map_W, map_H, viewport.w, viewport.h, m_start_pos);

	Math::Vec2 point2 = m_start_pos + (3.0f * m_dir);
	auto[x2_vp, y2_vp] = Math::map2_screen(map_W, map_H, viewport.w, viewport.h, point2);

	render.set_draw_color(0xff, 0xff, 0xff);
	SDL_RenderDrawLine(render.renderer(), x1_vp, y1_vp, x2_vp, y2_vp);



	if(m_draw_h_step){
		const auto& [comp_x, comp_y] = m_Hinter_components;
		Math::Vec2 h_point(comp_x, comp_y);

		auto[x, y] = Math::map2_screen(map_W, map_H, viewport.w, viewport.h, h_point);

		SDL_Rect rect;
		size_t rect_w = 5;
		size_t rect_h = 5;

		rect = {x - (int)(rect_w / 2), y - (int)(rect_h / 2), (int)rect_w, (int)rect_h};

		SDL_RenderFillRect(render.renderer(), &rect);

	};

	if(m_draw_v_step){
		const auto& [comp_x, comp_y] = m_Vinter_components;
		Math::Vec2 v_point(comp_x, comp_y);

		//Utils::log_vector(v_point);
		auto[x, y] = Math::map2_screen(map_W, map_H, viewport.w, viewport.h, v_point);

		SDL_Rect rect;
		size_t rect_w = 5;
		size_t rect_h = 5;

		rect = {x - (int)(rect_w / 2), y - (int)(rect_h / 2), (int)rect_w, (int)rect_h};

		SDL_RenderFillRect(render.renderer(), &rect);
	}
};

std::vector<Ray> RayCaster::apply(const Map& map, const Player& player){

	auto camera_plane = player.camera_plane();
	auto player_direction = player.direction();
	auto player_position = player.position();
	// horizontal interceptions
	// getting alpha

	double intersect_x;
	double intersect_y;

	double dx, dy;
	std::vector<Ray> rays;
	size_t n = map.width();
	//size_t n = 1;
	Math::Vec2 ray_dir;
	for(size_t x = 0; x < n; x++){
		size_t TILE_I, TILE_J;
		TILE_I = (size_t)player_position.x();
		TILE_J = (size_t)player_position.y();
		//std::cout << "Tile i: " << TILE_I << "\n";
		//std::cout << "Tile j: " << TILE_J << "\n";
		//double alpha = atan2
		/*Map columns to camera space*/
		float camera_x = 2 *(x / (float)map.width())- 1;
		ray_dir = player_direction + (camera_plane * camera_x);
		double alpha = atan2(ray_dir.y(), ray_dir.x());

		//double alpha = (tmp/ M_PI) * 180.0; // direction of the ray in angle form

		// setup the offsets



		/* Setup of the dy offset for the initial computation of intersection
		 * with the first horizontal line:
		 * */
		int step_dir_x, step_dir_y;
		if(ray_dir.y()< 0){ // Remember that origin is at the top left corner, so negative direction means up.
			dy = (player_position.y() - TILE_J) * -1; // multiplied by -1 because the direction of the vector will be up
			step_dir_y = 1;
		}else{
			dy = (TILE_J + 1) - player_position.y();
			step_dir_y = -1;
		}

		if(ray_dir.x() < 0){
			dx = player_position.x() - TILE_I;
			step_dir_x = 1;
		}else{
			dx = ((TILE_I + 1) - player_position.x());
			step_dir_x = -1;
		}

		/*
		 *   -------/--  ^
		 *  |      /   | | dy
		 *  |     /    | |
		 *  |<dx>* (1) | -
		 *  |          |
		 *   ----------
		 *   (1) -> dy / tan(alpha)
		 * */

		 
		/*Step needed to take in the x direction to hit the first
		 * horizontal line on the grid.*/
		//intersect_x = TILE_J + dx + dy / tan(alpha);
		intersect_x = player_position.x() + (dy / tan(alpha));
		intersect_y = player_position.y() + (dx * tan(alpha));

		//std::cout << "(" << intersect_x << ", " << dy << ")\n";

		Ray ray(player.position(), ray_dir);

		ray.set_Hinter(intersect_x, player.position().y() + dy);
		ray.set_Vinter(player.position().x() + dx, intersect_y);
		
		rays.push_back(std::move(ray));
		//ray.set_interX(intersect_x, dy);

		(void)step_dir_x;
		(void)step_dir_y;
	}

	return rays;
};
