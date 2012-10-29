#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Array2D.cpp"


using namespace std;

namespace walk_map {
	//please use save enum type
	enum entity_t {
		FLOOR_1,
		FLOOR_2,
		WALL,

		KIRKELLAY,
		ZOMBIE,
		RIDER,
		HERO,

		ENTITY_COUNT
	};
	
	bool isEnemy( entity_t type ){
		switch( type ){
			case KIRKELLAY: 
			case ZOMBIE: 
			case RIDER: 
				return true;
			default: 
				return false;
		}
	}
	
	bool isPassable( entity_t type ){
		switch( type ){
			case FLOOR_1:
			case FLOOR_2: 
				return false;
			default: 
				return true;
		}
	}
	

//	typedef IntItr vector<int>::iterator;

	enum direction_t {
		UP,
		RIGHT,
		DOWN,
		LEFT,

		DIRECTION_COUNT
	};

	struct enemy_information_t {
		string name;

		//Sprite and image will be here too
	};

	struct entity_enemy_t {
		unsigned short x, y;
		unsigned int pattern_index;
		enemy_information_t * info;
	};

	//====================== VARIABLE DECLERATION ======================

	int map_width = 0;
	int map_height = 0;
	int hero_x = 0;
	int hero_y = 0;

	vector< int* > moving_patterns; //array of array

	//for data about enemy
	map< entity_t, enemy_information_t > enemy_information;

	//for fast access to each blocks' entity
	Array2D< entity_t > map_entity_type;

	//for more data on each blocks' entity
	Array2D< void* > map_entity_data;

	//====================== END ======================

	//fill the enemy database informations
	void fill_information() {
		static bool enemy_information_filled = false;
		if( !enemy_information_filled ){
			enemy_information[ KIRKELLAY ].name = "Kirkellay";
			enemy_information[ ZOMBIE ].name = "Zombie";
			enemy_information[ RIDER ].name = "Rider";

			enemy_information_filled = true;
		}
	}

	//initilize the walk_map namespace
	void init() {
		fill_information();

		map_width = map_height = hero_x = hero_y = 0;
	}

	void debug_print() {
		for( int i = 0; i < map_entity_type.size(); i++ ){
			switch( map_entity_type[i] ){
				case FLOOR_1: case FLOOR_2:
					cout << ".";
					break;
				case WALL:
					cout << "#";
					break;
				case KIRKELLAY: case ZOMBIE: case RIDER:
					cout << "M";
					break;
				case HERO:
					cout << "H";
					break;
			}
			if( i % map_width ) cout << endl;
		}

		cout << endl << endl;
	}

	void encounter( entity_t type ){
		cout << "You encounter " << enemy_information[ type ].name << endl;
	}

	void move_hero( direction_t direction ){
		short dx = 0, dy = 0;
		int p;

		switch( direction ){
			case UP: dy = -1; break;
			case DOWN: dy = 1; break;
			case LEFT: dx = -1;	break;
			case RIGHT: dx = 1;	break;
		}

		if( dx != 0 ){
			if( 0 <= hero_x + dx && hero_x + dx < map_width ){
				entity_t tmp = map_entity_type( hero_x + dx, hero_y );

				switch( tmp ){

				case WALL:
					return;
				case KIRKELLAY: case RIDER: case ZOMBIE:
					encounter( tmp );
					break;
				case FLOOR_1: case FLOOR_2:
					hero_x += dx;
					break;

				}
			}
		}

		if( dy != 0 ) {
			if( 0 <= hero_y + dy && hero_y + dy < map_height ){
				entity_t tmp = map_entity_type( hero_x + dx, hero_y );

				switch( tmp ){

				case WALL:
					return;
				case KIRKELLAY: case RIDER: case ZOMBIE:
					encounter( tmp );
					break;
				case FLOOR_1: case FLOOR_2:
					hero_y += dy;
					break;

				}
			}

		}
	}
		
	bool parse_buffer( char * data, int length ){
		int i = 0;
		entity_t _type_;
		
		map_width = data[i++];
		map_height = data[i++];

		int index_entity = 0;
		map_entity_data.redefine( map_width, map_height );
		
		while( i < length ) {
			_type_ = (entity_t) data[i++];
			if( isEnemy( _type_ ) ) {
				entity_enemy_t * tmp = new entity_enemy_t;
				map_entity_data[ index_entity ] = tmp;
				
				tmp -> x = data[i++];
				tmp -> y = data[i++];
				tmp -> pattern_index = 0;
				tmp -> info = enemy_information[ _type_ ];
				
				index_entity++;
			} else if( isPassable( _type_ ) ){
				
			} else if( _type_ == HERO ){
				
			}
		}
	}
}

int main(){
	walk_map::init();
	
	

}
