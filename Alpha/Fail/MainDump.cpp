#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

#include <cmath>
#include <cassert>

#include "Animation.cpp"
#include "ResourceManager.cpp"
#include "RString.cpp"

#include "Array2D.cpp"

// screen settings
const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;
const float MUSIC_VOLUME = 60.f;

#include "VisualNovel.cpp"

struct wm_tile_t
{
	wm_tile_t( const string& FL, unsigned sx_ = 0, unsigned sy_ = 0 ){ 
		fileLoc = FL;
		sx = sx_;
		sy = sy_;
	}
	
	wm_tile_t(){ 
		sx = 0;
		sy = 0;
	}
	
	string fileLoc;
	unsigned sx; // in block unit 
	unsigned sy; // in block unit
};

struct wm_hero_t {
	string fileLoc;
	unsigned x; // in block unit 
	unsigned y; // in block unit 
	unsigned sx; // in block unit 
	unsigned sy; // in block unit 
	unsigned width; // in block unit
};

template< size_t BW > // block width
class WalkMap_t {
private:
	template< size_t length >
	struct tile_t {
		tile_t( image_t * image, unsigned sx, unsigned sy )
		{
			this -> image = image;
			this -> sx = sx * length;
			this -> sy = sy * length;
		}
		
		tile_t(){}
		
		sf::IntRect Rect(){
			return sf::IntRect( sx, sy, length, length );
		}

		void preapareImage(){
			image -> setRect( Rect() );
		}
		
		unsigned sx; // in pixel unit
		unsigned sy; // in pixel unit
		image_t * image;
	};
	
	// bwidth, block width, is in px
	template< size_t bwidth >	
	struct hero_t 
	{
		hero_t( 
			unsigned x_, unsigned y_, size_t width_,
			unsigned& mw, unsigned& mh, 
			image_t * image_, int sx_, int sy_,
			Array2D<int>& hits_
		) : map_width( mw ), map_height( mh ), Hits( hits_ ) {
			animation_time = 0.f;
			step_time = 0.f;
			stop_time = 0.f;

			frame_index = 0;
			image = image_;
			
			x = x_;
			y = y_;
			width = width_;
			
			sx = sx_;
			sy = sy_;
			
			walking = false;

			Front();
		}
		
		void Left(){
			walking = true;
			direction = 1;
		}
		
		void Right(){
			walking = true;
			direction = 2;
		}
		
		void Front(){
			walking = true;
			direction = 0;
		}
		
		void Back(){
			walking = true;
			direction = 3;
		}
		
		bool collide( int x, int y )
		{
			if( x < 0 ) return true;
			if( y < 0 ) return true;
			if( x >= map_width + 1 - width ) return true;
			if( y >= map_height + 1 - width ) return true;
			
			for( int hx = 0; hx < width; ++hx ){
				for( int hy = 0; hy < width; ++hy ){
					if( Hits.get( hx+x, hy+y ) == -1 ){
						return true;
					}
				}
			}
			
			return false;
		}
		
		void update( float dt )
		{
			assert( dt >= 0.f );
			static const float AMSPF = 0.1f; // Animation Milisecond per Frame
			static const unsigned frame_limit = 3;
			static const float BPMS = 0.6f / bwidth; // block per Milisecond
			
			stop_time += dt;
			

			if( walking ){
				stop_time = 0.f;
				animation_time += dt;
				step_time += dt;
				
				while( animation_time >= AMSPF )
				{
					++frame_index;
					animation_time -= AMSPF;
				}
				
				frame_index %= frame_limit;
				
				while( step_time >= BPMS ){
					switch( direction ){
					case 0: if( !collide(x,y+1) ) y++; break;
					case 1: if( !collide(x-1,y) ) x--; break;
					case 2: if( !collide(x+1,y) ) x++; break;
					case 3: if( !collide(x,y-1) ) y--; break;
					}
					step_time -= BPMS;
				}
					
				walking = false;
			} 
			
			else if( stop_time > 0.05f ){
				step_time = 0.f;
				
				if( frame_index != 0 ){
					animation_time += dt;
					while( animation_time >= AMSPF ){
						++frame_index;
						animation_time -= AMSPF;
					}
					frame_index %= frame_limit;
				} else 	animation_time = 0.f;	
			}
		}

		void preapareImage() {
			image -> setRect( 
				sf::IntRect( 
					sx + frame_index * bwidth * width, 
					sy + direction * bwidth * width, 
					bwidth * width, 
					bwidth * width 
				) 
			);
		}
		
		int x; // in block unit  
		int y; // in block unit 
		size_t width; // in block unit		
		unsigned& map_width; // in block unit 
		unsigned& map_height; // in block unit 
		
		unsigned direction;
		float animation_time;
		float step_time;
		float stop_time;
		
		unsigned frame_index;
		
		unsigned sx; // in px
		unsigned sy; // in px
		image_t * image;
	
		Array2D<int>& Hits;
		bool walking;

	};
	
