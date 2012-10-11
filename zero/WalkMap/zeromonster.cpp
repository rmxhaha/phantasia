/*
	Created by remax from Synergy
		2012
*/

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct 

class WorldMap {
public:
	
	void print(){
		int limit = width * height;
		int index = hero.y * width + hero.x;

		enemy_type tp;

		for(int i = 0; i < limit; i++){
			if( i % width == 0 ) cout << endl;

			if( i == index ){
				cout << '0';
			}else if( check_exist( i, tp ) ) {
				print_monster( tp );
			}else if( mapob[i] ){
				cout << "#";
			}else{
				cout << ".";
			}
		}
		cout << endl << endl;

	}

	void encounter( monster& input, bool plus ){
		if( plus ){
			cout << gSpeed+ h;
		}else{
			cout << gEncounter h; 
		}
		cout << input.type.name << endl;
		exit;
	}

	void print_monster( enemy_type tp ){
		switch( tp ){
			case KIRKALLAY: cout << "K"; break;
			case ZOMBIE: cout << "Z"; break;
			case RIDER: cout << "R"; break;
		}
	}
	
	bool movehero( char direction ){
		char dx = 0, dy = 0;
		enemy_type tp;
		
		switch( direction ){
			case 'w': dy = -1; break;
			case 'a': dx = -1; break;
			case 's': dy = 1; break;
			case 'd': dx = 1; break;
		}

		if( dx ){
			if( 0 <= hero.x + dx && hero.x + dx < width ){
				if( check_exist( enemies, hero.y*width + hero.x + dx, tp ) ){
					cout << "You encountered ";
					print_monster( tp );
					
					cout << endl;
					
					return false;
				}
				
				if( mapob[ hero.y*width + hero.x + dx ] ) return false;
				hero.x += dx;
				return true;
			}else return false;
		}

		if( dy ){
			if( 0 <= hero.y + dy && hero.y + dy < height ){

				if( check_exist( enemies, (hero.y + dy)*width + hero.x, tp ) ){
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

				if( mapob[ (hero.y + dy)*width + hero.x ] ) return false;
				hero.y += dy;
				return true;
			}else return false;
		}
	}

private:
	int width, height;
	
	struct hero_struct {
		short x,y;
	} hero;

	enum enemy_type {
		KIRKALLAY,
		ZOMBIE,
		RIDER,
		ENEMY_TYPE_COUNT
	};
	
	enum condition_t {
		SLEEPING,
		ONGUARD,
		CHASING
	};

	struct enemy_info {
		string name;
		int radius, view_radius;
	};
	
	struct enemy_struct {
		short index;
		enemy_info * type;

		enemy_t( short i, enemy_type t ){
			type = &enemy_list[ t ];
			index = i;
		}
	};

	vector<enemy_info> enemy_list;
	vector<enemy_struct> enemies;

	bool check_exist( int index, enemy_type& type ){
		for( int i = enemies.size(); i--; ){
			if( enemies[i].index == index ){
				type = enemies[i].type;
				return true;
			}
		}
		return false;
	}

}




int main(){
	
	
	
	return 0;
}
