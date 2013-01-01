/*
	Eterna Memoria - Game
		Alpha Build

	Created By Synergy
		2012 - 2013
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <memory>

#include "Animation.cpp"
#include "ResourceManager.cpp"
#include "RString.cpp"

// screen settings
const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;


using namespace TweenFx;

sf::Vector2<double> Camera( 0,0 );

struct object_t {
	double x,y;
	double rot;
	sf::IntRect Rect;
	sf::Color Color;
	image_t * image;
	int zindex;

	vector< unique_ptr<tween_dt> > Tweens;
	
	object_t( const string& fileLoc, const double& x_, const double& y_, unsigned sx = 0, unsigned sy = 0, unsigned w = 0, unsigned h = 0, float opacity = 1.f ) {
		image = get_image( fileLoc );
		image -> load();

		Rect.left = sx;
		Rect.top = sy;

		Rect.width = ( w != 0 ? w : image -> width() );
		Rect.height = ( h != 0 ? h : image -> height() );

		x = x_;
		y = y_;
		Color = sf::Color( 0xff, 0xff, 0xff, 255 * opacity );
		rot = 0.0;
		zindex = 0;
	}

	void draw( sf::RenderWindow& App ) {
		image -> setOrigin( Rect.left + Rect.width/2, Rect.top + Rect.height/2 );

		image -> setColor( Color );
		image -> setRect( Rect );
		image -> setRotation( rot );

		image -> draw( App, x - Camera.x, Camera.y - y );
	}

	void setOpacity( unsigned p ) {
		if( p > 100 ) p = 100;
		
		Color.a = p * 256 / 100;
	}

	void setColour( uint8_t r,uint8_t g, uint8_t b ) {
		Color.r = r;
		Color.g = g;
		Color.b = b;
	}

	void moveTo( const double& x_, const double& y_, const double& duration, AnimationFunc_t ACalc = TweenFx::Deaccel ){
		Tweens.emplace_back( new tween_dt( x, x_ - x, ACalc, duration, 0.0, 1 ) );
		Tweens.emplace_back( new tween_dt( y, y_ - y, ACalc, duration, 0.0, 1 ) );
	}
	
	void move( const double& dx, const double& dy, const double& duration, AnimationFunc_t ACalc = TweenFx::Deaccel ){
		Tweens.emplace_back( new tween_dt( x, dx, ACalc, duration, 0.0, 1 ) );
		Tweens.emplace_back( new tween_dt( y, dy, ACalc, duration, 0.0, 1 ) );
	}
	
	void moveX( const double& dx, const double& duration, AnimationFunc_t ACalc = TweenFx::Deaccel ){
		Tweens.emplace_back( new tween_dt( x, dx, ACalc, duration, 0.0, 1 ) );
	}
	
	void moveY( const double& dy, const double& duration, AnimationFunc_t ACalc = TweenFx::Deaccel ){
		Tweens.emplace_back( new tween_dt( y, dy, ACalc, duration, 0.0, 1 ) );
	}
	
	void rotateTo( const double& rot_, const double& duration, AnimationFunc_t ACalc = TweenFx::Deaccel ){
		Tweens.emplace_back( new tween_dt( rot, rot_ - rot, ACalc, duration ) );
	}
	
	void rotate( const double& rot_, const double& duration, AnimationFunc_t ACalc = TweenFx::Deaccel ){
		Tweens.emplace_back( new tween_dt( rot, rot_, ACalc, duration ) );
	}
	
	void update( const double& dt ){
		assert( dt > 0.0 );

		// Tween Update

		auto itr = Tweens.begin();
		while( itr != Tweens.end() ){
			if( (**itr).hasFinished() ){
				/* 
					delete *itr; // prevent leak
						Not neccessary anymore since unique_ptr is used
				*/
				itr = Tweens.erase( itr );
				continue;
			}

			(**itr).update( dt );

			itr ++;
		}
	}
	
};

struct world_t {
	vector< unique_ptr<object_t> > Objects;
	
	object_t * add( object_t * in ){
		Objects.emplace_back( in );
	}
	
	void remove( unique_ptr<object_t> in ) {
		auto itr = Objects.begin();
		
		while( itr != Objects.end() ) {
			if( (*itr) == in ) {
				Objects.erase( itr );
				break;
			}

			itr++;
		}
	}
	
	void update( const double& dt ) {
		for( auto itr = Objects.begin(); itr != Objects.end(); ++itr )
		{
			(**itr).update( dt );
		}
	}
	
	void draw( sf::RenderWindow& App ){
		for( auto itr = Objects.begin(); itr != Objects.end(); ++itr )
		{
			(**itr).draw( App );
		}
	}
	
};

int main(){
	sf::RenderWindow App(sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "Eterna Memoria 1.0.1", 1 << 2 ); 
	
	// Maximum FPS
	App.setFramerateLimit( 50 ); 
	
	world_t Wx;
	object_t * obj = new object_t( "d.png", 0,0 );
	Wx.add( obj );
	
	sf::Event Event;
	sf::Clock Timer;
	double dt;
	
	while ( App.isOpen() ){
		while (App.pollEvent(Event)){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
			if( Event.type == sf::Event::KeyPressed ) {
				
				if( Event.key.code == sf::Keyboard::Return ){
					obj->moveTo( obj->x + 200, obj->y, 1.f, Deaccel );
				}
				
				if( Event.key.code == sf::Keyboard::RShift ){
					obj->moveTo( obj->x - 200, obj->y, 1.f, Deaccel );
				}
				
				if( Event.key.code == sf::Keyboard::LShift ){
					obj->rotateTo( obj->rot + 20, 1.f, Deaccel );
				}
			}
		}

		dt = Timer.getElapsedTime().asSeconds();
		Timer.restart();
		
		
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Left )) 	Camera.x -= dt * 1000.f;
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Right )) 	Camera.x += dt * 1000.f;
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ))		Camera.y += dt * 1000.f;
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Down )) 	Camera.y -= dt * 1000.f;
		
		Wx.update( dt );
		
		App.clear( sf::Color( 0xff,0xff,0xff ) );
		Wx.draw( App );

//		App.draw();

//		App.draw( VisualNovel::interface_text );

		App.display();
		
	}
	
	return 0;
}
