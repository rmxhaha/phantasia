#ifndef LITE_DB_CLASS_
#define LITE_DB_CLASS_

#include <vector>
#include <string>
#include <fstream>
#include <cassert>

using namespace std;

class LiteDB {
public:
	bool open( string fileLoc ); // close kalo msh ada yg kebuka + Buka ifstream ke file tujuan
	void close(); // Flush + Close ifstream nya
	
	template< class T >
	vector<T> get( sstring table_name ); // pake algo kemarin untuk jadiin vector
	
	void put( const void * const pointer, unsigned int size );

	template< class T >
	void put( const vector<T>& data );
	
	void flush(); // tulis ke file tapi jangan close dlu connection nya

private:
	string fileLocation;
};

struct table_jump_t {
	char name[40];
	int jump;
};

template< class T > void LiteDB::put(){
}

#endif