#include <iostream>#include <vector>using namespace std;//	for using the old depricated vector<bool>	#define STD_VECTOR_BOOL_SPECIAL//	for not using the old depricated vector<bool>//	#define STD_VECTOR_BOOL_NOT_SPECIALtemplate<class Type>class Array2D {	typedef unsigned int uint;public:	Array2D( int w, int h ) : data( array ) {		width = w;		height = h;		array.resize( w * h );	}	Array2D() : data( array ){		width = 0;		height = 0;	}	Array2D<Type> submap( uint from_x_, uint from_y, uint w, uint h ) const {		Array2D<Type> output( w, h );		w += from_x_;		h += from_y;		if( w > width ) w = width;		if( h > height ) h = height;		int i = 0,			from_x;		for( ; from_y < h; ++from_y ){			for( from_x = from_x_ ; from_x < w; ++from_x ){				output.get(i) = this -> get( from_x, from_y );				i++;			}		}		return output;	}	Type& get( const uint x, const uint y ) {		return array[ y * width + x ];	}	Type& get( const uint index ) {		return array[ index ];	}	Type& operator[] ( const uint index ) {		return array[ index ];	}	void print() const {		for( int i = 0; i < array.size(); ++i ){			if( i % width == 0 ) cout << endl;			cout << array[i];		}		cout << endl;	}	void print( int fx_ , int fy, int w = 0, int h = 0 ) const {		int fx;		int tmp_index;		w = w + fx_;		h = h + fy;		for( ; fy < h; ++fy ){			tmp_index = fy * width;			for( fx = fx_; fx < w; ++fx ){				cout << array[ tmp_index + fx ];			}			cout << endl;		}	}	void resize( uint w, uint h ){		if( w != width && h != height ){			vector<Type> temp( w * h );			int lx = ( w < width ? w : width );			int ly = ( h < height ? h : height );			int x, y, temp_index, temp_index_2;			for( y = 0; y < ly; ++y ){				temp_index = y * width;				temp_index_2 = y * w;				for( x = 0; x < lx; ++x ){					temp[ temp_index_2 + x ] = array[ temp_index + x ];				}			}			//replace the array			array = temp;			//also replace the old config			width = w;			height = h;		}	}	void fill( const Type& input ){		for( auto itr = array.begin(); itr != array.end(); ++itr ){			*itr = input;		}	}	//Outside Read access	const vector<Type>& data;private:	int width;	int height;	vector<Type> array;};/*	Note :		Since Array2D use std vector, if Array2D<boo> is used, you cannot dereference a member.		The data can only be retrive because of the vector<bool> specialization ( 1 bit per bool ) instead ( 1 byte per bool ).*/