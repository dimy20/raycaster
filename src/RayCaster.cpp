#include "RayCaster.h"
#include "Player.h"
#include <limits>

void RayCaster::init(int plane_w, int plane_h){
	m_plane_width = plane_w;
	m_plane_height = plane_h;
	m_plane_center = plane_h / 2;
	m_angle_step = FOV / (float)plane_w;
	m_plane_dim = plane_w * plane_h;
	m_player_plane_dist = static_cast<size_t>((plane_w / 2) / tan(Math::to_rad(FOV / 2)));
};

HitPoint RayCaster::cast_horizontal_intercept(float ray_angle, const int px, const int py, const Map& map){
	/* H is the point used to trace horizontal hits along the ray */
	float Hx, Hy; 
	/* Fixed step of size CELL_SIZE along y axis, direction depends on ray_angle*/
	int step_y;  
	/* Fixed step of size CELL_SIZE / tan(ray_angle) along x, direction depends on ray_angle*/
	float delta_step_x;  
	/* Distance from hit point to player position.*/
	float distance;

	if(ray_angle > 0 && ray_angle < 180){ // facing up
		/* Get first horizontal hit point's y coordiante. */
		Hy = ((int)(py / (Map::CELL_SIZE)) * (int)Map::CELL_SIZE);
		step_y = -Map::CELL_SIZE; // step at fixed Map::CELL_SIZE up every iteration

		/* Get first horizontal hit point's x coordiante. */
		Hx = px + ((py - Hy) / tan(Math::to_rad(ray_angle)));

		/*This is needed to get the above map cell when we devide
		 * by Map::CELL_SIZE to get map_y*/
		Hy -= 1; 
	}else{ // facing down

		/*Same as above except the y coordinate will be on the next
		 * map cell line.*/
		Hy = ((int)(py / Map::CELL_SIZE) * Map::CELL_SIZE) + Map::CELL_SIZE;
		step_y = Map::CELL_SIZE;

		Hx = px + ((py - Hy) / tan(Math::to_rad(ray_angle)));
	}

	if(ray_angle != 0 && ray_angle != 180){
		/*At this point we step on x by this fixed amount. */
		delta_step_x = -step_y / tan(Math::to_rad(ray_angle));
		bool hit = false;
		while(!hit){
			int map_x = (int)(Hx / Map::CELL_SIZE);
			int map_y = Hy / Map::CELL_SIZE;

			if(map_x >= (int)map.width() || 
			   map_y >= (int)map.heigth() || 
			   map_y < 0 || map_x < 0){
				break;
			}else if(map.at(map_x, map_y) > 0){ // HIT ?
				hit = true;
				if(map.at(map_x, map_y) == 1)
					m_render->set_draw_color(0xff, 0, 0);
				else
					m_render->set_draw_color(0, 0xff, 0);
			}else{
				/*Move the point along the ray direction to the next Horizontal intersection*/
				Hx += delta_step_x;
				Hy += step_y;
			}
		};

		if(hit){
			distance = 0.0f;
		}else{
			distance = std::numeric_limits<float>::max();
		}

	}else{
		distance = std::numeric_limits<float>::max();
	}

	return {Hx, Hy, distance};
};


HitPoint RayCaster::cast_vertical_intercept(float ray_angle, const int px, const int py, const Map& map){
	/* V is the point used to trace horizontal hits along the ray */
	float Vx, Vy;
	/* Fixed step of size CELL_SIZE along x axis, direction depends on ray_angle*/
	int step_x;
	/* Fixed step of size CELL_SIZE * tan(ray_angle) along y, direction depends on ray_angle*/
	float delta_step_y;
	/* Distance from hit point to player position.*/
	float distance;

	if(ray_angle < 90 || ray_angle > 270){
		/* Get first vertical hit point's x coordiante. */
		Vx = (int)(px / Map::CELL_SIZE) * Map::CELL_SIZE + Map::CELL_SIZE;
		step_x = Map::CELL_SIZE;
		/* Compute the y coordinate */
		Vy = py - ((Vx - px) * tan(Math::to_rad(ray_angle)));
	}else{
		/*Same as above except the x coordinate will be on the first 
		 * vertical line of the current cell.*/
		Vx = (int)(px / Map::CELL_SIZE) * Map::CELL_SIZE;
		step_x = -Map::CELL_SIZE;
		Vy = py - ((Vx - px) * tan(Math::to_rad(ray_angle)));
		Vx--;
	}

	if(ray_angle != 90 && ray_angle != 270){
		/*At this point we step on y by this fixed amount. */
		delta_step_y = -step_x * tan(Math::to_rad(ray_angle));
		bool hit = false;
		while(!hit){
				int map_x = (Vx / Map::CELL_SIZE);
				int map_y = (int)(Vy / Map::CELL_SIZE);

				if(map_x >= (int)map.width() || 
				   map_y >= (int)map.heigth() || 
				   map_y < 0 || map_x < 0){
					break;
				}else if(map.at(map_x, map_y)){
					hit = true;
					if(map.at(map_x, map_y) == 1)
						m_render->set_draw_color(0xff, 0, 0);
					else
						m_render->set_draw_color(0, 0xff, 0);
				}else{
					// step
					Vx += step_x;
					Vy += delta_step_y;
				}
		}
		if(hit){
			distance = 0.0f;
		}else{
			distance = std::numeric_limits<float>::max();
		}

	}else{
		distance = std::numeric_limits<float>::max();
	}

	return {Vx, Vy, distance};

};

