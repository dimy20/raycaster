#include "Dot.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 728

Dot::Dot(){
	m_posx = m_posy = m_velx = m_vely = 0;
};

void Dot::handle_event(SDL_Event& e){
	 //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){

            case SDLK_UP: m_vely -= DOT_VEL; break;
            case SDLK_DOWN: m_vely += DOT_VEL; break;
            case SDLK_LEFT: m_velx -= DOT_VEL; break;
            case SDLK_RIGHT: m_velx += DOT_VEL; break;
        }
	}else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        switch( e.key.keysym.sym ){
            case SDLK_UP: m_vely += DOT_VEL; break;
            case SDLK_DOWN: m_vely -= DOT_VEL; break;
            case SDLK_LEFT: m_velx += DOT_VEL; break;
            case SDLK_RIGHT: m_velx -= DOT_VEL; break;
        }
	}
}

void Dot::move(){
	m_posx += m_velx;
	m_posy += m_vely;

	if((m_posx < 0) || (m_posx + WIDTH > SCREEN_WIDTH)){
		m_posx -= m_velx;
	};

	if((m_posy < 0) || (m_posy + HEIGHT > SCREEN_HEIGHT)){
		m_posy -= m_vely;
	};
}
