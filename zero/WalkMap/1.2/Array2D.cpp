#include <iostream>

using namespace std;

template<class Type>
class Array2D {

public:
	Type& operator() ( int x, int y ) {
		if( y * _width + x < _size && x < _width ) {
			return *( p + y * _width + x );
		} else {
			return *( p );
		}
	}

	Type& operator[] ( int index ){
		if( index < _size ){
			return *( p + index );
		} else {
			*( p );
		}
	}

	Array2D( int width, int height ) {
		this -> p = NULL;
		this -> _width = width;
		this -> _size = width * height;

		//allocate data requested
		p = new Type[ _size ];
	}

	Array2D(){
		this -> p = NULL;
		this -> _width = 0;
		this -> _size = 0;
	}

	~Array2D(){
		//prevent leak
		delete[] p;
	}

	void fill( Type input ){
		for( int i = _size; i--; ) {
			*(p+i) = input;
		}
	}

	void redefine( int width, int height ){
		this -> _width = width;
		this -> _size = width * height;

		//delete prior data if there are any
		delete[] p;

		//allocate data requested
		p = new Type[ _size ];
	}
	
	int size(){
		return this->_size;
	}
	int width(){
		return _width;
	}
	int height(){
		return _size / _width;
	}
	
	Type& back(){
		return *(p+_size);
	}

private:
	Type * p; //array data
	int _width; //width of the array
	int _size; //size of the array
};
