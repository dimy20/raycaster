#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(SDL_Renderer * renderer, size_t w, size_t h) : m_w(w), m_h(h) {
	m_buffer.resize(m_w * m_h * 4);
	SDL_Texture * texture;

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, 
										  SDL_TEXTUREACCESS_STREAMING, 
										  m_w, m_h);
    if (!texture) {
        std::cerr << "Failed to create framebuffer texture : " << SDL_GetError() << "\n";
		exit(1);
    }

	m_texture = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
	memset(m_buffer.data(), 0xff, m_buffer.size());
}

void FrameBuffer::update_texture(){
	int pitch = m_w * 4;
	int err;
	void * pixels = reinterpret_cast<void *>(m_buffer.data());

	//SDL_LockTexture(texture());
    err = SDL_UpdateTexture(m_texture.get(), NULL, pixels, pitch);
	//SDL_UnlockTexture(texture());

	if(err < 0){
		std::cerr << "Failed to update framebuffer: " << SDL_GetError() << "\n";
		exit(1);
	}
};

void FrameBuffer::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b){
	int offset = ((y * m_w) + x) * 4;
	m_buffer[offset] = r;
	m_buffer[offset + 1] = g;
	m_buffer[offset + 2] = b;
};

void FrameBuffer::clear(){ memset((void*)m_buffer.data(), 0, m_buffer.size()); }
