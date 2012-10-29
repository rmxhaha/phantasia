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

/* Pool for better access */
vector<image_t *> imagePool;
vector<tile_t *> tilePool;
vector<sprite_t *> spritePool;

class image_t {
public:
	image_t(){
		fileLoaded = false;
	}

	image_t( string FL ){
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

	void setFileLoc( string input ){
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

image_t * get_image( string fileLoc ){
	int i = 0;
	for( ; i < imagePool.size(); ++i ){
		if( imagePool[i] -> fileLocation() == fileLoc )
			break;
	}

	if( i == imagePool.size() ){
		imagePool.push_back( new image_t( fileLoc ) );
		return imagePool.back();
	} else {
		return imagePool[i];
	}
}

void flush_image(){
	//erase all
	for( int i = imagePool.size(); i--; ){
		imagePool[i] -> unload();
	}
}

class tile_t {
public:
	tile_t( image_t * Image, int dx = 1, int dy = 1 ) {
		width = 0;
		height = 0;
		width_n = 0;
		height_n = 0;

		image = Image;
		if( !image -> loaded() ) image -> load();
		divide( dx, dy );
	}

	tile_t( string FL, int dx = 1, int dy = 1 ){
		width = 0;
		height = 0;
		width_n = 0;
		height_n = 0;

		image = get_image( FL );
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
	sprite_t( image_t * Image, int _width_n ){
		image = Image;
		if( !image -> loaded() ) image -> load();

		this -> width = image -> width() / width_n;
		this -> width_n = width_n;
	}

	sprite_t( string FL, int width_n ){
		image = get_image( FL );
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

tile_t * new_tile( string fileLoc, int width_n, int height_n ) {
	tilePool.push_back( new tile_t( fileLoc, width_n, height_n ) );
	return tilePool.back();
}

sprite_t * new_sprite( string fileLoc, int width_n ) {
	spritePool.push_back( new sprite_t( fileLoc, width_n ) );
	return spritePool.back();
}

tile_t * get_tile( string fileLoc ) {
	int i = 0;
	for( ; i < tilePool.size(); ++i ){
		if( tilePool[i] -> fileLocation() == fileLoc )
			return tilePool[i];
	}

	return 0;
}

sprite_t * get_sprite( string fileLoc ) {
	int i = 0;
	for( ; i < spritePool.size(); ++i ){
		if( spritePool[i] -> fileLocation() == fileLoc )
			return spritePool[i];
	}

	return 0;
}

void clear_tile(){
	for( int i = tilePool.size(); i--; ) {
		//there is no need to clear image

		//delete the resources
		delete tilePool[i];
	}

	//clear the pointer
	tilePool.clear();
}

void clear_sprite(){
	for( int i = spritePool.size(); i--; ) {
		//there is no need to clear image

		//delete the resources
		delete spritePool[i];
	}

	//clear the pointer
	spritePool.clear();

}


class Floor {
public:
	Floor( int map_width_, int map_height_ ) {
		map_width = map_width_;
		map_height = map_height_;

		FloorBoxes.resize( map_width_ * map_height_ );
	}

	Floor( const string& fileLoc ) {
		loadData( fileLoc );
	}

	~Floor(){
	}

	void draw( sf::RenderWindow& App ) {
		assert( map_width * map_height == FloorBoxes.size() );

		int coor_x, coor_y, index;
		for( coor_y = 0; coor_y < map_height; ++coor_y ){
			for( coor_x = 0; coor_x < map_width; ++coor_x ){
				FloorBoxes[ index ].tile -> draw(
					App,
					::camera_x + coor_x * gridsize,
					::camera_y + coor_y * gridsize,
					FloorBoxes[ index ].index_x,
					FloorBoxes[ index ].index_y
				);

				index++;
			}
		}
	}

private:
	int map_width, map_height;

	void parseData( ifstream& is ){
		string line;
		while( getline( is, line ) ){
			if( *line.end() == 13 && line.size() > 0 ) line.resize(line.size()-1);
//			if( tolower( line.substr( 0, 5 ) ) == ""
		}
	}

	void loadData( const string& fileLoc ){
		ifstream is;
		is.open( fileLoc );

		parseData( is );

		is.close();
	}

	struct FloorBox {
		tile_t * tile;
		int index_x;
		int index_y;

		FloorBox( string fileLoc, int index_x = 0, int index_y = 0 ) {
			this -> index_x = index_x;
			this -> index_y = index_y;

			this -> tile = get_tile( fileLoc );
		}

		FloorBox() {
			index_x = 0;
			index_y = 0;

			tile = 0;
		}
	};

	vector<FloorBox> FloorBoxes;
};

vector<Floor> FloorPool;

struct Obstacle {
	sprite_t * sprite;
};

int main() {
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "1.2" );
	window.setFramerateLimit( FramePerSecond ); //max FPS

	cout << sizeof( sprite_t ) << " " << sizeof( tile_t ) << endl;
	new_sprite( "bbl.png", 15 );
	new_tile( "Outside_A2.png",16,12 );

	sprite_t * p;
	tile_t * p2;

	p = get_sprite( "bbl.png" );
	p2 = get_tile( "Outside_A2.png");
	
	
//	Floor Floor_1( "Outside_A2.png", 5,2, 9, 0 );

	int ii = 0;
	const sf::Color shiro( 0xff,0xff,0xff );

	while ( window.isOpen() ){
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}


		// Clear screen
		window.clear( shiro );

		//draw everything
		p -> draw( window, 0,0, ii++ * 20 / FramePerSecond );
		p2 -> draw( window, 85,0, 0, 0 );

		// Update the window
		window.display();
	}

	return 0;
}
