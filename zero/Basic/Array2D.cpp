#include <iostream>

using namespace std;

template<class Type>
class deux {

public:
	Type& operator() ( int x, int y ) {
		if( y * width + x < size && x < width ) {
			return *( p + y * width + x );
		} else {
			return *( p );
		}
	}

	deux( int width, int height ) {
		this -> p = NULL;
		this -> width = width;
		this -> size = width * height;

		//delete prior data if there are any
		delete[] p;
		
		p = new Type[ size ];
	}

	void fill( Type input ){
		for( int i = size; i--; ) {
			*(p+i) = input;
		}
	}

private:
	Type * p; //array data
	int width; //width of the array
	int size; //size of the array
};


int main(){
	deux<int> q(10,10);
	
	for( int i = 10; i--; ) {
		for( int j = 10; j--; ) {
//			cin >> q( i,j );
		}
	}

	cin >> q( 0,0 );

	cout << q( 0, 200 );

	return 0;
}