	template< size_t ewidth, size_t bwidth >
	struct enemy_t 
	{
		enemy_t( int _x, int _y, unsigned vr_, image_t * image_, int _sx, int _sy, Array2D<int>& Hits, hero_t<BW> * hero_ ) : SmartMap(Hits)
		{
			time = 0.f;
			x = _x;
			y = _y;
			sx = _sx;
			sy = _sy;
			
			view_radius = vr_;
			image = image_;
			hero = hero_;
			direction = 1;
		}
		
		void update( float dt ){
			assert( dt >= 0.f );
			
			unsigned range = 1 + ( unsigned )sqrt( pow( x - hero->x , 2 ) + pow( y - hero->y, 2 ) );
			// ceil the range
			if( range <= view_radius ){
				chase();
			} else {
				time = 0.f;
			}
		}
		
		void chase( float dt ){
			static const float AMSPF = 0.1f; // Animation Milisecond per Frame
			static const unsigned frame_limit = 3;

			time += dt;
			while( time >= Speed ){
				NextAiPath();
				time -= Speed;
			}
		}
		
		void NextAiPath()
		{
			if( x == hero->x && y == hero->y ) return;
			int min = numeric_limits<int>::max();
			
			if( SmartMap.get( x+1, y ) >= 0 ){
				min = SmartMap.get( x+1, y );
				direction = 2;
			}

			if( SmartMap.get( x-1, y ) >= 0 && min > SmartMap.get( x-1, y ) ){
				min = SmartMap.get( x-1, y );
				direction = 4;
			}

			if( SmartMap.get( x, y+1 ) >= 0 && min > SmartMap.get( x, y+1 ) ){
				direction = 3;
				min = SmartMap.get( x, y+1 );
			}

			if( SmartMap.get( x, y-1 ) >=0 && min > SmartMap.get( x, y-1 ) ){
				direction = 1;
			}
			
			assert( direction != 5 ); // invalid map

			switch( direction ){
				case 1:
					y = y - 1;
					break;
				case 2:
					x = x + 1;
					break;
				case 3:
					y = y + 1;
					break;
				case 4:
					x = x - 1;
					break;
				default: break;
			}
		}
		
		void preapareImage(){
			/*
			To do:
				Complete the enemy_t 
				Recreate the SmartMap so that it can search path for something bigger than 1 block
				Make that class still usable by all the other classes
				SmartMap class is fill able by Array2D< int > RealHits
				enemy_t to chase after the hero
				Create the map so that it's fun chasing
				make 3 enemy chase after you
				test the view_radius
				Lighting Fx
			*/
		}
		
		float Speed; // in milisecond
		float time;
		
		int x; // in blocks unit
		int y; // in blocks unit
		unsigned view_radius; // in blocks unit
		unsigned sx; // in pixel
		unsigned sy; // in pixel
		unsigned direction;
		image_t * image;
		hero_t<BW> * hero;
		Array2D< int >& SmartMap;
	};
	
public:
	WalkMap_t(
		sf::RenderWindow& App_, 
		int w, // map width 
		int h, // map height
		const vector< vector<wm_tile_t> >& layer_, 
		const vector<int>& hits_,
		const wm_hero_t& hero_
	)
	{	
		this -> Camera.x = 0;
		this -> Camera.y = 0;
		this -> map_width = w;
		this -> map_height = h;
		this -> App = &App_;

		map_size = w * h;
		
		/* Deal with the hit */
		RealHits.resize( map_width, map_height );
		SmartHits.resize( map_width, map_height );
		
		for( unsigned i = 0; i < map_size; ++i ){ RealHits[i] = hits_[i]; }
		
		/* Deal with the tiles layer */
		Tiles.resize ( layer_.size() );
		
		for( unsigned Li = 0; Li < layer_.size(); ++Li )
		{
			assert( layer_[Li].size() == map_size );

			Tiles[ Li ].resize( map_width, map_height );

			for( unsigned i = 0; i < map_size; ++i )
			{
				if( layer_[Li][i].fileLoc.size() == 0 ) {
					Tiles[Li][i] = tile_t<BW>( NULL,0,0 );
				} else {
					Tiles[Li][i] = tile_t<BW>(
						imagePool.get_image( layer_[Li][i].fileLoc ), 
						layer_[Li][i].sx, 
						layer_[Li][i].sy 
					);
				}
			}
		}
		
		/* Deal with the hero sprite */
		hero = new hero_t<BW>( 
			hero_.x,
			hero_.y,
			hero_.width,
			map_width,
			map_height,
			imagePool.get_image( hero_.fileLoc ),
			hero_.sx * BW,
			hero_.sy * BW,
			RealHits
		);
	}
	
	~WalkMap_t(){
		delete hero; 
	}
	
