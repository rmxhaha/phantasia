#include <iostream>
#include <vector>

using namespace std;

int main() {
	vector< void* > p;

	p.reserve( 1000000 );

	cout << ( sizeof( void* ) + sizeof( new int ) ) * 1000000;

	cin.get(); //pts 1

	for( int i = 0; i < 1000000; i++ ){
	    p.push_back( new double( i ) );
	}

	cin.get(); //pts 2

	for( int i = 0; i < 1000000; ++i ){
		delete (double *) (p[i]);
	}

	cout << endl;
	cin.get(); //pts 3

	return 0;

}
