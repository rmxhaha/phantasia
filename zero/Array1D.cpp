/*
	Created by remax from Synergy
		2012
*/

#include <iostream>

using namespace std;

template<class T>
class Array2D {
public:
	T& operator() ( int x, int y ){
		if( x < w && y < h )
			return _data[ y*w + x ];
        else
            return _data[ 0 ];//Error area
	}
	Array2D( int w, int h ){
		this -> w = w;
		this -> h = h;
		_data = new T[ w*h ];
	}

	T * data(){
		return _data;
	}

	void fill( T p ){
		for( int i = w*h; i--; ){
			_data[i] = p;
		}
	}

private:
	T * _data;
	int w;
	int h;
};
template<class T>
class Array3D {
public:
	T& operator() ( int x, int y, int z ){
		if( x < w && y < h && z < d )
			return _data[ z*w*h + y*w + x ];
		else
			return _data[ 0 ]; //error area
	}
	Array3D( int w, int h, int d ){
		this -> w = w;
		this -> h = h;
		this -> d = d;
		_data = new T[ w*h*d ];
	}

	T * data(){
		return _data;
	}

	void fill( T p ){
		for( int i = w*h*d; i--; ){
			_data[i] = p;
		}
	}

private:
	T * _data;
	int w;
	int h;
	int d; //depth
};


int main(){
	Array2D<int> yolo( 10, 10 );
	yolo.fill( 0 );
	cin >> yolo( 9, 3 );
	cout << yolo( 9, 3);

	return 0;
}