	void update( float dt ){
		/* Handling Events */
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ) hero->Right();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) ) hero->Left();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ) hero->Back();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ) hero->Front();
		
		
		/* Update Smart Map */

		// Fill The Hits First		
		fillSmartHits( hero->x, hero->y );
		
		/* Do Hero update */
		hero->update( dt );		
		
		Camera.x = hero->x * BW - WINDOW_WIDTH/2;
		Camera.y = hero->y * BW - WINDOW_HEIGHT/2;
	}
	
	void draw()
	{
		unsigned mh = map_height * BW;
		unsigned mw = map_width * BW;
		
		unsigned rx,ry;
		unsigned li;
		unsigned index = 0;
		
		for( ry = 0; ry < mh; ry += BW ){
			for( rx = 0; rx < mw; rx += BW ){
				for( li = 0; li < Tiles.size(); ++li ) {
					if( Tiles[ li ][ index ].image != NULL ){
						Tiles[ li ][ index ].preapareImage();
						Tiles[ li ][ index ].image -> draw( *App, rx - Camera.x, ry - Camera.y );
					}
				}

				++index;
			}
		}
		
		hero-> preapareImage();
		hero-> image -> draw( *App, hero->x * BW - Camera.x, hero->y * BW - Camera.y );
	}
	
private:



/* Smart Hits Filling Algorithm */

	struct coor_t {
		coor_t( short _x, short _y ){
			x = _x;
			y = _y;
		}

		short x;
		short y;
	};

	queue<coor_t> fillQueue;

	void qpush( short x, short y, int val ) {
		if( 0 <= x && 0 <= y && x < map_width && y < map_height ) {
			if( SmartHits.get( x, y ) > val ){
				SmartHits.get( x, y ) = val;
				fillQueue.push( coor_t( x, y ) );
			}
		}
	}

	void fillSmartHits( int hx, int hy ){
		for( int i = 0; i < map_size; ++i ){
			if( RealHits[i] != -1 ){
				SmartHits[i] = numeric_limits<int>::max();
			} else {
				SmartHits[i] = -1;
			}
		}
		
		for( unsigned x = 0; x < hero->width; ++x ){
			for( unsigned y = 0; y < hero->width; ++y ){
				qpush( x+hx, y+hy, 1 );
			}
		}
		

		int p;
		while( ! fillQueue.empty() ){
			p = SmartHits.get( fillQueue.front().x, fillQueue.front().y ) + 1;
			qpush ( fillQueue.front().x + 1, fillQueue.front().y, p );
			qpush ( fillQueue.front().x - 1, fillQueue.front().y, p );
			qpush ( fillQueue.front().x, fillQueue.front().y + 1, p );
			qpush ( fillQueue.front().x, fillQueue.front().y - 1, p );

			fillQueue.pop();
		}
	}



/* Variables Declaration */
	unsigned map_width;
	unsigned map_height;
	unsigned map_size;
	
	sf::Vector2< int > Camera;
	Array2D< int > RealHits;
	Array2D< int > SmartHits;
	vector< Array2D< tile_t<BW> > > Tiles; // kinda look like inception, I should make more template within template
	
	ImagePool_t imagePool;
	hero_t<BW> * hero;
	
	sf::RenderWindow * App;
};

int main( int argc, char ** argv ){
	sf::RenderWindow App ( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "Eterna Memoria 1.0.1 Alpha Build", 1 << 2 );
	sf::Clock Timer;
	sf::Event Event;
	
	App.setFramerateLimit( 24 );

	const unsigned mw = 30;
	const unsigned mh = 30;
	
	vector< vector<wm_tile_t> > datax( 2, vector<wm_tile_t>( mw * mh ) );
	
	for( unsigned x = 0; x < mw * mh; ++x )
	{
		datax[ 0 ][ x ] = wm_tile_t( "resource/Outside_A2.png", 0, 2 );
		datax[ 1 ][ x ] = wm_tile_t( "", 0, 0 );
	}
	
	datax[0][10] = wm_tile_t( "resource/Outside_A2.png", 5, 2 );
	datax[1][15] = wm_tile_t( "resource/Outside_A2.png", 8, 0 );
	
	vector<int> HITS( mw * mh, 0 );

	char p[mh][mw+1] = {
		"                              ",
		"                              ",
		"                              ",
		"                              ",
		"  #                           ",
		"  #                           ",
		"                              ",
		"  #                           ",
		"#                             ",
		"#                             ",
		"#                             ",
		"#                             ",
		"#             ##   #          ",
		"#                             ",
		"#                             ",
		"#          #####              ",
		"#                             ",
		"#                             ",
		"#                             ",
		"#                             ",
	};

	int ddx = 0;
	for( int x = 0; x < mw; ++x ){
		for( int y = 0; y < mh; ++ y ) {
			if( p[x][y] == '#' ){
				datax[1][ddx] = wm_tile_t( "resource/Outside_A2.png", 12, 0 );
				HITS[ddx] = -1;
			}
			ddx++;
		}
	}
	
	wm_hero_t hero_setup;
	hero_setup.fileLoc = "resource/People8.png";
	hero_setup.x = 0;
	hero_setup.y = 0;
	hero_setup.sx = 6;
	hero_setup.sy = 8;
	hero_setup.width = 2;
	
	WalkMap_t<16> WM( App, mw, mh, datax, HITS, hero_setup );
	
	
	while ( App.isOpen() ){
		while (App.pollEvent(Event)){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
		}
		float dt = Timer.getElapsedTime().asSeconds();
		Timer.restart();
		WM.update( dt );
		
		App.clear( sf::Color( 0,0,0 ) );
		
		WM.draw();
		
		App.display();
	}
	
	return 0;
}


