#ifndef LITE_DB_CLASS_
#define LITE_DB_CLASS_

class LiteDB {
public:
	bool open( std::string fileLoc );
	void close();
	
	template< class T >
	std::vector<T> get( std::string table_name );
	
	void put( const void * const pointer, unsigned int size );

	template< class T >
	void put( const vector<T>& data );
	
	void flush();
};


#endif