#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cassert>
#include <SFML/Graphics.hpp>
#include "Array2D.cpp"
#include "RString.cpp"

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

image_t * get_image( const string& fileLoc ){
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

tile_t * new_tile( const string& fileLoc, int width_n, int height_n ) {
	tilePool.push_back( new tile_t( fileLoc, width_n, height_n ) );
	return tilePool.back();
}

sprite_t * new_sprite( const string& fileLoc, int width_n ) {
	spritePool.push_back( new sprite_t( fileLoc, width_n ) );
	return spritePool.back();
}

tile_t * get_tile( const string& fileLoc ) {
	int i = 0;
	for( ; i < tilePool.size(); ++i ){
		if( tilePool[i] -> fileLocation() == fileLoc )
			return tilePool[i];
	}

	return 0;
}

sprite_t * get_sprite( const string& fileLoc ) {
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


enum MOD_T {
	MOD_WANDERMAP,
	MOD_BATTLEMAP,
	MOD_MENU,
	MOD_MAINMENU,
	MOD_VISUALNOVEL
};

int chmod( MOD_T code ){

}

int chmod( MOD_T code, const string& target ){

}



class WanderMap {
public:
	void init() {
		map_width = 0;
		map_height = 0;

		obs_data.fill( 1 );
	}

	WanderMap( const string& fileLoc ) {
		init();
		loadData( fileLoc );
	}

	WanderMap() {
		init();
	}

	~WanderMap(){
	}

	void draw( sf::RenderWindow& App ) {
		assert( map_width * map_height == FloorBoxes.size() );

		static int coor_x, coor_y, index;

		//FLOORS LAST LAYER
		index = 0;

		for( coor_y = 0; coor_y < map_height; ++coor_y ){
			for( coor_x = 0; coor_x < map_width; ++coor_x ){
				if( FloorBoxes[ index ].tile != 0 ){
					FloorBoxes[ index ].tile -> draw (
						App,
						::camera_x + coor_x * gridsize,
						::camera_y + coor_y * gridsize,
						FloorBoxes[ index ].index_x,
						FloorBoxes[ index ].index_y
					);
				}

				index++;
			}
		}


	}

	void loadData( const string& fileLoc ){
		ifstream is;
		is.open( fileLoc.c_str(), ios::binary );

		string line;
		vector<string> vstr;
		vector<int> vint;
		int index;
		int limit_size;
		int line_num = 0; //start counting from zero skip 1 for map width and height
		
		getline( is, line ); line_num++; 
		limit_size = line_num + parseInt( line );

		while( line_num < limit_size && getline( is, line ) ){
			if ( line[line.size()-1] == 13 && line.size() > 0 ) line.resize(line.size()-1);

			vstr = explode( ' ', line );
			assert( vstr.size() >= 3 );
			new_tile( vstr[0], parseInt( vstr[1] ), parseInt( vstr[2] ) );
			
			line_num++;
		}
		
		getline( is, line ); line_num++;
		limit_size = line_num + parseInt( line );
		
		while( line_num < limit_size && getline( is, line ) ){
			if ( line[line.size()-1] == 13 && line.size() > 0 ) line.resize(line.size()-1);
			vstr = explode( ' ', line );
			assert( vstr.size() >= 2 );
			new_sprite( vstr[0], parseInt( vstr[1] ) );

			line_num++;
		}

		getline( is, line ); line_num++; 
		vstr = explode(' ', line );

		assert( vstr.size() >= 2 );

		map_width = parseInt( vstr[0] );
		map_height = parseInt( vstr[1] );

		obs_data.resize( map_width, map_height );

		limit_size = line_num + map_width * map_height;

		while( line_num < limit_size && getline( is, line ) ){
			if ( line[line.size()-1] == 13 && line.size() > 0 ) line.resize(line.size()-1);

			if( line.size() >= 4 ){
				if( tolower( line.substr(0,4) ) == "skip" ) {
					FloorBoxes.push_back( FloorBox() );
				} else {
					vstr = explode( ' ', line );

					FloorBoxes.push_back( FloorBox(
						vstr[0],
						parseInt(vstr[1]),
						parseInt(vstr[2])
					) );

					obs_data[ line_num ] = 0;
				}
			}

			line_num++;
		}

		is.close();
	}

private:

	// =========== FLOOR ============

	struct FloorBox {
		tile_t * tile;
		int index_x;
		int index_y;

		FloorBox( const string& fileLoc, int index_x = 0, int index_y = 0 ) {
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

	// =========== END ============


	// VARIABLES
	int map_width;
	int map_height;

	Array2D<int> obs_data;
	vector<FloorBox> FloorBoxes;

};


struct Obstacle {
	sprite_t * sprite;
};

int main() {

//	cout << sizeof( sprite_t ) << " " << sizeof( tile_t ) << endl;
//	new_sprite( "bbl.png", 15 );
//	new_tile( "Outside_A2.png",16,12 );

//	sprite_t * p;
//	tile_t * p2;

//	p = get_sprite( "bbl.png" );
//	p2 = get_tile( "Outside_A2.png");

	WanderMap WMAP;
	WMAP.loadData("map.txt");

	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "1.2" );
	window.setFramerateLimit( FramePerSecond ); //max FPS

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
		WMAP.draw( window );
//		p -> draw( window, 0,0, ii++ * 20 / FramePerSecond );
//		p2 -> draw( window, 85,0, 0, 0 );

		// Update the window
		window.display();
	}

	return 0;
}
