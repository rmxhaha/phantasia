#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cassert>
#include <SFML/Graphics.hpp>


/* To reduce compile time */
#include "Array2D.cpp"
#include "RString.cpp"
#include "RImage.cpp"
#include "RImage32.cpp"

using namespace std;

const int gridsize = 32;
const int FramePerSecond = 30;
int camera_x = 0;
int camera_y = 0;



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

	~WanderMap() {
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

		for( index = 0; index < Obstacles.size(); ++index ){
			Obstacles[index].tile32 -> draw(
				App,
				Obstacles[index].x * gridsize,
				Obstacles[index].y * gridsize,
				0,	0
			);
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
			if ( line[line.size()-1] == 13 && line.size() > 0 ) line.resize( line.size()-1 );

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

		index = 0;

		while( line_num < limit_size && getline( is, line ) ){
			if ( line[line.size()-1] == 13 && line.size() > 0 ) line.resize(line.size()-1);

			if( tolower( line.substr(0,4) ) == "skip" ) {
				FloorBoxes.push_back( FloorBox() );

				obs_data[ index ] = 1;
			} else {
				vstr = explode( ' ', line );

				FloorBoxes.push_back( FloorBox(
					vstr[0],
					parseInt(vstr[1]),
					parseInt(vstr[2])
				) );

				obs_data[ index ] = 0;
			}

			line_num++;
			index++;
		}


		getline( is, line ); line_num++;
		limit_size = line_num + parseInt( line );

		short x,y,lx,ly,ty;
		while( line_num < limit_size && getline( is, line ) ){
			if ( line[line.size()-1] == 13 && line.size() > 0 ) line.resize(line.size()-1);
			vstr = explode( ' ', line );

			assert( vstr.size() >= 3 );

			x = parseInt( vstr[1] );
			y = parseInt( vstr[2] );

			//just being careful
			assert( x == x % map_width );
			assert( y == y % map_height );

			Obstacles.push_back( Obstacle( vstr[0],x,y ) );
			ty = y;
			
			lx = x + Obstacles.back().tile32 -> width32() ;
			ly = y + Obstacles.back().tile32 -> height32();

			for( ; x < lx; ++x ){
				for( y = ty; y < ly; ++y ){
					obs_data.get( x,y ) = 1;
				}
			}


			line_num++;
		}

		/*
		for( index = 0; index < obs_data.size(); ++index ){
			if( index % map_width == 0 ) cout << endl;
			cout << obs_data[index];
		}
		*/

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


	// =========== FLOOR ============

	struct Obstacle {

		Obstacle( const string& fileLoc, short _x, short _y ) {
			x = _x;
			y = _y;
			tile32 = get_tile32( fileLoc );
		}

		Obstacle( tile32_t * t, short _x, short _y ) {
			x = _x;
			y = _y;
			tile32 = t;
		}

		short x,y;
		tile32_t * tile32;		//One per image
	};
	
	struct Hero {
		Hero(){
			
		}
		
		sprite_t * sprite;
	};

	// =========== END ============

	// VARIABLES
	int map_width;
	int map_height;

	Array2D<int> obs_data;
	vector<FloorBox> FloorBoxes;
	vector<Obstacle> Obstacles;
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

	sf::RenderWindow window( sf::VideoMode( 720, 480 ), "1.2" );
	window.setFramerateLimit( FramePerSecond ); //max FPS

//	Floor Floor_1( "Outside_A2.png", 5,2, 9, 0 );

	int ii = 0;
	const sf::Color shiro( 0xff,0xff,0xff );
	const sf::Color kuro( 0x00,0x00,0x00 );

	while ( window.isOpen() ){
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}


		// Clear screen
		window.clear( kuro );

		//draw everything
		WMAP.draw( window );
//		p -> draw( window, 0,0, ii++ * 20 / FramePerSecond );
//		p2 -> draw( window, 85,0, 0, 0 );

		// Update the window
		window.display();
	}

	return 0;
}
