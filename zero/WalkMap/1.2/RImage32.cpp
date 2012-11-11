#ifndef REMAX_TILE_SPRITE_CLASS_32
#define REMAX_TILE_SPRITE_CLASS_32


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "RImage.cpp"

using namespace std;

class tile32_t;
class sprite32_t;

vector<tile32_t *>tile32Pool;
vector<sprite32_t *>sprite32Pool;

typedef unsigned short ushort;

class tile32_t {
public:
	tile32_t(){
		width_n = 1;
		height_n = 1;
		width_s = 0;
		height_s = 0;
	}

	tile32_t( const string& fileLoc , int w, int h ){
		image = get_image( fileLoc );
		width_s = w * 32;
		height_s = h * 32;

		if( !image->loaded() ) image->load();
		width_n = image->width() / width_s;
		height_n = image->height() / height_s;

//		cout << (int)width_s << " " << (int)height_s << " " << (int)width_n << " " << (int)height_n << endl;
	}
	tile32_t( const string& fileLoc ){
		image = get_image( fileLoc );
		if( !image->loaded() ) image->load();

		width_s = image->width() - image->width() % 32;
		height_s = image->height() - image->height() % 32;

		width_n = 1;
		height_n = 1;
	}

	const string& fileLocation(){
		return image->fileLocation();
	}

	~tile32_t(){
		/* Nothing to do here */
	}

	void draw( sf::RenderWindow& App, int x, int y, int sx = 0, int sy = 0 ){
		//prevent segfault
		sx %= width_n;
		sy %= height_n;

		//just being careful
		if( !image -> loaded() ) image -> load();
		image -> setRect( sf::IntRect( sx * width_s, sy * height_s, width_s, height_s ) );
		image -> draw( App, x, y );
	}

	int width32(){
		return width_s / 32;
	}

	int height32(){
		return height_s / 32;
	}

private:
	image_t * image;
	ushort width_n;
	ushort height_n;
	ushort width_s;
	ushort height_s;
}; //12 byte

tile32_t * new_tile32( const string& fileLoc, int ws, int hs ){
	tile32Pool.push_back( new tile32_t( fileLoc, ws, hs ) );
	return tile32Pool.back();
}

tile32_t * new_tile32( const string& fileLoc ){
	tile32Pool.push_back( new tile32_t( fileLoc ) );
	return tile32Pool.back();
}

tile32_t * get_tile32( const string& fileLoc ){
	int i = 0;
	for( ; i < tile32Pool.size(); ++i ){
		if( tile32Pool[i] -> fileLocation() == fileLoc )
			return tile32Pool[i];
	}

	return new_tile32( fileLoc );
}

class sprite32_t {
public:
	sprite32_t( image_t * Image, int width_s_ ){
		image = Image;
		if( !image -> loaded() ) image -> load();

		width_s = width_s_ * 32;
		height_s = image -> height() / 128; //  height / 32 / 4
		width_n = image -> width() / width_s;
	}

	sprite32_t( const string& fileLoc, int width_s_ ){
		image = get_image( fileLoc );
		if( !image -> loaded() ) image -> load();

		width_s = width_s_ * 32;
		height_s = (image -> height() / 128)*32; //  height / 32 / 4
		width_n = image -> width() / width_s;
	}

	sprite32_t( const string& fileLoc ){
		image = get_image( fileLoc );
		if( !image -> loaded() ) image -> load();

		width_n = 1;
		width_s = image -> width() - image->width()%32;
		height_s = (image -> height() / 128)*32;
	}

	void draw( sf::RenderWindow& App, int coor_x, int coor_y, int index = 0, int rot = 2 ) {
		/*
			rot :
			0 -> up
			1 -> right
			2 -> down
			3 -> left
		*/

		//Prevent segFault
		rot %= 4;
		index %= width_n;
//		cout << "sf::IntRect( "<<index * width_s<<","<< rot * height_s<<","<< width_s<<","<< height_s<<" )" << endl;

		image -> setRect( sf::IntRect( index * width_s, rot * height_s, width_s, height_s ) );
		image -> draw( App, coor_x, coor_y );
	}

	int width32(){
		return width_s / 32;
	}

	int height32(){
		return height_s / 32;
	}

	const string& fileLocation(){
		return image -> fileLocation();
	}

private:
	short width_s;
	short height_s;
	int width_n;

	//height_n is always 4

	image_t * image;
}; //12 byte

sprite32_t * new_sprite32( const string& fileLoc, int ws ){
	sprite32Pool.push_back( new sprite32_t( fileLoc, ws ) );
	return sprite32Pool.back();
}

sprite32_t * new_sprite32( const string& fileLoc ){
	sprite32Pool.push_back( new sprite32_t( fileLoc ) );
	return sprite32Pool.back();
}

sprite32_t * get_sprite32( const string& fileLoc ){
	int i = 0;
	for( ; i < sprite32Pool.size(); ++i ){
		if( sprite32Pool[i] -> fileLocation() == fileLoc )
			return sprite32Pool[i];
	}

	return new_sprite32( fileLoc );
}

/*
//For Testing

int main( int argc, char **argv ){
	sf::RenderWindow window( sf::VideoMode( 720, 480 ), "1.2" );
	window.setFramerateLimit( 30 ); //max FPS

//	new_tile32( "Outside_A2.png", 5, 5 );

	tile32_t * p = get_tile32( "Outside_A2.png" );
	sprite32_t * p2 = new_sprite32( "1333727218296.jpg" );

	int ii = 0;

	while ( window.isOpen() ){
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear screen
		window.clear();

		//draw everything
//		p->draw( window, 0,0, 1,0 );
		p2->draw( window, ii,0, 0 ,1 );

		ii -= 5;

		// Update the window
		window.display();
	}

	return 0;
}

*/

#endif
