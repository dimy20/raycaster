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

void Engine::run(){
	m_renderer.set_viewport({0, 0, VIEWPORT_H, VIEWPORT_W}, "map");
	m_renderer.set_viewport({Map::CELL_SIZE * 8, 0, PROJ_PLANE_W, PROJ_PLANE_H}, "scene");
	m_map = Map(&m_renderer);
	m_player = Player(this, &m_renderer, &m_map, Math::Vec2(96.0f, 224.0f), Math::Vec2(-1.0f, 0.0f));

	FrameBuffer framebuffer(m_renderer.renderer(), PROJ_PLANE_W, PROJ_PLANE_H);

	m_caster = RayCaster(&m_renderer, &m_map, &framebuffer);
	m_caster.init(PROJ_PLANE_W, PROJ_PLANE_H);

	SDL_Surface * srfc = IMG_Load("Wolf3d.png");
	if(!srfc){
		std::cerr << "Failed to load texture: " << SDL_GetError() << "\n";
		exit(1);
	}

	int n;
	m_keyboard_state = SDL_GetKeyboardState(&n);



	framebuffer.update_texture();
	while(m_running){
		m_renderer.prepare_scene();
		do_input();

		m_player.update();
		m_player.draw();

		m_map.draw();

		m_caster.render(m_player, m_map);

		m_renderer.present_scene();
		SDL_Delay(16);

	}
}
