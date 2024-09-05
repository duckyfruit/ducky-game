#include "PlayMode.hpp"
#include "asst_pipeline.hpp"
#include <functional> //for function code
#include <iostream>
#include <fstream>
#include <string>

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>

//NOTES; EACH TILE IN THE PPU TILE_TABLE CORRESPONDS TO 2 BITS

//4 COLORS ALLOWED, BIT 0 AND 1 SPECIFY WHERE EACH COLOR IS ON THE SPRITE

void check_duplicate(std::vector<glm::u8vec4> mycolors, std::vector<glm::u8vec4> pixels, std::array< uint16_t,3840> &backgroundtiles, PPU466 &ppu, int width, int height, int tileindex, int &indexcounter, int backgroundcounter, int paletteindex) //check for duplicate tiles to conserve space
{
	assert((tileindex + indexcounter)< ppu.tile_table.size() );
	assert(backgroundcounter < backgroundtiles.size());

	for(uint32_t y = uint32_t(height*8); y < uint32_t(height*8 + 8); y++)
	{
		for(uint32_t x = uint32_t(width*8); x < uint32_t(width*8 + 8); x++)
		{
			assert((x +  ppu.BackgroundWidth * y) < pixels.size()); //should not exceed this size
			place_bit(pixels[x +  ppu.BackgroundWidth * y], mycolors, x, y, ppu.tile_table[tileindex + indexcounter].bit0, ppu.tile_table[tileindex + indexcounter].bit1);
		}
	}

	for(uint32_t x  = tileindex; x < uint32_t(tileindex + indexcounter); x++)
	{
		if((ppu.tile_table[tileindex + indexcounter].bit0 == ppu.tile_table[x].bit0) && (ppu.tile_table[tileindex + indexcounter].bit1 == ppu.tile_table[x].bit1) && (uint32_t(tileindex + indexcounter) != x))
		{

			//map to this tile
			backgroundtiles[backgroundcounter] = uint16_t(x) | (uint16_t(paletteindex) << 8);

			return;
		}
		
	}
	
	backgroundtiles[backgroundcounter] =uint16_t(tileindex + indexcounter) |  (uint16_t(paletteindex) << 8);

	

}

int read_tiles(std::string path, int paletteindex, int tileindex, int width, int height, PPU466 &ppu, bool paletteloaded,std::array< uint16_t,3840> &backgroundtiles ) //return ended index
{
	std::string txtfile = load_background(path, width*8, height*8);
	std::vector<glm::u8vec4> pixels = get_pixels(txtfile);
	std::vector<glm::u8vec4> mycolors;
	std::vector<glm::u8vec4> colorindex;

	if(!paletteloaded)
	{
		mycolors = find_colors(txtfile);
		ppu.palette_table[paletteindex] = 
		{
			mycolors[0], //completely transparent
			mycolors[1], //yellow (BASE COLOR) --> COLOR 1
			mycolors[2], //black (EYE AND MOUTH COLOR) --> COLOR 2
			mycolors[3], //full opacity
		};
	}
	else
	{
		for(uint32_t x = 0; x < 4; x++)
			mycolors.emplace_back(ppu.palette_table[paletteindex][x]);

	}


	int indexcounter = 0;
	int backgroundcounter = 0;

	for(uint32_t b = 0; b < (uint32_t(height)); b++ )
	{
		for(uint32_t a = 0; a < (uint32_t(width)); a++ )
		{
			check_duplicate(mycolors,pixels, backgroundtiles, ppu,a,b,tileindex,indexcounter, backgroundcounter, paletteindex);
			indexcounter++;
			backgroundcounter++;
		}
	}


	return tileindex + indexcounter;

}

void create_sprite(std::string path, int paletteindex, int tileindex, PPU466 &ppu, bool paletteloaded,bool spriteloaded)
{
	
	std::string txtfile = load_frogs(path);
	std::vector<glm::u8vec4> mycolors;

	if(!paletteloaded)
	{
		mycolors = find_colors(txtfile);
		ppu.palette_table[paletteindex] = 
		{
			mycolors[0], //completely transparent
			mycolors[1], //yellow (BASE COLOR) --> COLOR 1
			mycolors[2], //black (EYE AND MOUTH COLOR) --> COLOR 2
			mycolors[3], //full opacity
		};
	}
	else
	{
		for(uint32_t x = 0; x < 4; x++)
			mycolors.emplace_back(ppu.palette_table[paletteindex][x]);

	}

	if(!spriteloaded)
	{
		std::vector<glm::u8vec4> pixels = get_pixels(txtfile);

		for(uint32_t x = 0; x < 8; x++)
		{
			ppu.tile_table[tileindex].bit0[x] = 0;
			ppu.tile_table[tileindex].bit1[x] = 0;
		}

		for(uint32_t y = 0; y < 8; y++)
		{
			for(uint32_t x = 0; x < 8; x++)
			{
				place_bit(pixels[(8 - (y + 1)) * 8 + x], mycolors, x, y, ppu.tile_table[tileindex].bit0, ppu.tile_table[tileindex].bit1 );
			}
		}
	}

	
}

