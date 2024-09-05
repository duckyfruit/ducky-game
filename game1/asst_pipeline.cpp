#include "asst_pipeline.hpp"
#include "load_save_png.hpp"

#include <functional> //for function code
#include <iostream>
#include <fstream>
#include <string>

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>

//Assets 2 code 
template< typename T >
void write_chunk(std::string const &magic, std::vector< T > const &from, std::ofstream *to_) //write all the data into binary
{
//!todo{
	assert(magic.size() == 4);
	assert(to_);
	auto &to = *to_;

	//write magic value:
	//TODO: write magic value (using a single call to 'to.write')
	to.write(reinterpret_cast< char const * >(&magic), 4);
	//write size (in bytes):

	to << " ";

	uint32_t size = uint32_t(from.size() * sizeof(T));
	to.write(reinterpret_cast< char const * >(&size), 4);
	//to.write(reinterpret_cast< char const *>(&size), sizeof(size));

	to << " \n";
	//write data:
	//reads it in ARGB order
	//to.write(reinterpret_cast < char const * > (from.data()),from.size()* sizeof(T));
	for(int x = 0; x < from.size(); x++)
	{

		to << from[x].w;
		to << " ";
		to << from[x].x;
		to << " ";
		to << from[x].y;
		to << " ";
		to << from[x].z;
		to << " \n"; 

	} 
	
	//TODO: write data to 'to' (using a single call to 'to.write')
	//to.write(reinterpret_cast < char const * >(from.data()), from.size()* sizeof(T));
//!}
}

template< typename T >
void read_chunk(std::ifstream &from, std::string const &magic, std::vector< T > *to_) //not using atm
{
	assert(to_);
	auto &to = *to_;

	struct ChunkHeader {
		char magic[4] = {'\0', '\0', '\0', '\0'};
		uint32_t size = 0;
	};
	static_assert(sizeof(ChunkHeader) == 8, "header is packed");

	ChunkHeader header;
	if (!from.read(reinterpret_cast< char * >(&header), sizeof(header))) {
		throw std::runtime_error("Failed to read chunk header");
	}
	if (std::string(header.magic,4) != magic) {
		throw std::runtime_error("Unexpected magic number in chunk");
	}

	if (header.size % sizeof(T) != 0) {
		throw std::runtime_error("Size of chunk not divisible by element size");
	}

	to.resize(header.size / sizeof(T));
	if (!from.read(reinterpret_cast< char * >(&to[0]), to.size() * sizeof(T))) {
		throw std::runtime_error("Failed to read chunk data.");
	}
}

void transfer_pallette(std::ifstream &readfile, std::string path, PPU466 ppu, int palleteindex, int spriteindex, int spritedimensions, bool palleteloaded)
{
	//put the colors in gthe correct pallette index
	
	std::vector<glm::u8vec4> pixels = get_pixels(path);

	if(!palleteloaded) //need to load in a new color pallete 
	{
		std::vector<glm::u8vec4> myColors = find_colors(path);
		
		ppu.palette_table[7] = 
		{
		glm::u8vec4(0x00, 0x00, 0x00, 0x00), //completely transparent
		glm::u8vec4(2, 28, 21, 255), //yellow (BASE COLOR) --> COLOR 1
		glm::u8vec4(96, 173, 202, 255), //black (EYE AND MOUTH COLOR) --> COLOR 2
		glm::u8vec4(158, 222, 247, 255), //full opacity
		};

	}

	assert(spritedimensions%8 == 0); //should be in multiples of 8
	uint32_t dim = 8 * (spritedimensions/8);

	

	for(uint32_t y = 0; y < dim; y++)
	{
		for(uint32_t x = 0; x < dim; x++)
		{
			//place_bit(pixels[y * dim + x], ppu.palette_table[palleteindex], ppu, spriteindex, x, y);
		}
	}


}

std::vector<glm::u8vec4> find_colors(std::string path) //returns a vector of 4 RGBA colors
{
	std::ifstream readfile;
	readfile.open(path); //open the file path
	std::uint8_t str; 
	std::string header;
	std::vector<glm::u8vec4> myColors; //vector of the png information
	glm::u8vec4 currColor; //the current color
	currColor.w = 0;
	bool duplicate = false; //check if there is a duplicate color
	int count = 0; //keep track of the # of values seen

	std::getline(readfile,header);

	while (readfile >> str)
	{
		if(myColors.size() >= 4) //leave the loop if we already have our 4 colors
		break;

		assert(myColors.size() <= 4); //for testing to make sure of no redundancy
		assert(count <= 4); //count should not exceed 4 --> for Red, Blue, Green, Alpha Channel

		if(count > 3) //should be 4 elements in the array
		{
			for(uint32_t x = 0; x < myColors.size(); x++)
			{
				if(currColor == myColors[x])
					duplicate = true;
			}  
			if(!duplicate)
			{
				glm::u8vec4 newColor = currColor;
				myColors.emplace_back(newColor);
			} 
			
			currColor.w = str;
			duplicate = false;
			count = 0;
		}
		else if(count == 0)
		currColor.w = str;
		else if(count == 1)
		currColor.x = str;
		else if(count == 2)
		currColor.y = str;
		else if(count == 3)
		currColor.z = str;
		else
		assert(1 == 0);

		count ++;
			
	} 
	readfile.close();
	
	while(myColors.size() < 4) //leave the loop if we already have our 4 colors
		myColors.emplace_back(glm::u8vec4(0,0,0,0));

	return myColors;
} 

