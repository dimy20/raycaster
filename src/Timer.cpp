#include "Timer.h"

Timer::Timer(){
	m_paused = false;
	m_started = false;
	m_start_ticks = 0;
	m_paused_ticks = 0;
}

void Timer::start(){
	m_started = true;
	m_paused = false;

	m_start_ticks = SDL_GetTicks();
	m_paused_ticks = 0;
};

void Timer::stop(){
	m_started = false;
	m_paused = false;
	m_start_ticks = 0;
	m_paused_ticks = 0;
};

void Timer::pause(){
	if(m_started && !m_paused){
		m_paused = true;
		m_paused_ticks = SDL_GetTicks() - m_start_ticks;
		m_start_ticks = 0;
	}
};

void Timer::unpause(){
	if(m_started && m_paused){
		m_paused = false;
		m_start_ticks = SDL_GetTicks() - m_paused_ticks;
		m_paused_ticks = 0;
	}
};

uint32_t Timer::get_ticks(){
	if(!m_started ) return 0;
	return m_paused ? m_paused_ticks : SDL_GetTicks() - m_start_ticks;
};

