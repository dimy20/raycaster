#include "Map.h"
#include "Player.h"

/*
void tmp_ray_casting(const Map& map, const Player& player){
	glm::vec2 ray_direction;
	glm::vec2 camera_plane = player.camera_plane();
	glm::vec2 player_direction = player.direction();
	glm::vec2 player_position = player.position();

	for(size_t x = 0; x < map.width(); x++){
		int map_x = int(player_position.x);
		int map_y = int(player_position.y);

		float camera_x = 2 *(x / (float)map.width())- 1;

		ray_direction = player_direction + (camera_plane * camera_x);

		float delta_x = ray_direction.x == 0 ? 1e30 : std::abs(1 / ray_direction.x);
		float delta_y = ray_direction.y == 0 ? 1e30 : std::abs(1 / ray_direction.y);

		int step_x, step_y;
		
		int hit = 0;
		int side;

		if(ray_direction.x < 0){
			step_x = -1;
			side_x_dist = (player_position.x - map_x) * delta_x;
		}else{
			step_x = -1;
			side_x_dist = ((map_x + 1) - player_position.x) * delta_x;
		}

		if(ray_direction.y < 0){
			step_y = -1;
			side_y_dist = (player_position.y - map_y) * delta_y;
		}else{
			step_y = 1;
			side_y_dist = ((map_y + 1) - payer_position.y) * delta_y;
		}


	};

};
*/
