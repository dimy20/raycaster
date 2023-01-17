#include "Engine.h"
void Engine::init(){
	int err = SDL_Init(SDL_INIT_VIDEO);
	if(err < 0){
		std::cout << "failed to initalize sdl" << std::endl;
		exit(EXIT_FAILURE);
	}

	err = IMG_Init(IMG_INIT_PNG);
	if(!(err & IMG_INIT_PNG)){
		std::cerr << "Error: " << IMG_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	if(TTF_Init() == -1){
		std::cerr << "Error: " << TTF_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	m_window = Window("ray caster", (size_t)1024, (size_t)728, 0, 0);
	m_renderer.init(m_window);
}

void Engine::do_input(){
	SDL_Event e;
	SDL_PumpEvents();
	while(SDL_PollEvent(&e)){
		switch(e.type){
			case SDL_QUIT:
				m_running = false;
				break;
			default:
				break;
		}
	};
}

void show_fps(const std::string& s){

}

void Engine::run(){
	m_renderer.set_viewport({PROJ_PLANE_W - VIEWPORT_W, 0, VIEWPORT_H, VIEWPORT_W}, "map");
	m_renderer.set_viewport({0, 0, PROJ_PLANE_W, PROJ_PLANE_H}, "scene");

	m_map = Map(&m_renderer);
	m_player = Player(this, &m_renderer, &m_map, Math::Vec2(96.0f, 224.0f), Math::Vec2(-1.0f, 0.0f));

	FrameBuffer framebuffer(m_renderer.renderer(), PROJ_PLANE_W, PROJ_PLANE_H);

	m_caster = RayCaster(&m_renderer, &m_map, &framebuffer);
	m_caster.init(PROJ_PLANE_W, PROJ_PLANE_H);

	int n;
	m_keyboard_state = SDL_GetKeyboardState(&n);

	framebuffer.update_texture();

	TTF_Font * gfont = TTF_OpenFont("American Captain.ttf", 28);
	SDL_Color color = {0xff, 0xff, 0xff};


	uint32_t old_time, now_time;
	while(m_running){
		m_renderer.prepare_scene();
		do_input();

		old_time = SDL_GetTicks();
		m_player.update();
		m_player.draw();
		m_caster.render(m_player, m_map);
		now_time = SDL_GetTicks();

		m_renderer.use_viewport("scene");

		std::string fps = "fps : " + std::to_string((now_time - old_time) / 1000);
		SDL_Surface * srf = TTF_RenderText_Solid(gfont, fps.c_str(), color);
		SDL_Texture * text = SDL_CreateTextureFromSurface(m_renderer.renderer(), srf);

		SDL_FreeSurface(srf);

		SDL_Rect dest = {0, PROJ_PLANE_H - 100, 100, 80};
		SDL_RenderCopy(m_renderer.renderer(), text, NULL, &dest);

		m_renderer.present_scene();
	}
}
