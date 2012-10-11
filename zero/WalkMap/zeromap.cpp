/*
	Created by remax from Synergy
		2012
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct hero_t{
	short x,y;
} hero;

enum enemy_type{
	PORING,
	SLIME,
	DRAGON,
	ENEMY_TYPE_COUNT
};

struct enemy_t{
	short index;
	enemy_type type;

	enemy_t( short i, enemy_type t){
		type = t;
		index = i;
	}
};

bool check_exist( vector<enemy_t>& enemies, int index, enemy_type& type ){
	for( int i = enemies.size(); i--; ){
		if( enemies[i].index == index ){
			type = enemies[i].type;
			return true;
		}
	}
	return false;
}

void print( hero_t& dummy, const bool * p, vector<enemy_t>& enemies, int width, int height ){
	int limit = width * height;
	int index = dummy.y * width + dummy.x;

	enemy_type tp;

	for(int i = 0; i < limit; i++){
		if( i % width == 0 ) cout << endl;

		if( i == index ){
			cout << '0';
		}else if( check_exist( enemies, i, tp ) ) {
			switch( tp ){
				case PORING: cout << "P"; break;
				case DRAGON: cout << "D"; break;
				case SLIME: cout << "S"; break;
			}
		}else if( p[i] ){
			cout << "#";
		}else{
			cout << ".";
		}
	}
	cout << endl << endl;

}

bool movehero( hero_t& dummy, const bool * p, vector<enemy_t>& enemies, int width, int height, char direction ){
	char dx = 0, dy = 0;
	enemy_type tp;
	
	switch( direction ){
		case 'w': dy = -1; break;
		case 'a': dx = -1; break;
		case 's': dy = 1; break;
		case 'd': dx = 1; break;
	}

	if( dx ){
		if( 0 <= dummy.x + dx && dummy.x + dx < width ){
			if( check_exist( enemies, dummy.y*width + dummy.x + dx, tp ) ){
				cout << "You encountered ";
				switch( tp ){
					case PORING: cout << "PORING"; break;
					case DRAGON: cout << "DRAGON"; break;
					case SLIME: cout << "SLIME"; break;
				}
				
				cout << endl;
				
				return false;
			}
			
			if( p[ dummy.y*width + dummy.x + dx ] ) return false;
			dummy.x += dx;
			return true;
		}else return false;
	}

	if( dy ){
		if( 0 <= dummy.y + dy && dummy.y + dy < height ){

			if( check_exist( enemies, (dummy.y + dy)*width + dummy.x, tp ) ){
				cout << "You encountered ";
				switch( tp ){
					case PORING: cout << "PORING"; break;
					case DRAGON: cout << "DRAGON"; break;
					case SLIME: cout << "SLIME"; break;
					default: cout << tp; break;
				}
				cout << endl;
				
				return false;
			}

			if( p[ (dummy.y + dy)*width + dummy.x ] ) return false;
			dummy.y += dy;
			return true;
		}else return false;
	}
}

int main(){
	vector<enemy_t> enemies;

	int width, height, obstacle, enemy_c;
	cin >> width >> height >> obstacle >> enemy_c;

	//seed random number
	srand ( time(NULL) );


	int limit = width*height;
	bool * mapob = new bool[ limit ];

	for(int i = 0; i < limit; i++ ) mapob[i] = 0;

	for(int i = 0, index, safe = 0; i < obstacle && safe < limit; safe++){
		index = (rand() % height)*width+(rand() % width);
		if( !mapob[ index ] ) {
			mapob[ index ] = true;
			i++;
		}
	}

	for(int i = 0, index, safe = 0; i < enemy_c && safe < limit; safe++){
		index = (rand() % height)*width+(rand() % width);
		if( !mapob[ index ] ) {
			mapob[ index ] = true;

			enemy_type tp = (enemy_type) (rand() % ENEMY_TYPE_COUNT);

			enemies.push_back( enemy_t( index, tp ) );
			i++;
		}
	}



	print( hero, mapob, enemies, width, height );

	string directions;
	bool running = true;
	bool res = 0;

	while( running ){
		res = 0;
		cin >> directions;
		for( int i = 0;  i < directions.length(); ++i ){

			if( directions[i] == 'x' ) goto exit;
			res |= movehero( hero, mapob, enemies, width, height, directions[i] );
		}

		if( res ) print( hero, mapob, enemies, width, height );
	}

	exit:
	return 0;
}
