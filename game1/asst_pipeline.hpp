#include "PPU466.hpp"
#include "Mode.hpp"

#include <functional> //for function code
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

#include <vector>
#include <deque>

//import assets --- Source: Assets exercise 1 --

template< typename T >
void read_chunk(std::ifstream &from, std::string const &magic, std::vector< T > *to_);

template< typename T >
void write_chunk(std::string const &magic, std::vector< T > const &from, std::ostream *to_);


std::string load_frogs(std::string path); //path to the frog sprites folder

std::string load_background(std::string path, int width, int height); //path to the claw sprites folder

void transfer_pallette(std::ifstream &readfile, std::string path, PPU466 ppu, int palleteindex, int spriteindex, int spritedimensions, bool palleteloaded);

std::vector<glm::u8vec4> find_colors(std::string path);

std::vector<glm::u8vec4> get_pixels(std::string path);

void place_bit(glm::u8vec4 color, std::vector<glm::u8vec4> myColors, uint32_t x, uint32_t y, std::array<uint8_t, 8Ui64> &bits0, std::array<uint8_t, 8Ui64> &bits1 );

void print_color(glm::u8vec4 color);
