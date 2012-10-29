#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Array2D.cpp"

using namespace std;

//prior decleration
class tile_t;
class sprite_t;

class image_t {
	friend class tile_t;
	friend class sprite_t;
public:
	void load(){
		if( fileLoaded == false ){
			if( texture.loadFromFile( fileLoc ) ){
				sprite.setTexture( texture );
				fileLoaded = true;
			} else {
				assert( 0 ); //error
			}
		}
	}

	void setFileLoc( string input ){
		if( fileLoc.size() == 0 ){ //empty
			fileLoc = input;
		}
	}

	void draw_unsafe( sf::RenderWindow& App, int coor_x, int coor_y ){
		sprite.setPosition( coor_x, coor_y );
		App.draw( sprite );
	}

	virtual void draw( sf::RenderWindow& App, int coor_x, int coor_y ) {
		//load the image it's not loaded
		if( !fileLoaded ) load();

		//just to make sure there is chance that it's not loaded by the load function
		if( fileLoaded ){
			draw_unsafe( App, coor_x, coor_y );
		}
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
	
private:
	bool fileLoaded;
	string fileLoc;

	sf::Sprite sprite;
	sf::Texture texture;
};

class tile_t : public image_t {
public:
	tile_t(){
		divide( 1, 1 ); //only one

		reset2();
	}

	void reset2(){
		width_n = 0;
		height_n = 0;
		width = 0;
		height = 0;

		reset();
	}

	void divide( int w_n, int h_n ){
		width_n = w_n;
		height_n = h_n;

		width = texture.getSize().x / w_n;
		height = texture.getSize().y / h_n;
	}

	void draw( sf::RenderWindow& App, int coor_x, int coor_y, int index_h = 0, int index_v = 0 ) {
		//load the image it's not loaded
		if( !fileLoaded ) load();

		//just to make sure there is chance that it's not loaded by the load function
		if( fileLoaded ){
			sprite.setTextureRect( sf::IntRect( index_h * width, index_v * height, width, height ) );

			draw_unsafe( App, coor_x, coor_y );
		}
	}

private:
	int width_n;
	int height_n;

	int width;
	int height;
};

class sprite_t : public image_t {
public:
	void draw( sf:RenderWindow& App, int coor_x, int coor_y, int index ){
		//load the image it's not loaded
		if( !fileLoaded ) load();

		//just to make sure there is chance that it's not loaded by the load function
		if( fileLoaded ){
			sprite.setTextureRect( sf::IntRect( index * width, 0, width, height ) );

			draw_unsafe( App, coor_x, coor_y );
		}
	}
	
	sprite_t(){
		reset();
	}
	
private:
	int width;
	int height;
	
	

};

struct iFloor {
	tile_t * tile;
	short tile_coor_x;
	short tile_coor_y;

	iFloor( tile_t * _tile = 0, int _x = 0, int _y = 0 ){
		tile = _tile;
		tile_coor_x = _x;
		tile_coor_y = _y;
	}
};

struct iObstacle {
	short x, y;
	tile_t * tile;

	iObstacle( int _x = 0, int _y = 0, tile_t * _tile = 0) {
		x = _x;
		y = _y;
		tile = _tile;
	}
};

struct iEnemy {
	short x, y;
	sprite_t * sprite;
	string enemy_type; //pass to battle map if encountered

	iEnemy( int _x = 0, int _y = 0, sprite_t * _sprite = 0) {
		x = _x;
		y = _y;
		sprite = _sprite;
	}
};

struct iNPC {
	short x, y;
	sprite_t * sprite;
	string vn_name;

	iNPC() {
		x = 0;
		y = 0;
		sprite = 0;
	}
};

/* array entity container */

map< string, iNPC > NPC_array;
Array2D< iFloor > FLOOR_array;
vector< iObstacle > OBS_array;
vector< iEnemy > ENEMY_array;

map< string, tile_t > TILE_array;
map< string, sprite_t > SPRITE_array;

int grid_size = 32;

void floorBlit( sf::RenderWindow& App, int cam_x, int cam_y ) {
	int x, y;
	iFloor * ptr;

	for( x = FLOOR_array.width_(); x--; ){
		for( y = FLOOR_array.height_(); y--; ){
			ptr = &FLOOR_array(x,y);
			
			if( ptr -> tile != 0 ) {
				ptr -> tile -> draw(
					App,
					cam_x + x * grid_size,
					cam_y + y * grid_size,
					ptr -> tile_coor_x,
					ptr -> tile_coor_y
				);
			}
		}
	}
}


/* END */

int main() {
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "1.2" );
	window.setFramerateLimit( 33 ); //max FPS
	
	tile_t p;
	
	p.setFileLoc( "Outside_A2.png" );
	p.load();
	p.divide( 16, 12 );
	
	FLOOR_array.redefine( 10, 10 );

	for( int i = 100; i--; ) {
		FLOOR_array[ i ].tile_coor_x = 9;
		FLOOR_array[ i ].tile_coor_y = 0;
		FLOOR_array[ i ].tile = &p;
	}
	
	while (window.isOpen()) {
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}


		// Clear screen
		window.clear();
		floorBlit( window, 0, 0 );

		// Update the window
		window.display();
	}

	return 0;
}
