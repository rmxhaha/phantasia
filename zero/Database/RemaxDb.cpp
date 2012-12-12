/*
	By Remax - Synergy
	*/

#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>
#include <vector>

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
	
	void FastFileWrite( const string& filename, void * ptr, int size ){
		FILE * pFile = fopen( filename.c_str(), "wb" );
		fwrite( ptr, 1, size, pFile );
		fclose( pFile );
	}

	void FastFileWrite( const string& filename, const string& buffer ){
		FILE * pFile = fopen( filename.c_str(), "wb" );
		fwrite( buffer.c_str(), 1, buffer.size(), pFile );
		fclose( pFile );
	}

	template< class T > void FastWrite( const string& filename, vector<T>& array ){
		RemaxDb::encrypt( (unsigned char *)array.data(), array.size() * (sizeof(T)/sizeof(unsigned char)) );

		FastFileWrite( filename, (void *)array.data(), array.size() * ( sizeof(T) / sizeof(void) ) );
	}

	template< class T > struct buffer_t {
		buffer_t(){}

		buffer_t( void * ptr, int s ){
			set( ptr, s );
		}

		void set( void * ptr, int s ) // size in char
		{
			size = s / ( sizeof(T)/sizeof(void) );
			buffer = (T *) ptr;
		}

		long size;
		T * buffer;
	};

	template< class T >
	buffer_t<T> FileRead( const string& filename ){
		ifstream in( filename, std::ios::in | std::ios::binary );
		if ( in )
		{
			char * buffer;
			int size;

			in.seekg(0, std::ios::end);
			size = in.tellg();
			in.seekg(0, std::ios::beg);

			buffer = new char[ size ]; //no need for null terminator

			in.read( buffer, size );
			in.close();

			RemaxDb::decrypt( (unsigned char*) buffer, size );

			return buffer_t<T> ( buffer, size );
		}

		throw(errno);

	}
	
}

struct two {
	int a;
	int b;
};

int main(){
	const int data_size = 1 << 18;
//	int * data = new int[ data_size ]; // 1MB
	vector<int> data( data_size );

	for( int i = 0; i < data_size; i++ ){
		data[i] = i + 1;
	}

//	void * p = static_cast<void *>( data );

//	RemaxDb::FastFileWrite( "Data", static_cast<char *>(p), data_size * ( sizeof(int) / sizeof(char) ) );
	RemaxDb::FastWrite<int>( "Data", data );
	
	/* read */

	RemaxDb::buffer_t<two> lol = RemaxDb::FileRead<two>( "Data" );


	for( int i = 0; i < lol.size; i++ ){
//		cout << lol.buffer[i].a << '\n';
//		cout << lol.buffer[i].b << '\n';
	}
	cout << flush;
	
	return 0;
}
