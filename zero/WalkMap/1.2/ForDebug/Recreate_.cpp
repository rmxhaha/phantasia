#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Array2D.cpp"

using namespace std;

const int gridsize = 32;
const int FramePerSecond = 30;
int camera_x = 0;
int camera_y = 0;

/* Prior Decleration */
class image_t;
class tile_t;
class sprite_t;

class image_t {
public:
	image_t(){
		fileLoaded = false;
	}

	image_t( const string& FL ){
		fileLoc = FL;
		fileLoaded = false;
	}

	~image_t(){

	}

	void load(){
		if( fileLoaded == false ){

			if( !texture.loadFromFile( fileLoc ) ){
				cout << "ERR" << endl;
			}

			sprite.setTexture( texture );
			fileLoaded = true;
		}
	}

	void setFileLoc( const string& input ){
		if( fileLoc.size() == 0 ){ //empty
			fileLoc = input;
		}
	}

	void draw_unsafe( sf::RenderWindow& App, int coor_x, int coor_y ){
		sprite.setPosition( coor_x, coor_y );
		App.draw( sprite );
	}

	void draw( sf::RenderWindow& App, int coor_x, int coor_y ){
		//load the image it's not loaded
		if( !fileLoaded ) load();

		//just to make sure there is chance that it's not loaded by the load function
		if( fileLoaded ){
			draw_unsafe( App, coor_x, coor_y );
		}
	}

	void setRect( sf::IntRect Rect ){
		sprite.setTextureRect( Rect );
	}

	void reset(){
		fileLoaded = false;
		fileLoc.resize( 0 );
		unload();
	}

	void unload(){
		if( fileLoaded ){
			texture.loadFromFile(""); //release it
			fileLoaded = false;
		}
	}

	bool loaded(){
		return fileLoaded;
	}

	const string& fileLocation(){
		return fileLoc;
	}

	int width(){
		return texture.getSize().x;
	}

	int height(){
		return texture.getSize().y;
	}
private:
	bool fileLoaded;
	string fileLoc;

	sf::Sprite sprite;
	sf::Texture texture;
};

/* Pool for better access */
vector<image_t> imagePool;

image_t& new_image( const string& fileLoc ){
	imagePool.push_back( image_t( fileLoc ) );
	return imagePool.back();
}

image_t& get_image( const string& fileLoc ) {
	int i = 0;
	for( ; i < imagePool.size(); ++i ){
		if( imagePool[i].fileLocation() == fileLoc )
			return imagePool[i];
	}
	return new_image( fileLoc );
}

void flush_image(){
	//erase all
	for( int i = imagePool.size(); i--; ){
		imagePool[i].unload();
	}
}


class tile_t {
public:
	tile_t( image_t& Image, int dx = 1, int dy = 1 ) {
		width = 0;
		height = 0;
		width_n = 0;
		height_n = 0;

		image = &Image;
		if( !image -> loaded() ) image -> load();
		divide( dx, dy );
	}

	tile_t( const string& FL, int dx = 1, int dy = 1 ){
		width = 0;
		height = 0;
		width_n = 0;
		height_n = 0;

		image = &get_image( FL );
		image -> load();
		divide( dx, dy );
	}

	void divide( int _nx, int _ny ){
		width_n = _nx;
		height_n = _ny;
		width = image -> width() / _nx;
		height = image -> height() / _ny;
	}

	//make sure image is loaded
	void draw( sf::RenderWindow& App, int _x, int _y, int index_x, int index_y ){
		index_x %= width_n;
		index_y %= height_n;
		if( !image -> loaded() ) image -> load();

		image -> setRect( sf::IntRect(index_x * width, index_y * height, width, height ) );
		image -> draw( App, _x, _y );
	}

	void draw( sf::RenderWindow& App, int _x, int _y ){
		if( !image -> loaded() ) image -> load();

		image -> setRect( sf::IntRect( 0, 0, width, height ) );
		image -> draw( App, _x, _y );
	}

	const string& fileLocation(){
		return image -> fileLocation();
	}

private:
	image_t * image;
	int width;
	int height;
	int width_n;
	int height_n;
};

class sprite_t {
public:
	sprite_t( image_t& Image, int _width_n ){
		image = &Image;
		if( !image -> loaded() ) image -> load();

		this -> width = image -> width() / width_n;
		this -> width_n = width_n;
	}

	sprite_t( const string& FL, int width_n ){
		image = &get_image( FL );
		image -> load();


		this -> width = image -> width() / width_n;
		this -> width_n = width_n;
	}

	void draw( sf::RenderWindow& App, int coor_x, int coor_y, int index = 0 ) {
		index %= width_n;

		image -> setRect( sf::IntRect( index * width, 0, width, image -> height() ) );
		image -> draw( App, coor_x, coor_y );
	}

	const string& fileLocation(){
		return image -> fileLocation();
	}

private:
	int width;
	int width_n;

	image_t * image;
};


/* Pool for better access */
vector<tile_t> tilePool;
vector<sprite_t> spritePool;

tile_t& new_tile( string fileLoc, int width_n, int height_n ){
	tilePool.push_back( tile_t( fileLoc, width_n, height_n ) );
	return tilePool.back();
}

sprite_t& new_sprite( const string& fileLoc, int width_n ){
	spritePool.push_back( sprite_t( fileLoc, width_n ) );
	return spritePool.back();
}

tile_t& get_tile( const string& fileLoc ) {
	int i = 0;
	for( ; i < tilePool.size(); ++i ){
		if( tilePool[i].fileLocation() == fileLoc )
			return tilePool[i];
	}

	return new_tile( fileLoc, 1, 1 );
}

sprite_t& get_sprite( const string& fileLoc ) {
	int i = 0;
	for( ; i < spritePool.size(); ++i ){
		if( spritePool[i].fileLocation() == fileLoc )
			return spritePool[i];
	}

	return new_sprite( fileLoc, 1 );
}


struct Floor {
	//please keep in mind that I am too lazy to make this a class
	//so a struct will do
	//the variables inside this struct shouldn't be changed from outside

	tile_t * tile;
	int coor_x;
	int coor_y;
	int index_x;
	int index_y;

	Floor( string fileLoc, int index_x = 0, int index_y = 0 ) {
		this -> coor_x = 0;
		this -> coor_y = 0;
		this -> index_x = index_x;
		this -> index_y = index_y;

		this -> tile = &get_tile( fileLoc );
	}

	void draw( sf::RenderWindow& App ) {
		tile -> draw(
			App,
			::camera_x + coor_x * gridsize,
			::camera_y + coor_y * gridsize,
			index_x,
			index_y
		);
	}
};

struct Obstacle {
	sprite_t * sprite;
};

int main() {
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "1.2" );
	window.setFramerateLimit( FramePerSecond ); //max FPS

	cout << sizeof( sprite_t ) << " " << sizeof( tile_t ) << endl;

	sprite_t p = new_sprite( "bbl.png", 15 );
	sprite_t& p2 = new_sprite( "Outside_A2.png", 16 );

//	tile_t& p = new_tile( "Outside_A2.png",16,12 );

	int ii = 0;
	const sf::Color shiro( 0xff,0xff,0xff );

	while ( window.isOpen() ) {
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}


		// Clear screen
		window.clear( shiro );

		//draw everything
		p.draw( window, 0,0, ii * 20 / FramePerSecond );
//		spritePool[1].draw( window, 0,0, ii );

		ii++;

//		tilePool[0].draw( window, 85,0, 8, 0 );
//		p.draw( window,85, 0, 8, 0 );


		// Update the window
		window.display();
	}

	return 0;
}
