#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <windows.h>
#include <cassert>

using namespace std;

void gotoxy( short x, short y )
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ;
    COORD position = { x, y } ;

    SetConsoleCursorPosition( hStdout, position ) ;
}

int map_limit = 0;
int map_width 	= 0;
int map_height 	= 0;

char * map = 0;
short * smart_map = 0; //for finding shortest path

void load_map( const string& fileLoc )
{
	ifstream is( fileLoc.c_str(), ios::binary );

	char * buffer;
	int length;

	// get length of file:
	is.seekg (0, ios::end);
	length = is.tellg();
	is.seekg (0, ios::beg);

	buffer = new char[ length ];

	is.read( buffer, length );
	is.close();

	int max = 0;
	int count = 0;
	int lines = 0;
	for( int i = 0 ; i < length ; i ++ ){
		if( buffer[i] == '\n' ){
			if( max < count ) max = count;
			count = 0;
			lines++;
		} else {
			count ++;
		}
	}
	map_height = ( buffer[ length - 1 ] == '\n' ? lines : lines + 1 );
	map_width = max;
	map_limit = map_width * map_height;

	//Create the map Array
	map = new char[ map_width * map_height ];
	smart_map = new short[ map_limit ];

	// Warning : if there happened to be an array and delete[] is not called it will leaked

	int k = 0;

	for( int i = 0 ; i < length ; i ++ ){
		if( buffer[i] == '\n' ){
			for( ; k%max != 0 ; k ++ ){
				*( map + k ) = ' ';
			}
		} else {
			*( map + k ) = buffer[i];
			k++;
		}
	}

	delete[] buffer;
}

void show_map()
{

	for( int k = 0; k < map_limit; ++ k ){
		if( k % map_width == 0 && k != 0 ) cout << endl;
		cout << map[k];
	}

}


struct coor {
	coor( short _x, short _y ){
		x = _x;
		y = _y;
	}

	short x;
	short y;
};

queue<coor> fillQueue;

void qpush( short x, short y, int val ) {
	if( x < map_width && y < map_height ) {
		if( smart_map[ y * map_width + x ] > val ){
			smart_map[ y * map_width + x ] = val;
			fillQueue.push( coor( x, y ) );
		}
	}
}

void fillSM( int x, int y )
{
	assert( smart_map != 0 );
	
	for( int i = 0 ; i < map_limit; i++ ) {
		if( map[i] == ' ' ){
			smart_map[i] = numeric_limits<short>::max();
		} else {
			smart_map[i] = -1; //-1 is a block
		}
	}

	qpush( x, y, 1 );

	int p;
	while( ! fillQueue.empty() ){
		p = smart_map[ fillQueue.front().y * map_width + fillQueue.front().x ] + 1;
		qpush ( fillQueue.front().x + 1, fillQueue.front().y, p );
		qpush ( fillQueue.front().x - 1, fillQueue.front().y, p );
		qpush ( fillQueue.front().x, fillQueue.front().y + 1, p );
		qpush ( fillQueue.front().x, fillQueue.front().y - 1, p );

		fillQueue.pop();
	}
}

void NextAIPath( short& x, short& y ){
	
	assert( smart_map != 0 );
	
	if( smart_map[ y * map_width + x ] == 1 ) return;
	
	int min = numeric_limits<int>::max();
	int dir = 5;
	
	if( smart_map[ y * map_width + x + 1 ] >= 0 ){
		min = smart_map[ y * map_width + x + 1 ];
		dir = 2;
	}

	if( smart_map[ y * map_width + x - 1 ] >= 0 && min > smart_map[ y * map_width + x - 1 ] ){
		min = smart_map[ y * map_width + x - 1 ];
		dir = 4;
	}

	if( smart_map[ (y + 1 ) * map_width + x ]>= 0 && min > smart_map[ (y + 1 ) * map_width + x ] ){
		dir = 3;
		min = smart_map[ (y + 1 ) * map_width + x ];
	}

	if( smart_map[ (y - 1 ) * map_width + x ]>=0 && min > smart_map[ (y - 1 ) * map_width + x ] ){
		dir = 1;
	}
	
	switch( dir ){
		case 1:
			y = y - 1;
			break;
		case 2:
			x = x + 1;
			break;
		case 3:
			y = y + 1;
			break;
		case 4:
			x = x - 1;
			break;
		default: break;
	}
	
}

class entity {
public:
	entity( int x, int y ){
		this -> x = x;
		this -> y = y;
	}

	void move( int p, int q ){
		x += p;
		y += q;
	}

	int get_x(){ return x; }
	int get_y(){ return y; }

	void draw(){
		gotoxy( x, y ); printf( "%c", p );
	}

private:
	int x;
	int y;
};

class Eater : public entity {
public:

};




int main(){
	load_map("map.txt");
	bool running = true;

	show_map();

	fillSM( 1, 1 );

/*
	for( int i = 0 ; i < map_limit; i++ ){
		if( i % map_width == 0 ) cout << endl;
		if( smart_map[i] == -1 ) cout << "##"; else {
			cout << smart_map[i];
			if( smart_map[i] < 10 ) cout << " ";
		}
	}*/

	short x = 15;
	short y = 16;
	short x1 = 18;
	short y1 = 5;
	
	while( smart_map[ y * map_width + x ] != 1 ){
		gotoxy( x, y ); cout << " ";
		gotoxy( x1, y1 ); cout << " ";

		NextAIPath( x, y );
		NextAIPath( x1, y1 );

		gotoxy( x, y ); cout << "X";
		gotoxy( x1, y1 ); cout << "E";

		Sleep( 100 );

	}
	
	
	cin.get();
	
	return 0;

	while( running ){
		gotoxy( 0,0 );
		show_map();

		Sleep( 50 ); //20 Frame Per Second
	}

	return 0;
}
