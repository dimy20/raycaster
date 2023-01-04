#pragma once

#include <string>
#include <iostream>
#include <cstddef>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Render.h"

class Texture{
	public:
		Texture(Render * render);
		~Texture();
		bool load(const std::string& filename);
		void render(int x, int y, SDL_Rect * clip);
		void set_color(uint8_t r, uint8_t g, uint8_t b);
		void set_alpha(uint8_t alpha);
		void set_blend_mode(SDL_BlendMode mode);
		bool load_from_rendered_text(const std::string& texture_text, SDL_Color text_color);
		void free();
		int constexpr w () const{ return m_w; };
		int constexpr h () const{ return m_h; };
	private:
		Render * m_render = NULL;
		SDL_Texture * m_texture;
		int m_w;
		int m_h;
};
