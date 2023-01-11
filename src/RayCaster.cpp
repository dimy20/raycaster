#include "RayCaster.h"
#include "Player.h"
#include <limits>

#define FOV 60.0
#define PLAYER_HEIGHT 32
#define PROJ_PLANE_W 320
#define PROJ_PLANE_H 200

HitPoint RayCaster::cast_horizontal_intercept(float ray_angle, int px, int py, const Map& map){
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

		/*Correct negative angles so we point in the correct direction*/
		float alpha = -tan(Math::to_rad(ray_angle));
		Hx = px + ((Hy - py) / alpha);
	}

	if(ray_angle != 0 && ray_angle != 180){
		/*At this point we step on x by this fixed amount. */
		delta_step_x = -step_y/ tan(Math::to_rad(ray_angle));
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
			}else{
				/*Move the point along the ray direction to the next Horizontal intersection*/
				Hx += delta_step_x;
				Hy += step_y;
			}
		};

		if(hit){
			// distance to point of intersection
			distance = std::abs(px - Hx) * cos(ray_angle);
		}else{
			distance = std::numeric_limits<float>::max();
		}

	}else{
		distance = std::numeric_limits<float>::max();
	}

	return {Hx, Hy, distance};
};


HitPoint RayCaster::cast_vertical_intercept(float ray_angle, int px, int py, const Map& map){
	/* V is the point used to trace horizontal hits along the ray */
	float Vx, Vy;
	/* Fixed step of size CELL_SIZE along x axis, direction depends on ray_angle*/
	int step_x;
	/* Fixed step of size CELL_SIZE * tan(ray_angle) along y, direction depends on ray_angle*/
	float delta_step_y;
	/* Distance from hit point to player position.*/
	float distance;

	if((ray_angle > 0 && ray_angle < 90) || (ray_angle < 0 &&  ray_angle > -90)){
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
		Vy = py + ((px - Vx) * tan(Math::to_rad(ray_angle)));
		Vx--;
	}

	if(ray_angle != 90 && ray_angle != -45 && ray_angle != 0){
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
				}else{
					// step
					Vx += step_x;
					Vy += delta_step_y;
				}
		}
		if(hit){
			distance = std::abs(py - Vy) * sin(ray_angle);
		}else{
			distance = std::numeric_limits<float>::max();
		}

	}else{
		distance = std::numeric_limits<float>::max();
	}

	return {Vx, Vy, distance};

};

std::vector<std::pair<int , int >> RayCaster::cast(const Player& player, const Map& map){
	const size_t proj_plane_dim = PROJ_PLANE_W * PROJ_PLANE_H;
	Math::Vec2 proj_plane_center(PROJ_PLANE_W / 2, PROJ_PLANE_H / 2);

	// distance from player position to the projection plane
	const size_t proj_plane_dist = static_cast<size_t>((PROJ_PLANE_W / 2) / tan(Math::to_rad(FOV / 2)));

	/* FOV / NUM_COLUMS = The angle each ray spans for a colum*/
	const float angle_step = FOV / (float)PROJ_PLANE_W;

	(void)proj_plane_dim;
	(void)proj_plane_center;
	(void)proj_plane_dist;


	auto dir = player.direction();
	float viewing_angle = Math::to_deg(dir.angle());

	float ray_angle = viewing_angle - (FOV / 2); // start at the left end of the fov arc

	std::vector<std::pair<int, int >> points;
	for(size_t i = 0; i < PROJ_PLANE_W; i++){
		/* Bounds */
		if(ray_angle >= 180) ray_angle -= 360;
		if(ray_angle <= -180) ray_angle += 360;

		const auto& [px, py] = player.position().xy();

		auto h_hit = cast_horizontal_intercept(ray_angle, px, py, map);
		if(h_hit.distance != std::numeric_limits<float>::max()){
			//UNIMPLEMENTED
		}

		auto v_hit = cast_vertical_intercept(ray_angle, px, py, map);
		if(v_hit.distance != std::numeric_limits<float>::max()){
			//UNIMPLEMENTED
		}

		ray_angle += angle_step;
	}

	return points;
};