/* This function gets the perpedicular distance to the wall,
   That is the distance along the player's view direction, we'll use this distance to calculate
   the perspective on the walls, if we used the distance from the point of intersection to the player's
   position instead we get a fish eye effect, this makes sense since the farther to the right or left the point is
   the higher the depth value will be, so we draw a smaller wall slice. Since the screen is flat we wont use this value
   however we can correct this effect by using the perpendicular distance to the wall.
   The formula is derived using 3 angles:
   theta -> The viewing direction angle
   alpha-> the ray angle
   beta-> the angle between the ray and the viewing direction
   D = dist(p, hit) (the distance we dont want to use)
   then -> perp_dist = D * cos(beta) = D * cos(alpha - theta)
   Using cos(a - b) identity and the point found hit we derive the formula bellow.*/

static float get_perpendicular_distance(float viewing_angle, float px, float py, float hit_x, float hit_y){
	float dx = hit_x - px;
	float dy = py - hit_y;
	return (dx * cos(Math::to_rad(viewing_angle))) + (dy * sin(Math::to_rad(viewing_angle)));
}

/* Based on a distance depth and the current colum, draw a wall slice with appropriate perspective.*/
void RayCaster::draw_wall_slice(const float dist_to_slice, int col){
	int projected_slice_height;

	/*Derived from similar triangle relation. */
	projected_slice_height = static_cast<int>((Map::CELL_SIZE * (float)m_player_plane_dist) / dist_to_slice);

	int wall_bottom = (projected_slice_height / 2) + m_plane_center;
	//int wall_top = m_plane_center - (projected_slice_height / 2);
	int wall_top = m_plane_height - wall_bottom;

	if(wall_top < 0 ) wall_top = 0;
	if(wall_bottom >= m_plane_height) wall_bottom = m_plane_height - 1;

	SDL_RenderDrawLine(m_render->renderer(), col, wall_top, col, wall_bottom);
};

std::vector<std::pair<int , int >> RayCaster::cast(const Player& player, const Map& map){
	auto dir = player.direction();
	float viewing_angle = Math::to_deg(dir.angle());

	float ray_angle = viewing_angle + (FOV / 2); // start at the left end of the fov arc

	std::vector<std::pair<int, int >> points;

	m_render->set_viewport(Map::CELL_SIZE * 8, 0, 320, 200);

	for(int col = 0; col < m_plane_width; col++){
		if(ray_angle < 0) ray_angle += 360.0f;

		auto px = player.position().x();
		auto py = player.position().y();

		auto h_hit = cast_horizontal_intercept(ray_angle, px, py, map);
		if(h_hit.distance != std::numeric_limits<float>::max()){
			h_hit.distance = get_perpendicular_distance(viewing_angle, px, py, h_hit.x, h_hit.y);
		}

		auto v_hit = cast_vertical_intercept(ray_angle, px, py, map);
		if(v_hit.distance != std::numeric_limits<float>::max()){
			v_hit.distance = get_perpendicular_distance(viewing_angle, px, py, v_hit.x, v_hit.y);
		}

		float distance = std::min(h_hit.distance, v_hit.distance);

		//Just draw the closest ray for debug
		if(h_hit.distance < v_hit.distance){
			points.push_back({h_hit.x, h_hit.y});
		}else{
			points.push_back({v_hit.x, v_hit.y});
		}

		draw_wall_slice(distance, col);

		ray_angle -= m_angle_step;
		if(ray_angle >= 360) ray_angle -= 360.0f;
	}

	return points;
}

