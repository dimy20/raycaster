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
	SDL_PumpEvents(); // updates event queue
	while(SDL_PollEvent(&e)){
		if(e.type == SDL_QUIT){
			m_running = false;
		};
		if(e.type == SDL_KEYDOWN){
			m_player.keypressed(e.key.keysym.sym);
		}
	};
}

void Engine::run(){
	m_renderer.set_viewport({0, 0, VIEWPORT_H, VIEWPORT_W}, "map");
	m_renderer.set_viewport({Map::CELL_SIZE * 8, 0, PROJ_PLANE_W, PROJ_PLANE_H}, "scene");
	m_map = Map(&m_renderer);
	m_player = Player(&m_renderer, &m_map, Math::Vec2(96.0f, 224.0f), Math::Vec2(-1.0f, 0.0f));

	m_caster = RayCaster(&m_renderer);
	m_caster.init(PROJ_PLANE_W, PROJ_PLANE_H);

	while(m_running){
		m_renderer.prepare_scene();
		do_input();

		m_map.draw();

		m_player.draw();
		m_caster.render(m_player, m_map);

		m_renderer.present_scene();

	}
}
