/*
	Created by remax from Synergy
		2012
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct hero_t{
	short x,y;
} hero;

void print( hero_t& dummy, const bool * p, int width, int height ){
	int limit = width * height;
	int index = dummy.y * width + dummy.x;

	for(int i = 0; i < limit; i++){
		if( i % width == 0 ) cout << endl;
		if( i == index ){
			cout << '0';
		}else if( p[i] ){
			cout << "#";
		}else{
			cout << ".";
		}
	}
	cout << endl << endl;

}

bool movehero( hero_t& dummy, const bool * p, int width, int height, char direction ){
	char dx = 0, dy = 0;

	switch( direction ){
		case 'w': dy = -1; break;
		case 'a': dx = -1; break;
		case 's': dy = 1; break;
		case 'd': dx = 1; break;
	}

	if( dx ){
		if( 0 <= dummy.x + dx && dummy.y + dx < width ){
			if( p[ dummy.y*width + dummy.x + dx ] ) return false;
			dummy.x += dx;
			return true;
		}else return false;
	}

	if( dy ){
		if( 0 <= dummy.y + dy && dummy.y + dy < height ){
			if( p[ (dummy.y + dy)*width + dummy.x ] ) return false;
			dummy.y += dy;
			return true;
		}else return false;
	}

}


int main(){

	int width, height, obstacle;
	cin >> width >> height >> obstacle;

	//seed random number
	srand ( time(NULL) );


	bool * mapob = new bool[ width*height ];
	for(int i = 0; i < width * height; i++ ) mapob[i] = 0;

	for(int i = 0, index; i < obstacle;){
		index = (rand() % height)*width+(rand() % width);
		if( !mapob[ index ] ) {
			mapob[ index ] = true;
			i++;
		}
	}


	print( hero, mapob, width, height );

	string directions;
	bool running = true;
	bool res = 0;

	while( running ){
		cin >> directions;
		for( int i = 0;  i < directions.length(); ++i ){

			if( directions[i] == 'x' ) goto exit;
			res |= movehero( hero, mapob, width, height, directions[i] ) ;
		}
		
		if( res ) print( hero, mapob, width, height );
	}

	exit:
	return 0;
}