std::vector<glm::u8vec4> get_pixels(std::string path)
{
	std:: ifstream readfile;
	readfile.open(path); //open the file path
	std::uint8_t str; 
	std::string header;
	std::vector<glm::u8vec4> pixels; //vector of the png information
	glm::u8vec4 currColor; //the current color
	currColor.w = 0;
	int count = 0; //keep track of the # of values seen

	std::getline(readfile,header);

	while (readfile >> str)
	{
		assert(count <= 4); //count should not exceed 4 --> for Red, Blue, Green, Alpha Channel

		if(count > 3)
		{
			glm::u8vec4 newColor = currColor;
			pixels.emplace_back(newColor);
			currColor.w = str;
			count = 0;
		}
		else if(count == 0)
		currColor.w = str;
		else if(count == 1)
		currColor.x = str;
		else if(count == 2)
		currColor.y = str;
		else if(count == 3)
		currColor.z = str;
		else
		assert(1 == 0);

		count ++;
		
	}
	glm::u8vec4 newColor = currColor;
	pixels.emplace_back(newColor); //place back the last pixel

	readfile.close();

	return pixels;

}

void place_bit(glm::u8vec4 color, std::vector<glm::u8vec4> myColors, uint32_t x, uint32_t y, std::array<uint8_t, 8Ui64> &bits0, std::array<uint8_t, 8Ui64> &bits1 )
{
	
	if(color == myColors[0])
	{
		bits0[y] = (bits0[y] & (~(1 << (x + 1))));
		bits1[y] = (bits1[y] & (~(1 << (x + 1))));
		//ppu.tile_table[spriteindex].bit0[y] = (ppu.tile_table[spriteindex].bit0[y] & (~(1 << (x + 1))));
		//ppu.tile_table[spriteindex].bit1[y] = (ppu.tile_table[spriteindex].bit0[y] & (~(1 << (x + 1))));
	}

	else if(color == myColors[1])
	{
		bits0[y] = (bits0[y] | ((1 << (x + 1))));
		bits1[y] = (bits1[y] & (~(1 << (x + 1))));
		
		//ppu.tile_table[spriteindex].bit0[y] = (ppu.tile_table[spriteindex].bit0[y] & (~(1 << (x + 1))));
		//ppu.tile_table[spriteindex].bit1[y] = (ppu.tile_table[spriteindex].bit0[y] | ((1 << (x + 1))));
	}

	else if(color == myColors[2])
	{
		bits0[y] = (bits0[y] & (~(1 << (x + 1))));
		bits1[y] = (bits1[y] | ((1 << (x + 1))));
		
		//ppu.tile_table[spriteindex].bit0[y] = (ppu.tile_table[spriteindex].bit0[y] | ((1 << (x + 1))));
		//ppu.tile_table[spriteindex].bit1[y] = (ppu.tile_table[spriteindex].bit0[y] | ((1 << (x + 1))));
	}

	else if(color == myColors[3])
	{

		bits0[y] = (bits0[y] | ((1 << (x + 1))));
		bits1[y] = (bits1[y] | ((1 << (x + 1))));
		//ppu.tile_table[spriteindex].bit0[y] = (ppu.tile_table[spriteindex].bit0[y] | ((1 << (x + 1))));
		//ppu.tile_table[spriteindex].bit1[y] = (ppu.tile_table[spriteindex].bit0[y] | (~(1 << (x + 1))));
	}

	else
		assert(1==0);


}

void print_color(glm::u8vec4 color) //printing colors for debugging purpouses
{
	std::cout << uint32_t(color.w);
	std::cout << " ";
	std::cout << uint32_t(color.x);
	std::cout << " ";
	std::cout << uint32_t(color.y);
	std::cout << " ";
	std::cout << uint32_t(color.z);
	std::cout << " \n";
}

std::string load_frogs(std::string path)//load the frogs function
{
	//Stack Overflow --- https://stackoverflow.com/questions/13035674/how-to-read-a-file-line-by-line-or-a-whole-text-file-at-once ---
	
	std::vector<glm::u8vec4> myText; //vector of the png information
	std::vector<glm::u8vec4> myColors; //vector of the png information
	std::string textname = "frog.txt";
    glm::uvec2 size;
    size.x = 8;
    size.y = 8;
	//std::string str; //string to read line by line the png file
	//std::ifstream MyReadFile(path); //start reading the path file
    OriginLocation start = UpperLeftOrigin;
	std::ofstream myfile;
	
  	myfile.open (textname);
	
    load_png(path, &size, &myText, start);
	write_chunk("frog",myText,&myfile);
	myfile.close();

	return textname;
	
}

std::string load_background(std::string path, int width, int height)
{
	std::vector<glm::u8vec4> myText; //vector of the png information
	std::vector<glm::u8vec4> myColors; //vector of the png information
	std::string textname = "background.txt";
    glm::uvec2 size;
    size.x = width;
    size.y = height;
	//std::string str; //string to read line by line the png file
	//std::ifstream MyReadFile(path); //start reading the path file
    OriginLocation start = UpperLeftOrigin;
	std::ofstream myfile;
	
  	myfile.open (textname);
	
    load_png(path, &size, &myText, start);
	write_chunk("back",myText,&myfile);
	myfile.close();

	return textname;
	
}