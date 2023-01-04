#include "utils.h"
std::pair<int , int> Utils::map2_screen(size_t map_w, size_t map_h, size_t vp_w, size_t vp_h, const glm::vec2 position){
	int ans_x, ans_y;
	float map_2_screenX = (float)vp_w / (float)map_w;
	float map_2_screenY = (float)vp_h / (float)map_h;
	ans_x = (int)(position.x * map_2_screenX);
	ans_y = (int)(position.y * map_2_screenY);

	return {ans_x, ans_y};
};

void Utils::log_vector(glm::vec2 vec){
	std::cout << "(" << vec.x << ", " << vec.y << ")\n";
};
