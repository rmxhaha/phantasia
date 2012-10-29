#include <iostream>

using namespace std;

template<class Type>
class Array2D {

public:
	Type& operator() ( int x, int y ) {
		if( y * width + x < size && x < width ) {
			return *( p + y * width + x );
		} else {
			return *( p );
		}
	}

	Type& operator[] ( int index ){
		if( index < size ){
			return *( p + index );
		} else {
			*( p );
		}
	}

	Array2D( int width, int height ) {
		this -> p = NULL;
		this -> width = width;
		this -> size = width * height;

		//delete prior data if there are any
		delete[] p;

		//allocate data requested
		p = new Type[ size ];
	}
	Array2D(){
		this -> p = NULL;
		this -> width = 0;
		this -> size = 0;
	}

	void fill( Type input ){
		for( int i = size; i--; ) {
			*(p+i) = input;
		}
	}
	
	void redefine( int width, int height ){
		this -> width = width;
		this -> size = width * height;

		//delete prior data if there are any
		delete[] p;

		//allocate data requested
		p = new Type[ size ];
	}

private:
	Type * p; //array data
	int width; //width of the array
	int size; //size of the array
};