void place_sprites(int spriteindex, int8_t distx, int8_t disty, int8_t tileindex, int8_t paletteindex,PPU466 &ppu, glm::vec2 &player_at)
{
	ppu.sprites[spriteindex].x = int8_t(player_at.x + distx);
	ppu.sprites[spriteindex].y = int8_t(player_at.y + disty);
	ppu.sprites[spriteindex].index = tileindex;
	ppu.sprites[spriteindex].attributes = paletteindex;

}



PlayMode::PlayMode() {
	//TODO:
	// you *must* use an asset pipeline of some sort to generate tiles.
	// don't hardcode them like this!
	// or, at least, if you do hardcode them like this,
	//  make yourself a script that spits out the code that you paste in here
	//   and check that script into your repository.

	//Also, *don't* use these tiles in your game:

	

	//std::string path = "frogs/blueFrog.png";
	//std::string txtfile = "frog.txt";

	//main character?

	/* character sprites */
	create_sprite("dist/frogs/greenFrogTL.png",7,34, ppu, false, false);
	create_sprite("dist/frogs/greenFrogTR.png",7,35, ppu, true, false);
	create_sprite("dist/frogs/greenFrogBL.png",7,32, ppu, true,false);
	create_sprite("dist/frogs/greenFrogBR.png",7,33, ppu, true, false);
	create_sprite("dist/frogs/highlightL.png",3,40, ppu, false, false);
	create_sprite("dist/frogs/highlightR.png",3,41, ppu, true, false);

	create_sprite("dist/frogs/greenFrogFullBL.png",7,46, ppu, true,false);
	create_sprite("dist/frogs/greenFrogFullBR.png",7,47, ppu, true, false);
	create_sprite("dist/frogs/greenFrogFullTL.png",7,48, ppu, true, false);
	create_sprite("dist/frogs/greenFrogFullTR.png",7,49, ppu, true, false);
	create_sprite("dist/frogs/highlightFullL.png",3,50, ppu, false, false);
	create_sprite("dist/frogs/highlightFullR.png",3,51, ppu, false, false);


	create_sprite("dist/frogs/greenFrogCrouchBL.png",7,52, ppu, true,false);
	create_sprite("dist/frogs/greenFrogCrouchBR.png",7,53, ppu, true, false);


	create_sprite("dist/cookie/cookieBL.png",5,42, ppu, false, false);
	create_sprite("dist/cookie/cookieBR.png",5,43, ppu, true, false);
	create_sprite("dist/cookie/cookieTL.png",5,44, ppu, true, false);
	create_sprite("dist/cookie/cookieTR.png",5,45, ppu, true, false);


	create_sprite("dist/cookie/sugarcookieBL.png",6,54, ppu, false, false);
	create_sprite("dist/cookie/sugarcookieBR.png",6,55, ppu, true, false);
	create_sprite("dist/cookie/sugarcookieTL.png",6,56, ppu, true, false);
	create_sprite("dist/cookie/sugarcookieTR.png",6,57, ppu, true, false);
	//create_sprite("frogs/shadeL.png",4,38, ppu, false, false);
	//create_sprite("frogs/shadeR.png",4,39, ppu, true, false);

	/* character sprites */

	/* background tiles */

	ppu.palette_table[0] = {
		glm::u8vec4(0x46, 0x95, 0xA1, 0xff),
		glm::u8vec4(0x46, 0x95, 0xA1, 0xff),
		glm::u8vec4(0x46, 0x95, 0xA1, 0xff),
		glm::u8vec4(0x46, 0x95, 0xA1, 0xff),
	};

	ppu.palette_table[1] = {
		glm::u8vec4(0xff, 0xff, 0xff, 0x00),
		glm::u8vec4(0xff, 0xff, 0xff, 0x00),
		glm::u8vec4(0xff, 0xff, 0xff, 0x00),
		glm::u8vec4(0xff, 0xff, 0xff, 0x00),
	};






	/*std::array< uint16_t, ppu.BackgroundWidth * ppu.BackgroundHeight> backgroundtiles;
	read_tiles("background/background.png",4,0,ppu.BackgroundWidth,ppu.BackgroundHeight,ppu,false,backgroundtiles);
	ppu.background = backgroundtiles; */
	//read_tiles()
	/*					*/


	//used for the misc other sprites:

}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) { // actual game controls

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.downs += 1;
			left.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.downs += 1;
			right.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.downs += 1;
			up.pressed = true;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	} else if (evt.type == SDL_KEYUP) {
		if (evt.key.keysym.sym == SDLK_LEFT) {
			left.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_RIGHT) {
			right.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_UP) {
			up.pressed = false;
			return true;
		} else if (evt.key.keysym.sym == SDLK_DOWN) {
			down.pressed = false;
			return true;
		}
	}

	return false;
}

