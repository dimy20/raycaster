#include "RayCaster.h"
#include "Player.h"
#include <limits>

#define VIEWPORT_W Map::CELL_SIZE * 8
#define VIEWPORT_H Map::CELL_SIZE * 8

std::pair<int, int> world2screen2(const int x, const int y){
	const size_t MW = 8;
	const size_t MH = MW;
	int screen_x = x * (64 * 8 / (MW * Map::CELL_SIZE));
	int screen_y = y * (64 * 8 / (MH * Map::CELL_SIZE));
	return {screen_x, screen_y};
}



void RayCaster::init(int plane_w, int plane_h){
	m_plane_width = plane_w;
	m_plane_height = plane_h;
	m_plane_center = plane_h / 2;
	m_angle_step = FOV / (float)plane_w;
	m_plane_dim = plane_w * plane_h;
	m_player_plane_dist = static_cast<size_t>((plane_w / 2) / tan(Math::to_rad(FOV / 2)));
	m_points.resize(m_plane_width);
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

float RayCaster::cast_horizontal_intercept(float ray_angle, const int px, const int py, const Map& map){
	/* Fixed step of size CELL_SIZE along y axis, direction depends on ray_angle*/
	int step_y;  
	/* Fixed step of size CELL_SIZE / tan(ray_angle) along x, direction depends on ray_angle*/
	float delta_step_x;  
	/* Distance from hit point to player position.*/
	float distance;

	if(ray_angle > 0 && ray_angle < 180){ // facing up
		/* Get first horizontal hit point's y coordiante. */
		m_hitHy = ((int)(py / (Map::CELL_SIZE)) * (int)Map::CELL_SIZE);
		step_y = -Map::CELL_SIZE; // step at fixed Map::CELL_SIZE up every iteration

		/* Get first horizontal hit point's x coordiante. */
		m_hitHx = px + ((py - m_hitHy) / tan(Math::to_rad(ray_angle)));

		/*This is needed to get the above map cell when we devide
		 * by Map::CELL_SIZE to get map_y*/
		m_hitHy -= 1; 
	}else{ // facing down

		/*Same as above except the y coordinate will be on the next
		 * map cell line.*/
		m_hitHy = ((int)(py / Map::CELL_SIZE) * Map::CELL_SIZE) + Map::CELL_SIZE;
		step_y = Map::CELL_SIZE;

		m_hitHx = px + ((py - m_hitHy) / tan(Math::to_rad(ray_angle)));
	}

	distance = std::numeric_limits<float>::max();

	m_hcell_id = 0;
	if(ray_angle != 0 && ray_angle != 180){
		/*At this point we step on x by this fixed amount. */
		delta_step_x = -step_y / tan(Math::to_rad(ray_angle));
		bool hit = false;
		while(!hit){
			int map_x = (int)(m_hitHx / Map::CELL_SIZE);
			int map_y = m_hitHy / Map::CELL_SIZE;
			if(map_x >= (int)map.width() || 
			   map_y >= (int)map.heigth() || 
			   map_y < 0 || map_x < 0){
				break;
			}else if(map.at(map_x, map_y) > 0){ // HIT ?
				hit = true;
				distance = get_perpendicular_distance(m_viewing_angle, px, py, m_hitHx, m_hitHy);
				m_hcell_id = map.at(map_x, map_y);
			}else{
				/*Move the point along the ray direction to the next Horizontal intersection*/
				m_hitHx += delta_step_x;
				m_hitHy += step_y;
			}
		};
	}

	return distance;
};


float RayCaster::cast_vertical_intercept(float ray_angle, const int px, const int py, const Map& map){
	/* Fixed step of size CELL_SIZE along x axis, direction depends on ray_angle*/
	int step_x;
	/* Fixed step of size CELL_SIZE * tan(ray_angle) along y, direction depends on ray_angle*/
	float delta_step_y;
	/* Distance from hit point to player position.*/
	float distance;

	if(ray_angle < 90 || ray_angle > 270){
		/* Get first vertical hit point's x coordiante. */
		m_hitVx = (int)(px / Map::CELL_SIZE) * Map::CELL_SIZE + Map::CELL_SIZE;
		step_x = Map::CELL_SIZE;
		/* Compute the y coordinate */
		m_hitVy = py - ((m_hitVx - px) * tan(Math::to_rad(ray_angle)));
	}else{
		/*Same as above except the x coordinate will be on the first 
		 * vertical line of the current cell.*/
		m_hitVx = (int)(px / Map::CELL_SIZE) * Map::CELL_SIZE;
		step_x = -Map::CELL_SIZE;
		m_hitVy = py - ((m_hitVx - px) * tan(Math::to_rad(ray_angle)));
		m_hitVx--;
	}


	// assume failure
	distance = std::numeric_limits<float>::max();

	m_vcell_id = 0;
	if(ray_angle != 90 && ray_angle != 270){
		/*At this point we step on y by this fixed amount. */
		delta_step_y = -step_x * tan(Math::to_rad(ray_angle));
		bool hit = false;
		while(!hit){
				int map_x = (m_hitVx / Map::CELL_SIZE);
				int map_y = (int)(m_hitVy / Map::CELL_SIZE);

				if(map_x >= (int)map.width() || 
				   map_y >= (int)map.heigth() || 
				   map_y < 0 || map_x < 0){
					break;
				}else if(map.at(map_x, map_y)){
					hit = true;
					distance = get_perpendicular_distance(m_viewing_angle, px, py, m_hitVx, m_hitVy);
					m_vcell_id = map.at(map_x, map_y);
				}else{
					// step
					m_hitVx += step_x;
					m_hitVy += delta_step_y;
				}
		}

	}

	return distance;
};

void RayCaster::column_map_texture(SDL_Surface * texture, int texture_x, int column_height, int screen_column){
	assert(texture != NULL);
	uint8_t * pixels = reinterpret_cast<uint8_t *>(texture->pixels);

	int pixel_y;
	int texture_y;
	int pixel_offset;

	uint8_t r, g, b;
	size_t texture_size = texture->w;

	for(int i = 0; i < column_height; i++){
		// location of this texture pixel on screen?
		pixel_y = i + (m_plane_center - column_height / 2);
		if(pixel_y >= 0 && pixel_y < m_plane_height){

			/*Makes the following mapping of values from [0, size] -> [0, column_height]
			 *Scaling the original texture to column height*/
			texture_y = (int)((i * texture_size) / column_height);
			pixel_offset = ((texture_y * texture->w) + texture_x) * 4;

			r = pixels[pixel_offset];
			g = pixels[pixel_offset + 1];
			b = pixels[pixel_offset + 2];

			m_framebuffer->set_pixel(screen_column, pixel_y, r, g, b);
		}
	}
};

/* Based on a distance depth and the current colum, draw a wall slice with appropriate perspective.*/
void RayCaster::draw_wall_slice(const float dist_to_slice, int col, int cell_id, int offset){
	int projected_slice_height;

	/*Derived from similar triangle relation. */
	projected_slice_height = static_cast<int>((Map::CELL_SIZE * (float)m_player_plane_dist) / dist_to_slice);

	int wall_bottom = (projected_slice_height / 2) + m_plane_center;
	//int wall_top = m_plane_center - (projected_slice_height / 2);
	int wall_top = m_plane_height - wall_bottom;

	if(wall_top < 0 ) wall_top = 0;
	if(wall_bottom >= m_plane_height) wall_bottom = m_plane_height - 1;

	Cell cell = m_map->cell_table()[cell_id];

	uint8_t r, g, b;
	if(!cell.texture){
		r = cell.color >> 24;
		g = cell.color >> 16;
		b = cell.color >> 8;

		for(int y = wall_top; y < wall_bottom; y++){
			m_framebuffer->set_pixel(col, y, r, g, b);
		}
	}else{
		column_map_texture(cell.texture, offset, projected_slice_height, col);
	}
};


void RayCaster::render(const Player& player, const Map& map){
	auto dir = player.direction();
	m_viewing_angle = Math::to_deg(dir.angle());

	float ray_angle = m_viewing_angle + (FOV / 2); // start at the left end of the fov arc

	m_render->use_viewport("scene");
	m_framebuffer->clear();

	for(int col = 0; col < m_plane_width; col++){
		if(ray_angle < 0) ray_angle += 360.0f;

		auto px = player.position().x();
		auto py = player.position().y();

		float h_distance  = cast_horizontal_intercept(ray_angle, px, py, map);
		float v_distance = cast_vertical_intercept(ray_angle, px, py, map);

		float distance = std::min(h_distance, v_distance);

		int cell_id;
		int offset; // rey offset within the cell [0, 64)

		if(h_distance < v_distance){
			cell_id = m_hcell_id;
			offset = static_cast<int>(m_hitHx) % Map::CELL_SIZE;
			if(m_draw_rays)
				m_points[col] = {m_hitHx, m_hitHy};
		}else{
			cell_id = m_vcell_id;
			offset = static_cast<int>(m_hitVy) % Map::CELL_SIZE;
			if(m_draw_rays)
				m_points[col] = {m_hitVx, m_hitVy};
		}

		draw_wall_slice(distance, col, cell_id, offset);

		ray_angle -= m_angle_step;
		if(ray_angle >= 360) ray_angle -= 360.0f;

	}

	// update screen
	m_framebuffer->update_texture();
	SDL_RenderCopy(m_render->renderer(), m_framebuffer->texture(), NULL, NULL);

	if(m_draw_rays){
		m_render->use_viewport("map");
		m_render->set_draw_color(0xff, 0xff, 0xff);

		for(const auto& [x, y] : m_points){
			auto[x1, y1] = world2screen2((int)player.position().x(), (int)player.position().y());
			auto[x2, y2] = world2screen2(x, y);
			SDL_RenderDrawLine(m_render->renderer(), x1, y1, x2, y2);
		}
	}
}
