template<class T>
class Array2D {
public:
	T& operator() ( int x, int y ){
		if( y * width + x < size && x < width ) {
			return *( p + y * width + x );
		} else {
			return *( p ); //error
		}
	}
	
	T& operator[] ( unsigned int index ){
		if( index < w * h ) 
			return *( _data + index );
		else
			return *( _data + index	);
	}
	
	
	Array2D(){
		//blank
	}
	
	Array2D( int w, int h ){
		redefine( w, h );
	}
	
	void redefine( int w, int h ){
		this -> w = w;
		this -> h = h;
	
		allocate();
	}
	
	~Array2D(){
		delete[] _data;
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
	void allocate(){
		delete[] _data;
		_data = new T[ w*h ];
	}
	
	T * _data;
	int w;
	int h;
};
