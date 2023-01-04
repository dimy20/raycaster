#pragma once

class Dot{
	public:
		static const int WIDTH = 20;
		static const int HEIGHT = 20;
		static const int DOT_VEL = 10;

		Dot();
		void handle_event(SDL_Event& e);
		void move();
		void render();

	private:
		int m_posx;
		int m_posy;
		int m_velx;
		int m_vely;
};

