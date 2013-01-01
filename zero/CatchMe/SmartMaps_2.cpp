#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <queue>

#include "Array2D.cpp"

using namespace std;


struct hero_t 
{
	unsigned x; // in block unit  
	unsigned y; // in block unit 
	size_t width; // in block unit		
};


class SmartMap_t {
public:
	SmartMap_t( hero_t * hero_ ){
		hero = hero_;
		map_width = 0;
		map_height = 0;
	}
	
	~SmartMap_t(){
		
	}
	
	void update( Array2D< short >& Obs_ ) {
		Obs = Obs_;
		map_width = Obs.width_();
		map_height = Obs.height_();
		for( unsigned x = 0; x < hero->width; ++x ){
			for( unsigned y = 0; y < hero->width; ++y ){
				Obs.get( hero->x + x, hero->y + y ) = -1;
			}
		}
		Smart.clear(); // remove everything
	}
	
	int getAiPath( int _x, int _y, int _w ){
		if( collideWithHero( _x, _y, _w ) ) return 6; // Destination Reached
		
		/* Check the exisiting solution */
		if( Smart.size() > _w && Smart[ _w ].width_() != 0 )
		{
			return NextAiPath( Smart[ _w ], _x, _y, _w );
		}			
		
		/* Search the solution because the solution doesn't exist yet */
		if( Smart.size() <= _w ) Smart.resize( _w + 1 );
		
		fillPath( _w );

		for( int y = 0; y < map_width + 1 - _w; ++ y ){
			for( int x = 0; x < map_height + 1 - _w; ++x ){
				cout << setw( 5 ) << Smart[ _w ].get( x, y );
			}
			cout << '\n';
		}
		cout << flush;

		return NextAiPath( Smart[ _w ], _x, _y, _w );
	}
	
	int NextAiPath( Array2D< int >& SmartMap, int x, int y, int _w ){
		int min = numeric_limits<int>::max();
		int direction = 5;
		
		if( x + 1 < map_width + 1 - _w && SmartMap.get( x + 1, y ) >= 0 ){
			min = SmartMap.get( x+1, y );
			direction = 2;
		}

		if( 0 <= x - 1 && SmartMap.get( x - 1, y ) >= 0 && min > SmartMap.get( x - 1, y ) ){
			min = SmartMap.get( x-1, y );
			direction = 4;
		}

		if( y + 1 < map_height + 1 - _w && SmartMap.get( x, y + 1 ) >= 0 && min > SmartMap.get( x, y + 1 ) ){
			direction = 3;
			min = SmartMap.get( x, y+1 );
		}

		if( 0 <= y - 1 && SmartMap.get( x, y - 1 ) >=0 && min > SmartMap.get( x, y-1 ) ){
			direction = 1;
		}
		
		return direction;
	}
	
	bool collideWithHero( int _x, int _y, int _w ){
		if( hero->x + hero->width < _x ) 	return false;
		if( _x + _w < hero->x ) 			return false;
		if( hero->y + hero->width < _y ) 	return false;
		if( _y + _w < hero->y ) 			return false;
		
		return true;
	}
	
	
	struct coor_t {
		coor_t( short _x, short _y ){
			x = _x;
			y = _y;
		}

		short x;
		short y;
	};

	void fillPath( int _w ){
		queue<coor_t> fillQueue;
		Array2D< int > * target_map = &Smart[ _w ];

		target_map->resize( map_width + 1 - _w, map_height + 1 - _w );

		for( int x = 0; x < map_width + 1 - _w; ++x ){
			for( int y = 0; y < map_height + 1 - _w; ++y ){
				if( Obs.get( x,y ) != -1 ){
//					target_map -> get( x, y ) = numeric_limits<int>::max();
					target_map -> get( x, y ) = 401;
				} else {
					target_map -> get( x, y ) = -1;
				}
			}
		}
		
		for( unsigned x = 0; x < hero->width; ++x ){
			for( unsigned y = 0; y < hero->width; ++y ){
				target_map -> get( hero->x + x, hero->y + y ) = 1;
				fillQueue.push( coor_t( hero->x + x, hero->y + y ) );
			}
		}
		

		int val;
		while( ! fillQueue.empty() ){
			val = target_map->get( fillQueue.front().x, fillQueue.front().y ) + 1;
			
			const int& x = fillQueue.front().x;
			const int& y = fillQueue.front().y;

			int i;
						
			if( 0 <= x + 1 && 0 <= y && x + 1 < map_width + 1 - _w && y < map_height + 1 - _w ) {
				for( i = 0; i < _w; ++i ){
					if( Obs.get( x - 1 + _w + 1, y + i ) == -1 ) break;
				}
				
				if( i == _w && target_map->get( x + 1, y ) > val ){
					target_map->get( x + 1, y ) = val;
					fillQueue.push( coor_t( x + 1, y ) );
				}
			}
			
			if( 0 <= x - 1 && 0 <= y && x - 1 < map_width + 1 - _w && y < map_height + 1 - _w ) {
				for( i = 0; i < _w; ++i ){
					if( Obs.get( x - 1, y + i ) == -1 ) break;
				}
				
				if( i == _w && target_map->get( x - 1, y ) > val ){
					target_map->get( x - 1, y ) = val;
					fillQueue.push( coor_t( x - 1, y ) );
				}
			}

			if( 0 <= x && 0 <= y + 1 && x < map_width + 1 - _w && y + 1 < map_height + 1 - _w ) {
				for( i = 0; i < _w; ++i ){
					if( Obs.get( x + i, y  - 1 + _w + 1 ) == -1 ) break;
				}
				
				if( i == _w && target_map->get( x, y + 1 ) > val ){
					target_map->get( x, y + 1 ) = val;
					fillQueue.push( coor_t( x, y + 1 ) );
				}
			}

			if( 0 <= x && 0 <= y - 1 && x < map_width + 1 - _w && y - 1 < map_height + 1 - _w ) {
				for( i = 0; i < _w; ++i ){
					if( Obs.get( x + i, y - 1 ) == -1 ) break;
				}
				
				if( i == _w && target_map->get( x, y - 1 ) > val ){
					target_map->get( x, y - 1 ) = val;
					fillQueue.push( coor_t( x, y - 1 ) );
				}
			}
			
			fillQueue.pop();
		}
		
		target_map = NULL;
	}
	
//private:
	int map_width;
	int map_height;
	
	hero_t * hero;

	Array2D< short > Obs;	
	vector< Array2D< int > > Smart;	
};


int main(){
	hero_t h;
	h.x = 1;
	h.y = 1;
	h.width = 2;
	
	SmartMap_t SMap( &h );
	
	Array2D< short > HITS;
	HITS.resize( 10, 10 );

	for( int x = 0; x < 10; ++x ){
		for( int y = 0; y < 10; ++y ){
			HITS.get( x, y ) = - 0;
		}
	}
	
	HITS.get( 5, 5 ) = -1;
	HITS.get( 4, 5 ) = -1;
	HITS.get( 5, 4 ) = -1;
	
	SMap.update( HITS );	
	cout << SMap.getAiPath( 7, 7, 2 ) << endl;
	
	
	
	return 0;
}

