#include "Texture.h"

TTF_Font * g_font = NULL;

Texture::Texture(Render * render){
	m_texture = nullptr;
	m_w = 0;
	m_h = 0;
	m_render = render;
}

void Texture::free(){
	if(m_texture){
		SDL_DestroyTexture(m_texture);
		m_w = m_h = 0;
		m_texture = NULL;
	}
}

Texture::~Texture(){ free(); }

bool Texture::load(const std::string& filename){
	assert(m_render != NULL);

	SDL_Texture * texture = nullptr;
	SDL_Surface * surface = IMG_Load(&filename[0]);


	SDL_Surface * new_s = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);

	if(surface == NULL){
		std::cerr << "Failed to load texture " << filename << ": " << SDL_GetError() << "\n";
		return false;
	}

	uint32_t pixel = SDL_MapRGB(surface->format, 0xff, 0, 0xfc);
	printf("%d\n", surface->format->BitsPerPixel);
	SDL_SetColorKey(surface, SDL_TRUE, pixel);

	texture = SDL_CreateTextureFromSurface(m_render->renderer(), new_s);

	if(texture == NULL){
		std::cerr << "Failed to create texture" << filename << ": " << SDL_GetError() << "\n";
		return false;
	}

	m_w = surface->w;
	m_h = surface->h;
	m_texture = texture;

	SDL_FreeSurface(surface);

	return true;
}

void Texture::render(int x, int y, SDL_Rect * clip){
	assert(m_texture != NULL && m_render != NULL);

	SDL_Rect quad = {x, y, m_w, m_h};

	if(clip != NULL){
		quad.w = clip->w;
		quad.h = clip->h;
	};

	int err = SDL_RenderCopy(m_render->renderer(), m_texture, clip, &quad);
	if(err != 0){
		std::cerr << "Error: " << SDL_GetError() << "\n";
	}
	//SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
	//SDL_RenderDrawRect(renderer, &quad);
}

void Texture::set_color(uint8_t r, uint8_t g, uint8_t b){
	assert(m_texture != NULL);
	// multiplies the current color by x/255, x being to color passed as argument.
	int err = SDL_SetTextureColorMod(m_texture, r, g, b);
	if(err != 0){
		std::cerr << "Error: " << SDL_GetError() << "\n";
	}
};

void Texture::set_blend_mode(SDL_BlendMode mode){
	assert(m_texture != NULL);
	SDL_SetTextureBlendMode(m_texture, mode);
}

void Texture::set_alpha(uint8_t alpha){
	assert(m_texture != NULL);
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

bool Texture::load_from_rendered_text(const std::string& texture_text, SDL_Color text_color){
	free();
	g_font = TTF_OpenFont("test.ttf", 28);
	if(g_font == NULL){
		std::cerr << "Error TTF: " << TTF_GetError() << "\n";
		return false;
	}

	SDL_Surface * text_surface = TTF_RenderText_Solid(g_font, texture_text.c_str(), text_color);
	if(text_surface == NULL){
		std::cerr << "Error: " << TTF_GetError() << "\n";
		return false;
	}

	m_texture = SDL_CreateTextureFromSurface(m_render->renderer(), text_surface);
	if(m_texture == NULL){
		std::cerr << "Error: " << SDL_GetError() << "\n";
		return false;
	}

	m_w = text_surface->w;
	m_h = text_surface->h;
	SDL_FreeSurface(text_surface);
	return true;
}
