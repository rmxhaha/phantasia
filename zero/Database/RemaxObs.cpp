#include <iostream>
#include <cmath>


using namespace std;

namespace RemaxDb {
	void encrypt( unsigned char * ptr, int size ){
		static const unsigned int prime = 224284387;
		static const unsigned int base = 707;
		static const unsigned int power = 100000000;

		int index = 0;

		
		unsigned long long a 		= power;
		unsigned long long b = 0;
		
		
		while( index < size ){	
			a *= power;

			b = a / prime;
			
			while( index < size && b > base ){
				ptr[ index++ ] += b % base;
				
				b = b / base;
			}
							
			
			a %= prime;
		}
		
		return;
	}
	
	void decrypt( unsigned char * ptr, int size ){
		static const unsigned int prime = 224284387;
		static const unsigned int base = 707;
		static const unsigned int power = 100000000;

		int index = 0;

		
		unsigned long long a 		= power;
		unsigned long long b = 0;
		
		
		while( index < size ){	
			a *= power;

			b = a / prime;
			
			while( index < size && b > base ){
				ptr[ index++ ] -= b % base;
				
				b = b / base;
			}
							
			
			a %= prime;
		}
		
		return;
	}
	
}

int main(){
	const int size = 1024;
	int * p = new int[size];
	
	for( int i = 0; i < size; ++i ) p[i] = i + 1;
	
	void * q = p;
	
	RemaxDb::encrypt( static_cast<unsigned char *>(q), size * (sizeof(int)/sizeof(unsigned char)) );
	RemaxDb::decrypt( static_cast<unsigned char *>(q), size * (sizeof(int)/sizeof(unsigned char)) );
	
	for( int i = 0; i < size; ++i ) if( p[i] != i + 1 ) cout << "ERR" << i << endl;

	cout << flush;
	
	return 0;
}