#ifndef RESOURCE_MANAGER_MOD_REMAX
#define RESOURCE_MANAGER_MOD_REMAX

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;

class image_t {
public:
	image_t(){
		resLoaded = false;
		resNeeded = false;
	}

	image_t( const string& FL ){		
		fileLoc = FL;
		resLoaded = false;
		resNeeded = false;
	}

	~image_t(){

	}

	void load(){
		if( resLoaded == false ){

			if( !texture.loadFromFile( fileLoc ) ){
				cout << "ERR" << endl;
			}

			sprite.setTexture( texture );
			resLoaded = true;
		}
	}

	void setFileLoc( const string& input ){
		if( fileLoc.size() == 0 ){ //empty
			fileLoc = input;
		}
	}

	void draw_unsafe( sf::RenderWindow& App, int coor_x, int coor_y ){
		sprite.setPosition( coor_x, coor_y );
		App.draw( sprite );
	}

	void draw( sf::RenderWindow& App, int coor_x, int coor_y ){
		//load the image it's not loaded
		if( !resLoaded ){
			cout << "ABERRATION : You forgot to ManageImage()" << endl;
			cout << "ASSUMPTION : Load the image" << endl;
			
			load();
		}
		
		//just to make sure there is chance that it's not loaded by the load function
		if( resLoaded ){
			draw_unsafe( App, coor_x, coor_y );
		}
	}

	void setRect( sf::IntRect Rect ){
		//load the image it's not loaded
		if( !resLoaded ) load();

		sprite.setTextureRect( Rect );
	}

	void setOpacity( unsigned opacity ){
		if( opacity > 100 ){
			cout << "ABERRATION : Opacity cannot be more than 100%" << endl;
			cout << "ASSUMPTION : Opacity is 100%" << endl;

			opacity = 100;
		}

		sprite.setColor( sf::Color( 0xff,0xff,0xff, opacity * 256 / 100 ) );
	}
	
	void setColor( sf::Color& C ){
		sprite.setColor( C );
	}
	
	void setRotation( float rot ){
		sprite.setRotation( rot );
	}
	
	void setOrigin( float x, float y ){
		sprite.setOrigin( x, y );
	}

	bool loaded(){
		return resLoaded;
	}

	bool needed(){
		return resNeeded;
	}

	void needed( bool p ){
		resNeeded = p;
	}

	const string& fileLocation(){
		return fileLoc;
	}

	int width(){
		//load the image it's not loaded
		if( !resLoaded ) load();

		return texture.getSize().x;
	}

	int height(){
		//load the image it's not loaded
		if( !resLoaded ) load();

		return texture.getSize().y;
	}


private:
	bool resLoaded;
	bool resNeeded;
	string fileLoc;

	sf::Sprite sprite;
	sf::Texture texture;
};

class ImagePool_t {
public:
	image_t * get_image( const string& fileLoc ){
		for( unsigned i = 0; i < pool.size(); ++i ){
			if( pool[i] -> fileLocation() == fileLoc ){
				pool[i] -> needed( true );
				return pool[i];
			}
		}

		image_t * out = new image_t( fileLoc );
		out -> needed( true );
		pool.push_back( out );
		return out;
	}

	void flush(){
		for( unsigned i = pool.size(); i--; ){
			delete pool[i];
		}
		pool.clear();
	}

	void manage(){
		for( auto it = pool.begin(); it != pool.end(); ){
			if( (**it).needed() ){
				(**it).load();
				++it;
			} else {
				delete *it;
				it = pool.erase( it );
			}
		}
	}
	
	void resetNeeds(){
		for( unsigned i = pool.size(); i--; ){
			pool[i] -> needed( false );
		}
	}
	
	ImagePool_t(){}
	~ImagePool_t(){
		for( unsigned i = 0; i < pool.size(); ++i ){
			delete pool[i];
		}
	}
	
	bool check_existance( image_t * ptr ){
		for( unsigned i = 0; i < pool.size(); ++i ){
			if( pool[i] == ptr ){
				return true;
			}
		}
		return false;
	}
	
private:
	vector< image_t* > pool;
};

class sound_queue_t {
public:
	sound_queue_t(){
		Sound = new sf::Music;
		volume_ = 100.f;
	}

	void append( const string& FL ){
		fileLocs.push( FL );
	}

	void update(){
		if( Sound == NULL || Sound->getStatus() == sf::SoundSource::Status::Stopped ) next();
	}

	void next(){
		
		if( Sound != NULL && Sound->getStatus() == sf::SoundSource::Status::Playing ){
			Sound->stop();
		}

		delete Sound;
		Sound = 0;

		if( !fileLocs.empty() ){
			Sound = new sf::Music;
			Sound -> setVolume( volume_ );
			Sound -> openFromFile( fileLocs.front() );
			Sound -> play();

			fileLocs.pop();
		}
	}

	float volume( float d ){
		if( Sound != 0 ){
			Sound->setVolume( d );
		}

		volume_ = d;
		return d;
	}

	float volume(){
		return volume_;
	}
	
	void force_end(){
		if( Sound != NULL ){
			Sound->stop();
			delete Sound; Sound = NULL;
		}

		while( !fileLocs.empty() ) fileLocs.pop();
	}

private:
	sf::Music * Sound;
	float volume_;
	queue< string > fileLocs;
};

#endif

/*
int main(){
	ImagePool_t POOL;
	image_t * d = POOL.get_image("resource/B.png");

	POOL.resetNeeds();
	d->needed( true );
	POOL.manage();

//	sound_queue_t soundq;
	

	sf::RenderWindow App(sf::VideoMode( 720, 480 ), "Resource Manager Test", 1 << 2 );
	App.setFramerateLimit( 12 );

	sf::Event Event;
	while ( App.isOpen() ){
		while (App.pollEvent(Event)){
			if ( Event.type == sf::Event::Closed )
                App.close();
		}

		App.clear( sf::Color( 0xff,0xff,0xff ) );
		d->draw( App, 100, 100 );

		App.display();
	}


	return 0;
}
*/