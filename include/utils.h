#include <iostream>
#include <glm/glm.hpp>

// scale convertion from point in the map to viewport
// map[x, y] -> viewport[x', y']
// x, y real numbers.

namespace Utils{
	std::pair<int , int> map2_screen(size_t map_w, size_t map_h, size_t vp_w, size_t vp_h, const glm::vec2 position); 
	void log_vector(glm::vec2 vec);
}

