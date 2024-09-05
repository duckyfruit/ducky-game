#include "PPU466.hpp"
#include "Mode.hpp"

#include <functional> //for function code
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	//some weird background animation:
	float background_fade = 0.0f;

	//player position:
	glm::vec2 player_at = glm::vec2(0.0f);

	std::vector<glm::vec2> cookievec{glm::vec2(0.0f, float((ppu.BackgroundHeight/2)*8))}; 

	std::vector<int> numcookie;

	int totalcookies = 1;

	float cookiecooldown = 0.0f;

	//glm::vec2 cookie_at = glm::vec2(0.0f, float((ppu.BackgroundHeight/2)*8));

	bool pointgain = false;

	float cookiespeed = 50.0f;

	int cookietype = 0;

	int cookiemiss = 0;

	float moveanim = 0.0f;

	float timer = 0.0f;


	// indicies

	//----- drawing handled by PPU466 -----

	PPU466 ppu;

};

