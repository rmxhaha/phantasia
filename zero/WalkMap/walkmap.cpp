#include "walkmap.h"

void fill_enemy_db(){
	enemy_db.resize( ENEMY_TYPE_COUNT );

	enemy_db[ KIRKELLAY ].name = "Kirkellay";
	enemy_db[ KIRKELLAY ].radius = 3;
	enemy_db[ KIRKELLAY ].view_radius = 5;
	enemy_db[ KIRKELLAY ].code_name = 'K';

	enemy_db[ ZOMBIE ].name = "Zombie";
	enemy_db[ ZOMBIE ].radius = 3;
	enemy_db[ ZOMBIE ].view_radius = 5;
	enemy_db[ ZOMBIE ].code_name = 'Z';

	enemy_db[ RIDER ].name = "Rider";
	enemy_db[ RIDER ].radius = 3;
	enemy_db[ RIDER ].view_radius = 5;
	enemy_db[ RIDER ].code_name = 'R';
}

walk_map::walk_map( int w, int h ){
	if( enemy_db.size() == 0 ) fill_enemy_db();
	map_height = h;
	map_width = w;

	field.redefine( w, h );
	field.fill( 0 );
}

walk_map::~walk_map(){

}

void walk_map::print(){
	int limit = map_width * map_height;
	int index = dummy.y * map_width + dummy.x;

	int p;


	for( int i = 0; i < limit; ++i ){
		if( i % map_width == 0 ) cout << endl;

		if( i == index ){
			cout << "H";
		}else if( check_exist( i, p ) ){
			cout << enemies[ p ].type->code_name;
		}else if( field[ i ] ){
			cout << '#';
		}else{
			cout << '.';
		}
	}

	cout << endl << endl;
}

int walk_map::find_enemy( int x, int y ){
	return find_enemy( y*map_width + x );
}

int walk_map::find_enemy( int index ){
	for( int i = enemies.size(); i--; ){
		if( enemies[i].y * map_width + enemies[i].x == index ){
			return i;
		}
	}

	return -1;
}

bool walk_map::check_exist( int index ){
	return ( find_enemy( index ) >= 0 );
}

bool walk_map::check_exist( int index, int& eindex ){
	int p = find_enemy( index );
	if( p >= 0 ){
		eindex = p;
	}

	return p >= 0;
}

void walk_map::move_hero( char direction ) {
	char dx = 0, dy = 0;

	int p;

	switch( direction ){
		case 'w': dy = -1; break;
		case 'a': dx = -1; break;
		case 's': dy = 1; break;
		case 'd': dx = 1; break;
	}

	if( dx != 0 ){
		if( 0 <= dummy.x + dx && dummy.x + dx < map_width ){
			if( check_exist( dummy.y*map_width + dummy.x + dx, p ) ){
				encounter( enemies[p], false );

				return;
			}

			if( field( dummy.x + dx, dummy.y ) ) return;
			dummy.x += dx;
		}
	}

	if( dy != 0 ) {
		if( 0 <= dummy.y + dy && dummy.y + dy < map_height ){
			if( check_exist( (dummy.y + dy)*map_width + dummy.x, p ) ){
				encounter( enemies[p], false );

				return;
			}

			if( field( dummy.x, dummy.y + dy ) ) return;
			dummy.y += dy;
		}
	}
}

void walk_map::handle_enemy(){

}

void walk_map::encounter( enemy_struct& enemy, bool plus ){
	if( plus ) cout << "Speed+ "; else cout << "Encounter ";

	cout << enemy.type -> name << endl;
}

void walk_map::add_enemy( enemy_struct input ){
	enemies.push_back( input );
}

void walk_map::add_random_enemy( int number ){
	int i = map_height*map_width;
	int rx,ry;

	while( number && i-- ){
		rx = rand() % map_width;
		ry = rand() % map_height;

		if( field( rx, ry ) == false ){
			number--;
			field( rx, ry ) = true;

			enemy_struct temp( rand() % map_width, rand() % map_height, (enemy_t) (rand() % ENEMY_TYPE_COUNT) );
			enemies.push_back( temp );
		}
	}
}
void walk_map::add_random_wall( int number ){
	int i = map_height*map_width;
	int rx,ry;

	while( number && i-- ){
		rx = rand() % map_width;
		ry = rand() % map_height;

		if( field( rx, ry ) == false ){
			number--;
			field( rx, ry ) = true;
		}
	}

}


int main(){
	//seed random number
	srand ( time(NULL) );

	walk_map virtual_world( 20, 20 );

	virtual_world.add_random_wall( 10 );
	virtual_world.add_random_enemy( 5 );
	virtual_world.print();


	string input;
	bool running = true;

	while( running ){

		cin >> input;
		for( int i = 0;  i < input.length(); ++i ){
			if( input[i] == 'x' ) goto exit;

			virtual_world.move_hero( input[i] );
		}

//		virtual_world.handle_enemy();
		virtual_world.print();
	}

	exit:
	return 0;
}
