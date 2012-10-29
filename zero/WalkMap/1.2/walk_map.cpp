#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Array2D.cpp"

using namespace std;

enum direction_t {
	UP 		= 1,
	RIGHT 	= 2,
	DOWN 	= 3,
	LEFT 	= 4
};	

namespace walk_map {
	
	//=============================== DATA STRUCT ===============================
	class walk_map_entity {
	public:
		bool passable;
		bool moveable;
		bool isEnemy;

	private:	
		bool imageLoaded;
		string file_loc; //file location
		
		sf::Sprite sprite_;
		sf::Image image_;

	public:

		walk_map_entity( const string& file_loc_, bool _passable, bool _moveable, bool _isEnemy ){
			passable = _passable;
			moveable = _moveable;
			isEnemy = _isEnemy;
			file_loc = file_loc_;
			
			imageLoaded = false;
		}
		
		void unloadImage() {
			if( imageLoaded ){
				image_.loadFromFile("");
				
				imageLoaded = false;
			}
		}
		
		void loadImage(){
			if( !imageLoaded ){
				image_.loadFromFile( file_loc );
//				sprite_
				imageLoaded = true;
			}
		}
		
		void render( const sf::RenderWindow& App, direction_t direction ) {
			if( !imageLoaded ) loadImage();
			
			/* SETUP SPRITE */
			
			App.draw( sprite_ );
			
		}
	};

	//for database datas on string index
	map< string, walk_map_entity > walk_map_entity_info;
		
	void insert_walk_map( const string& name, const string& file_loc_, bool _passable, bool _moveable, bool _isEnemy ){
		walk_map_entity_info.insert(
			pair<string, walk_map_entity> ( 
				name, 
				walk_map_entity( file_loc_, _passable, _moveable, _isEnemy )
			)
		);
	}
	
	void init_info() {
		insert_walk_map( "floor", "resource\\f.png", true, false, false );
		insert_walk_map( "wall", "resource\\w.png", false, false, false );
		insert_walk_map( "tree", "resource\\t.png", false, false, false );
		
	}
	
	//=============================== END ===============================

	//hero's coordinate
	int hero_x;
	int hero_y;

	//map information
	int map_width;
	int map_height;

	Array2D<bool> map_ob; //map obsticle

	struct entity_t {
		short x,y;
		direction_t direction;
		walk_map_entity * data;
	};
		
}

int main() {
	database::init_info();
}
