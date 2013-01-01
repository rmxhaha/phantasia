/*
	RImage 1.0.0
	
	Release Note:
		The new image_t type for handling image resources
		function get_image( std::string ) for portablty in getting the specific image_t 
		
		
		tile_t 		build for dividing image
		sprite_t 	build for sprite animation

		imagePool 	keep a collection of image_t
		tilePool 	keep a collection of tile_t
		spritePool 	keep a collection of sprite_t

	Created By Remax - Synergy
		2012
	*/

#ifndef REMAX_TILE_SPRITE_CLASS
#define REMAX_TILE_SPRITE_CLASS
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <SFML/Graphics.hpp>

using namespace std;

/* Prior Decleration */
class image_t;
class tile_t;
class sprite_t;

/* Pool for better access */
vector<image_t *> imagePool;
vector<tile_t *> tilePool;
vector<sprite_t *> spritePool;

/*
	class image_t
		Used for keeping the image singularity. So there won't be any duplicate
		"get_image" is used for this capability of not having any duplicate image resources in the program
		
		using a pointer like this one bellow
		image_t * p = get_image( fileLocation );
		is recommended
*/
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

	void reset(){
		resLoaded = false;
		fileLoc.resize( 0 );
		unload();
	}

	void unload(){
		if( resLoaded ){
			texture.loadFromFile(""); //release it
			resLoaded = false;
		}
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

/*
	ResetImageNeeds
		Called change of mode is occuring
		This function resets the needs of image resources
		
		Note : Declare the needs after the resets
		
		"ManageImage" must be called after all the image necesity are declared
*/

void ResetImageNeeds(){
	for( int i = 0; i < imagePool.size(); ++i ){
		imagePool[i] -> needed( false );
	}
}

/*
	ManageImage
		Called after all needs are declared
		This function unload all unnecessary image resources
		and loads necessary image resources that haven't been loaded to memory
*/

void ManageImage(){
	for( int i = 0; i < imagePool.size(); ++i ){
		if( imagePool[i] -> needed() ) {
			imagePool[i] -> load();
		} else {
			imagePool[i] -> unload();
		}
	}
}

/*
	"get_image" will find the images of the url if there is any.
	Basiclly, get_image function will return you a pointer address of an image_t
	from the array ( vectorPool ) to prevent image resource duplication.
*/

image_t * get_image( const string& fileLoc ){
	for( int i = 0; i < imagePool.size(); ++i ){
		if( imagePool[i] -> fileLocation() == fileLoc ){
			return imagePool[i];
		}
	}
	
	imagePool.push_back( new image_t( fileLoc ) );	
	return imagePool.back();
}

/*
	"flush_image" erases all the resources
	this function shouldn't not be used unless it's a special case where this is necessary
	use "ManageImage" instead for common use
*/

void flush_image(){
	for( int i = imagePool.size(); i--; ){
		imagePool[i] -> unload();
	}
}

class tile_t {
public:
	tile_t( image_t * Image, int dx = 1, int dy = 1 ) {
		width = 0;
		height = 0;
		width_n = 0;
		height_n = 0;

		image = Image;
		if( !image -> loaded() ) image -> load();
		divide( dx, dy );
	}

	tile_t( const string& FL, int dx = 1, int dy = 1 ){
		width = 0;
		height = 0;
		width_n = 0;
		height_n = 0;

		image = get_image( FL );
		image -> load();
		divide( dx, dy );
	}

	void divide( int _nx, int _ny ){
		width_n = _nx;
		height_n = _ny;
		width = image -> width() / _nx;
		height = image -> height() / _ny;
	}

	//make sure image is loaded
	void draw( sf::RenderWindow& App, int _x, int _y, int index_x, int index_y ){
		index_x %= width_n;
		index_y %= height_n;
		if( !image -> loaded() ) image -> load();

		image -> setRect( sf::IntRect(index_x * width, index_y * height, width, height ) );
		image -> draw( App, _x, _y );
	}

	void draw( sf::RenderWindow& App, int _x, int _y ){
		if( !image -> loaded() ) image -> load();

		image -> setRect( sf::IntRect( 0, 0, width, height ) );
		image -> draw( App, _x, _y );
	}

	const string& fileLocation() const {
		return image -> fileLocation();
	}

private:
	image_t * image;
	int width;
	int height;
	int width_n;
	int height_n;
};

class sprite_t {
public:
	sprite_t( image_t * Image, int _width_n ){
		image = Image;
		if( !image -> loaded() ) image -> load();

		this -> width = image -> width() / width_n;
		this -> width_n = width_n;
	}

	sprite_t( const string& FL, int width_n ){
		image = get_image( FL );
		image -> load();


		this -> width = image -> width() / width_n;
		this -> width_n = width_n;
	}

	void draw( sf::RenderWindow& App, int coor_x, int coor_y, int index = 0 ) {
		index %= width_n;

		image -> setRect( sf::IntRect( index * width, 0, width, image -> height() ) );
		image -> draw( App, coor_x, coor_y );
	}

	const string& fileLocation(){
		return image -> fileLocation();
	}

private:
	int width;
	int width_n;

	image_t * image;
};

tile_t * new_tile( const string& fileLoc, int width_n, int height_n ) {
	tilePool.push_back( new tile_t( fileLoc, width_n, height_n ) );
	return tilePool.back();
}

sprite_t * new_sprite( const string& fileLoc, int width_n ) {
	spritePool.push_back( new sprite_t( fileLoc, width_n ) );
	return spritePool.back();
}

tile_t * get_tile( const string& fileLoc ) {
	int i = 0;
	for( ; i < tilePool.size(); ++i ){
		if( tilePool[i] -> fileLocation() == fileLoc )
			return tilePool[i];
	}

	return 0;
}

sprite_t * get_sprite( const string& fileLoc ) {
	int i = 0;
	for( ; i < spritePool.size(); ++i ){
		if( spritePool[i] -> fileLocation() == fileLoc )
			return spritePool[i];
	}

	return 0;
}

void clear_tile(){
	for( int i = tilePool.size(); i--; ) {
		//there is no need to clear image

		//delete the resources
		delete tilePool[i];
	}

	//clear the pointer
	tilePool.clear();
}

void clear_sprite(){
	for( int i = spritePool.size(); i--; ) {
		//there is no need to clear image

		//delete the resources
		delete spritePool[i];
	}

	//clear the pointer
	spritePool.clear();

}

#endif