bool collission(glm::vec2 p1, glm::vec2 p2, float range)
{
	if( ((p1.x + range > p2.x) && (p1.x - range < p2.x)) && ((p1.y + range > p2.y) && (p1.y - range < p2.y)))
	return true;
	return false;

}
void delete_sprite(int index, PPU466 &ppu)
{
	ppu.sprites[index].x = 0;
	ppu.sprites[index].y = 0;
	ppu.sprites[index].index = 0;
	ppu.sprites[index].attributes = 1; //transparent attribute
}

void PlayMode::update(float elapsed) {
	
	//slowly rotates through [0,1):
	// (will be used to set background color)
	//background_fade += elapsed / 10.0f;
	//background_fade -= std::floor(background_fade);

	//update the cookie list

	constexpr float PlayerSpeed = 50.0f;
	constexpr float range = 10.0f;
	constexpr float eattimer = 0.5f;
	bool ismoving = false;

	cookiecooldown += elapsed;
	

	if(totalcookies > 10)
	exit(0);
	
	
	if (left.pressed)
	{
		player_at.x -= PlayerSpeed * elapsed;
		moveanim += elapsed;
		ismoving = true;
	} 
	if (right.pressed)
	{
		player_at.x += PlayerSpeed * elapsed;
		moveanim += elapsed;
		ismoving = true;
	} 

	if(!ismoving)
	moveanim = 0.95f;

	if( player_at.x < 0) //left bound
	player_at.x = 0;

	if( player_at.x > (ppu.BackgroundWidth/2 *8 - 16)) //right bound
	player_at.x = (ppu.BackgroundWidth/2 *8 - 16);


	for(int i = 0; i< cookievec.size(); i++)
	{
		cookievec[i].y -= cookiespeed * elapsed;
		if(cookievec[i].y <= 0.0f)
		{
			cookievec[i].x = ((rand()) / static_cast <float> (RAND_MAX)) * (ppu.BackgroundWidth/2 *8 - 16);
			cookievec[i].y = float((ppu.BackgroundHeight/2)*8);
		}
	}
		

	if(pointgain)
	{
		timer += elapsed;
		if((timer > eattimer))
		{
			
			cookievec[numcookie[0]].x = ((rand()) / static_cast <float> (RAND_MAX)) * (ppu.BackgroundWidth/2 *8 - 16);
			cookievec[numcookie[0]].y = float((ppu.BackgroundHeight/2)*8);
			numcookie.erase(numcookie.begin());
			timer = 0.0f;
			if(numcookie.empty())
			pointgain = false;
		}
	
	}

	if((rand()%100 == 0) && (cookiecooldown > 10.0f)) //1/100 chance to spawn another cookie
	{
		totalcookies += 1;
		cookiecooldown = 0.0f;
		cookievec.emplace_back(glm::vec2(((rand()) / static_cast <float> (RAND_MAX)) * (ppu.BackgroundWidth/2 *8 - 16) ,float((ppu.BackgroundHeight/2)*8)));
	}


	//if (down.pressed) player_at.y -= PlayerSpeed * elapsed;
	//if (up.pressed) player_at.y += PlayerSpeed * elapsed;

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	//up.downs = 0;
	//down.downs = 0;

	for(int i=0; i < cookievec.size(); i++)
	{
		bool newcookie = true;
		if(collission(player_at, cookievec[i], range))
		{
			for(int j = 0; j < numcookie.size(); j++)
			{
				if(numcookie[j] == i)
				{
					newcookie = false;
				}
			}

			if(newcookie)
			{
				pointgain = true;
				numcookie.emplace_back(i); //should only emplace back 1
			}
			
			
		}
		
	}


}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---
	
	//background color will be some hsv-like fade:

	//tilemap gets recomputed every frame as some weird plasma thing:
	//NOTE: don't do this in your game! actually make a map or something :-)
	for (uint32_t y = 0; y < PPU466::BackgroundHeight; ++y) {
		for (uint32_t x = 0; x < PPU466::BackgroundWidth; ++x) {
			//TODO: make weird plasma thing
			ppu.background[x+PPU466::BackgroundWidth*y] = 0;
		}
	}  

	//background scroll:
	//ppu.background_position.x = int32_t(-0.5f * player_at.x);
	//ppu.background_position.y = int32_t(-0.5f * player_at.y);

	std::vector <uint16_t> frogspriteindex;
	std::vector <uint16_t> playerspriteindex;

	for(int i = 0; i< cookievec.size(); i++)
	{
		
		place_sprites(6 + i * 4, 0, 0, 42, 5, ppu, cookievec[i]);
		place_sprites(7 + i * 4, 7, 0, 43, 5, ppu, cookievec[i]);
		place_sprites(8 + i * 4, 0, 7, 44, 5, ppu, cookievec[i]);
		place_sprites(9 + i * 4, 7, 7, 45, 5, ppu, cookievec[i]);
	}

	if(!pointgain)
	{
		
		//for loop for how ever many cookies to place, maybe one extra every 2 cookies eaten
		//instead of straight at cookie_at there can be a function to calculate the place of the cookie
		//vector of cookie_at
		/*if(cookietype == 0)
		{
			place_sprites(6, 0, 0, 42, 5, ppu, cookie_at);
			place_sprites(7, 7, 0, 43, 5, ppu, cookie_at);
			place_sprites(8, 0, 7, 44, 5, ppu, cookie_at);
			place_sprites(9, 7, 7, 45, 5, ppu, cookie_at);

		}
		else
		{
			place_sprites(6, 0, 0, 54, 6, ppu, cookie_at);
			place_sprites(7, 7, 0, 55, 6, ppu, cookie_at);
			place_sprites(8, 0, 7, 56, 6, ppu, cookie_at);
			place_sprites(9, 7, 7, 57, 6, ppu, cookie_at);
		} */


		if(moveanim > 0.5f) //alternate between crouch and standing animation
		{
			place_sprites(0, 0, 0, 32, 7, ppu, player_at);

			place_sprites(1, 7, 0, 33, 7, ppu, player_at);

			place_sprites(2, 0, 7, 34, 7, ppu, player_at);

			place_sprites(3, 7, 7, 35, 7, ppu, player_at);

			place_sprites(4, 0, 7, 40, 3, ppu, player_at);

			place_sprites(5, 7, 7, 41, 3, ppu, player_at);

			if(moveanim > 1.0f)
			moveanim = 0.0f;
		}
		else
		{
			place_sprites(0, 0, 0, 52, 7, ppu, player_at);

			place_sprites(1, 7, 0, 53, 7, ppu, player_at);

			place_sprites(2, 0, 6, 34, 7, ppu, player_at);

			place_sprites(3, 7, 6, 35, 7, ppu, player_at);

			place_sprites(4, 0, 6, 40, 3, ppu, player_at);

			place_sprites(5, 7, 6, 41, 3, ppu, player_at);
		}

	}
	else
	{
		for(int i = 0; i< cookievec.size(); i++)
		{
			for(int j = 0; j <numcookie.size(); j++)
			if(i == numcookie[j])
			{
				delete_sprite(6 + i *4, ppu);
				delete_sprite(7 + i *4, ppu);
				delete_sprite(8 + i * 4, ppu);
				delete_sprite(9 + i * 4, ppu);
			}
		} 


		place_sprites(0, 0, 0, 46, 7, ppu, player_at);

		place_sprites(1, 7, 0, 47, 7, ppu, player_at);

		place_sprites(2, 0, 7, 48, 7, ppu, player_at);

		place_sprites(3, 7, 7, 49, 7, ppu, player_at);

		place_sprites(4, 0, 7, 50, 3, ppu, player_at);

		place_sprites(5, 7, 7, 51, 3, ppu, player_at);
	}
	
	//--- actually draw ---
	ppu.draw(drawable_size);
}

