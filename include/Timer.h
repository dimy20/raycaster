#pragma once

#include <cstdint>
#include <SDL2/SDL_ttf.h>

class Timer{
	public:
		Timer();
		void start();
		void stop();
		void pause();
		void unpause();

		// retrive current time from timer.
		uint32_t get_ticks();

		constexpr bool is_running(){ return m_started; };
		constexpr bool is_paused(){ return m_paused; };

	private:
		uint32_t m_start_ticks; // tick the moment the timer was started.
		uint32_t m_paused_ticks; // tick the moment the timer was paused.
		bool m_paused = false;
		bool m_started = false;
};
