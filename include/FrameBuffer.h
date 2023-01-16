#pragma once

#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>

class FrameBuffer{
	public:
		FrameBuffer(SDL_Renderer * renderer, size_t w, size_t h);
		void update_texture();
		SDL_Texture * texture() {return m_texture.get(); };
		void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
		void clear();
	private:
		std::vector<uint8_t> m_buffer;
		size_t m_w;
		size_t m_h;
		std::shared_ptr<SDL_Texture> m_texture;
};
