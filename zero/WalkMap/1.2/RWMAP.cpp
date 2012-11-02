#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include "Array2D.cpp"
#include "RString.cpp"

using namespace std;


struct tile_t {
    int gak_kopong;
} ttt;

tile_t * get_tile( const string& n){
    return &ttt;
}

int main();


class WanderMap {
public:
	void init(){
		map_width = 0;
		map_height = 0;
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



	void loadData( const string& fileLoc ){
		ifstream is;
		is.open( fileLoc.c_str(), ios::binary );

		string line;
		vector<string> vstr;
		vector<int> vint;
		int map_size;
		int line_num = 1; //start counting from zero skip 1 for map width and height

		getline( is, line );

		//get the first line
		vstr = explode(' ', line );

		assert( vstr.size() >= 2 );

		map_width = parseInt( vstr[0] );
		map_height = parseInt( vstr[1] );

		obs_data.resize( map_width, map_height );
		obs_data.fill( 1 );

		map_size = map_width * map_height;

		while( getline( is, line ) && line_num <= map_size ){
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

					obs_data.get( line_num ) = 0;
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

		void init( const string& fileLoc_, int index_x_ = 0, int index_y_ = 0 ){
			this -> index_x = index_x_;
			this -> index_y = index_y_;

			this -> tile = get_tile( fileLoc_ );
		}

		FloorBox( const string& fileLoc, int index_x = 0, int index_y = 0 ) {
			init( fileLoc, index_x, index_y );
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

int main(){	
	WanderMap WMAP;
	WMAP.loadData( "map.txt" );

	return 0;
}
