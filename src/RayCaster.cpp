#include "RayCaster.h"
#include "Player.h"

#define FOV 60.0
#define PLAYER_HEIGHT 32
#define PROJ_PLANE_W 320
#define PROJ_PLANE_H 200

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

	/* For every casted ray:
	 * V is the point used to trace vertical hits along the ray
	 * H is the point used to trace horizontal hits along the ray */
	float Vx, Vy; 
	float Hx, Hy;

	/*Fixed steps of size Map::CELL_SIZE in x and y direction after first vertical and horizontal hit
	 * is found respectively, The direction sign will depend on the angle of the current ray.*/
	int step_y; 
	int step_x; 

	/*Fixed step along the x direction of size Map::CELL_SIZE / tan(alpha).
	 * This step will be used to move Hx after the first horizontal hit
	 * is found. The sign will depend on alpha*/
	float delta_step_x; 

	/*Fixed step along the y direction of size Map::CELL_SIZE * tan(alpha).
	 * This step will be used to move Vy after the first vertical hit
	 * is found. The sign will depend on alpha*/
	float delta_step_y;

	std::vector<std::pair<int, int >> points;
	for(size_t i = 0; i < PROJ_PLANE_W; i++){
		/* Bounds */
		if(ray_angle >= 180) ray_angle -= 360;
		if(ray_angle <= -180) ray_angle += 360;

		const auto& [px, py] = player.position().xy();

		//HORIZONTAL HITS!
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
				if(map_x >= (int)map.width() || map_y >= (int)map.heigth() || map_y < 0 || map_x < 0){
					break;
				}else if(map.at(map_x, map_y)){ // HIT ?
					//points.push_back({Hx, Hy});
					break;
				}else{
					/*Move the point along the ray direction to the next Horizontal intersection*/
					Hx += delta_step_x;
					Hy += step_y;
				}
			};

		}

		//VERTICAL HITS
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
			while(true){
					int map_x = (Vx / Map::CELL_SIZE);
					int map_y = (int)(Vy / Map::CELL_SIZE);
					if(map_x >= (int)map.width() || map_y >= (int)map.heigth() || map_y < 0 || map_x < 0){
						break;
					}else if(map.at(map_x, map_y)){
						points.push_back({Vx, Vy});
						break;
					}else{
						Vx += step_x;
						Vy += delta_step_y;
					}
			}

		}

		ray_angle += angle_step;
	}

	return points;
};
