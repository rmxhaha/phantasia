#include <SFML/Graphics.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <vector>

#include <cmath>
#include <cassert>

using namespace std;

#include "Animation.cpp"
#include "ResourceManager.cpp"
#include "RString.cpp"

#include "Array2D.cpp"

// screen settings
const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;
const float MUSIC_VOLUME = 60.f;

#include "VisualNovel.cpp"

enum mods_t {
	MOD_VISUAL_NOVEL,
	MOD_WALKMAP
} now_mod;

void chmod( mods_t M ){
	now_mod = M;
}


struct wm_tile_t
{
	wm_tile_t( const string& FL, int x_, int y_, const sf::IntRect& Rect_ )
	: fileLoc(FL), Rect( Rect_ ) {
		x = x_;
		y = y_;
	}

	wm_tile_t(){ }

	int x;
	int y;
	string fileLoc;
	sf::IntRect Rect;
};

struct wm_hero_t {
	string fileLoc;
	int x; // in block unit
	int y; // in block unit
	int sx; // in px unit
	int sy; // in px unit
	int width; // in block unit
};

struct wm_enemy_t {
	string fileLoc;
	int x;
	int y;
	int sx;
	int sy;
	int width;
	int vr;
	float speed;
};

/*
To do WalkMap_t:
	use Animation.cpp
	Lighting Fx
	sf::Music
*/

template< size_t BLOCK_WIDTH >
class WalkMap_t {
private:
	class SmartMap_t;
	class hero_t;
	class tile_t;
	class enemy_t;

	class SmartMap_t {
	public:
		SmartMap_t( hero_t * hero_ ){
			hero = hero_;
			map_width = 0;
			map_height = 0;
		}

		SmartMap_t(){}

		~SmartMap_t(){

		}

		void update( Array2D< short >& Obs_ ) {
			Obs = Obs_;
			map_width = Obs.width_();
			map_height = Obs.height_();

			Smart.clear(); // remove everything
		}

		int getAiPath( int _x, int _y, int _w ){
			if( collideWithHero( _x, _y, _w ) ) return 6; // Destination Reached

			/* Check the exisiting solution */
			if( Smart.size() > ( unsigned  ) _w && Smart[ _w ].width_() != 0 )
			{
				return NextAiPath( Smart[ _w ], _x, _y, _w );
			}

			/* Search the solution because the solution doesn't exist yet */
			if( Smart.size() <= (unsigned) _w ) Smart.resize( _w + 1 );

			fillPath( _w );

			return NextAiPath( Smart[ _w ], _x, _y, _w );
		}
	private:

		int NextAiPath( Array2D< int >& SmartMap, int x, int y, int _w ){
			int min = numeric_limits<int>::max();
			int direction = 5;

			if( x + 1 < map_width + 1 - _w && SmartMap.get( x + 1, y ) >= 0 ){
				min = SmartMap.get( x+1, y );
				direction = 2;
			}

			if( 0 <= x - 1 && SmartMap.get( x - 1, y ) >= 0 && min > SmartMap.get( x - 1, y ) ){
				min = SmartMap.get( x-1, y );
				direction = 4;
			}

			if( y + 1 < map_height + 1 - _w && SmartMap.get( x, y + 1 ) >= 0 && min > SmartMap.get( x, y + 1 ) ){
				direction = 3;
				min = SmartMap.get( x, y+1 );
			}

			if( 0 <= y - 1 && SmartMap.get( x, y - 1 ) >=0 && min > SmartMap.get( x, y-1 ) ){
				direction = 1;
			}

			return direction;
		}

		bool collideWithHero( int _x, int _y, int _w ){
			if( hero->x + hero->width < _x ) 	return false;
			if( _x + _w < hero->x ) 			return false;
			if( hero->y + hero->width < _y ) 	return false;
			if( _y + _w < hero->y ) 			return false;

			return true;
		}


		struct coor_t {
			coor_t( short _x, short _y ){
				x = _x;
				y = _y;
			}

			short x;
			short y;
		};

		void fillPath( int _w ){
			queue<coor_t> fillQueue;
			Array2D< int > * target_map = &Smart[ _w ];
			int mw = map_width + 1 - _w;
			int mh = map_height + 1 - _w;

			target_map->resize( mw, mh );

			int x,y,lx,ly,vx,vy;

			for( x = 0; x < mw; ++x ){
				for( y = 0; y < mh; ++y ){
					if( Obs.get( x,y ) != -1 ){
						target_map -> get( x, y ) = numeric_limits<int>::max();
					} else {

						lx = ( x - _w + 1 < 0  ? x + 1 : _w  );
						ly = ( y - _w + 1 < 0  ? y + 1 : _w  );

						for( vx = 0; vx < lx; ++vx ){
							for( vy = 0; vy < ly; ++vy ){
								target_map -> get( x - vx, y - vy ) = -1;
							}
						}
					}
				}
			}

			for( int x = 0; x < hero->width; ++x ){
				for( int y = 0; y < hero->width; ++y ){
					target_map -> get( hero->x + x, hero->y + y ) = 1;
					fillQueue.push( coor_t( hero->x + x, hero->y + y ) );
				}
			}




			int val;
			while( ! fillQueue.empty() ){
				val = target_map->get( fillQueue.front().x, fillQueue.front().y ) + 1;

				const int& x = fillQueue.front().x;
				const int& y = fillQueue.front().y;

				if( 0 <= x - 1 ){
					if( target_map->get( x - 1, y ) > val ){
						target_map->get( x - 1, y ) = val;
						fillQueue.push( coor_t( x - 1, y ) );
					}
				}

				if( x + 1 < mw ){
					if( target_map->get( x + 1, y ) > val ){
						target_map->get( x + 1, y ) = val;
						fillQueue.push( coor_t( x + 1, y ) );
					}
				}

				if( 0 <= y - 1 ){
					if( target_map->get( x, y - 1 ) > val ){
						target_map->get( x, y - 1 ) = val;
						fillQueue.push( coor_t( x, y - 1 ) );
					}
				}

				if( y + 1 < mh ){
					if( target_map->get( x, y + 1 ) > val ){
						target_map->get( x, y + 1 ) = val;
						fillQueue.push( coor_t( x, y + 1 ) );
					}
				}

				fillQueue.pop();
			}

			target_map = NULL;
		}

		int map_width;
		int map_height;

		hero_t * hero;

		Array2D< short > Obs;
		vector< Array2D< int > > Smart;
	};

	struct tile_t {
		tile_t( image_t * image_, const int& x_, const int& y_, const sf::IntRect& R ) : Rect( R ){
			x = x_;
			y = y_;
			image = image_;
		}
		tile_t(){
			x = 0;
			y = 0;
		}

		void preapareImage(){
			image -> setRect( Rect );
		}

		int x; // in px
		int y; // in px
		image_t * image;
		sf::IntRect Rect;
	};

	struct hero_t
	{
		hero_t(
			int x_, int y_, int width_,
			int& mw, int& mh,
			image_t * image_, int sx_, int sy_,
			Array2D< short >& hits_
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
			if( x >= int( map_width + 1 - width ) ) return true;
			if( y >= int( map_height + 1 - width ) ) return true;

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
			static const int frame_limit = 3;
			static const float BPMS = 3.f / BLOCK_WIDTH; // block per Milisecond

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
					sx + frame_index * BLOCK_WIDTH * width,
					sy + direction * BLOCK_WIDTH * width,
					BLOCK_WIDTH * width,
					BLOCK_WIDTH * width
				)
			);
		}

		int x; // in block unit
		int y; // in block unit
		int width; // in block unit
		int& map_width; // in block unit
		int& map_height; // in block unit

		int direction;
		float animation_time;
		float step_time;
		float stop_time;

		int frame_index;

		int sx; // in px
		int sy; // in px
		image_t * image;

		Array2D< short >& Hits;
		bool walking;
	};

	struct enemy_t {
		enemy_t(
			image_t * image_,
			int _x, int _y,
			int sx_, int sy_,
			int _width, int vr, float speed
		)

		: 	x( _x ), y( _y ),
			sx( sx_ ), sy( sy_ ),
			width( _width ), view_radius(vr), velocity ( speed )
		{
			image = image_;
			step_time = 0.f;
			animation_time = 0.f;
		}


		void preapareImage(){
			const static int sprite_map[] = { -1, 3, 2,0,1 };
			const static int frame_limit = 3;
			image -> setRect( sf::IntRect(
				( frame_index % frame_limit ) * BLOCK_WIDTH + sx,
				sprite_map[direction] * BLOCK_WIDTH + sy,
				width * BLOCK_WIDTH,
				width * BLOCK_WIDTH
				)
			);
		}

		int x;
		int y;
		int sx;
		int sy;
		int width;
		int view_radius;

		float velocity;
		float step_time;
		float animation_time;
		int direction;
		int frame_index;

		image_t * image;
	};

	vector< enemy_t > Enemies;

	template< class T >
	bool AiChasing( T& enm ){
		int range = ( int ) sqrt( pow( hero->x - enm->x, 2 ) + pow( hero->y - enm->y, 2 ) );
		return ( range <= enm->view_radius );
	}

	template< class T >
	bool collideWithHero( T& enm ){
		if( hero->x + int( hero->width ) < enm->x  ) return false;
		if(  enm->x + int( enm->width  ) < hero->x ) return false;
		if( hero->y + int( hero->width ) < enm->y  ) return false;
		if(  enm->y + int( enm->width  ) < hero->y ) return false;

		return true;
	}

	void updateAi( float dt )
	{
		static const float AMSPF = 0.1f;

		for( auto it = Enemies.begin(); it != Enemies.end(); ++it ){
			if( AiChasing( it ) ){
				it->step_time  += dt;
				it->animation_time  += dt;

				while( it->step_time >= it->velocity ){
					int dir = Smart.getAiPath( it->x, it->y, it->width );

					switch ( dir ){
					case 1: it->y--;
						break;
					case 2: it->x++;
						break;
					case 3: it->y++;
						break;
					case 4: it->x--;
						break;
					default: dir = 1;
						break;
					}

					it->direction = dir;

					if( collideWithHero( it ) ){
						chmod( MOD_VISUAL_NOVEL );
					}

					it->step_time -= it->velocity;
				}

				while( it->animation_time >= AMSPF ){
					it->frame_index++;
					it->animation_time -= AMSPF;
				}

			} else {
				it -> step_time = 0.f;
			}
		}
	}


public:
	WalkMap_t(
		sf::RenderWindow& App_,
		int w, // map width in block
		int h, // map height in block
		const vector< wm_tile_t >& layer_,
		const Array2D< short >& Obs,
		const wm_hero_t& hero_,
		const vector< wm_enemy_t >& Enemies_
	) {

		Camera.x = 0;
		Camera.y = 0;
		map_width = w;
		map_height = h;

		MapObstacle = Obs;

		App = &App_;

		map_size = w * h;

		/* Deal with the tiles layer */

		Tiles.resize( layer_.size() );

		for( unsigned i = 0; i < layer_.size(); ++i ){
			Tiles[i] = tile_t(
				imagePool.get_image( layer_[i].fileLoc ),
				layer_[i].x,
				layer_[i].y,
				layer_[i].Rect
			);
		}

		hero = new hero_t(
			hero_.x,
			hero_.y,
			hero_.width,
			map_width,
			map_height,
			imagePool.get_image( hero_.fileLoc ),
			hero_.sx, hero_.sy,
			MapObstacle
		);

		Smart = SmartMap_t( hero );

		for( auto it = Enemies_.begin(); it != Enemies_.end(); ++it ){
			Enemies.push_back(
				enemy_t(
					imagePool.get_image( it -> fileLoc ),
					it -> x,
					it -> y,
					it -> sx,
					it -> sy,
					it -> width,
					it -> vr,
					it -> speed
				)
			);
		}
	}

	void update( float dt ){
		/* Handling Events */
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) ) hero->Right();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) ) hero->Left();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) ) hero->Back();
		if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ) hero->Front();

		Array2D< short > MpObs = MapObstacle;
		for( unsigned i = 0; i < Enemies.size(); ++i ){
			for( int x = Enemies[i].x; x < Enemies[i].x + Enemies[i].width; ++x ){
				for( int y = Enemies[i].y; y < Enemies[i].y + Enemies[i].width; ++y ){
					MpObs.get( x, y ) = -1;
				}
			}
		}

		Smart.update( MpObs );

		updateAi( dt );

		hero -> update( dt );

		Camera.x = hero->x * BLOCK_WIDTH - WINDOW_WIDTH/2;
		Camera.y = hero->y * BLOCK_WIDTH - WINDOW_HEIGHT/2;
	}

	void draw(){
		for( unsigned i = 0; i < Tiles.size(); ++i ) {
			Tiles[i].preapareImage();
			Tiles[i].image -> draw( *App, Tiles[i].x - Camera.x, Tiles[i].y - Camera.y );
		}

		for( unsigned i = 0; i < Enemies.size(); ++i ){
			Enemies[i].preapareImage();
			Enemies[i].image -> draw( *App, Enemies[i].x * BLOCK_WIDTH - Camera.x, Enemies[i].y * BLOCK_WIDTH - Camera.y );
		}

		hero -> preapareImage();
		hero -> image -> draw( *App, hero -> x * BLOCK_WIDTH - Camera.x, hero -> y * BLOCK_WIDTH - Camera.y );
	}


private:
	/* Variables Declaration */
	int map_width;
	int map_height;
	int map_size;

	sf::Vector2< int > Camera;

	vector< tile_t > Tiles;
	Array2D< short > MapObstacle;
	hero_t * hero;

	ImagePool_t imagePool;
	SmartMap_t Smart;

	sf::RenderWindow * App;
};

int main(){
	sf::RenderWindow App ( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "Eterna Memoria 1.0.1 Alpha Build", 1 << 2 );
	sf::Clock Timer;
	sf::Event Event;

	App.setFramerateLimit( 24 );

/* Data Init */
	const int map_width = 30;
	const int map_height = 30;
	const int BLOCK_WIDTH = 32;

	vector< wm_tile_t > tileP;

	for( int y = 0; y < map_height; y += 64 / BLOCK_WIDTH ){
		for( int x = 0; x < map_width; x += 64 / BLOCK_WIDTH ){
			tileP.push_back(
				wm_tile_t( "resource/Outside_A2.png", x * BLOCK_WIDTH, y * BLOCK_WIDTH, sf::IntRect( 0,0, 64,64 ) )
			);
		}
	}

	tileP.push_back( wm_tile_t( "resource/Outside_A2.png", 29 * BLOCK_WIDTH, 0, sf::IntRect( 5 * BLOCK_WIDTH,0, 128, 128 ) ) );

	char mp[ map_height ][ map_width+1 ] = {
		"                 #            ", // 0
		"                 #            ", // 1
		"                 #     # #####", // 2
		"                 #     # #    ", // 3
		"  #  ##   ####   ##### # # #  ", // 4
		"  #       ###  ###   # # # #  ", // 5
		"                   #   # # #  ", // 6
		"  #######          ##### # # #", // 7
		"#       #              # # # #", // 8
		"#    #  ##########     # # #  ", // 9
		"#    #           ### ### # #  ", // 10
		"#    ##########    #       #  ", // 11
		"#             ##   ## ######  ", // 12
		"#  ###### ##   #      #    #  ", // 13
		"#          #   #  #####    #  ", // 14
		"########  ##  ###          #  ", // 15
		"#        ##        ##  #   #  ", // 16
		"#   ######      #   #         ", // 17
		"#   #           #             ", // 18
		"#                             ", // 19
		"              ####            ", // 20
		"                              ", // 21
		"########  ####### ##  ########", // 22
		"          # #   # #           ", // 23
		"  #   ##      ### ########    ", // 24
		"  ##########        #    #    ", // 25
		"      #       ##### # #  #  # ", // 26
		" ##   #    ###      # #     # ", // 27
		"#   ##            ### ####### ", // 28
		"#                             "  // 29
	};



	Array2D< short > Hits;

	Hits.resize( map_width, map_height );
	for( int y = 0; y < map_height; ++ y ){
		for( int x = 0; x < map_width; ++x ){
			if( mp[y][x] == '#' ){
				Hits.get( x, y ) = -1;
				tileP.push_back(
					wm_tile_t( "resource/Outside_A2.png", x * BLOCK_WIDTH, y * BLOCK_WIDTH, sf::IntRect( 12 * BLOCK_WIDTH, 0, 32,32 ) )
				);
			} else {
				Hits.get( x, y ) = 0;
			}
		}
	}

	wm_hero_t hero;
	hero.fileLoc = "resource/People8.png";
	hero.x = 2;
	hero.y = 10;
	hero.sx = 96;
	hero.sy = 128;
	hero.width = 1;

	vector< wm_enemy_t > ENM;
	wm_enemy_t tmp_enm;

	tmp_enm.x = 15;
	tmp_enm.y = 0;
	tmp_enm.width = 1;
	tmp_enm.fileLoc = "resource/People8.png";
	tmp_enm.sx = 192;
	tmp_enm.sy = 128;
	tmp_enm.vr = 15;
	tmp_enm.speed = 0.3f;

	ENM.push_back( tmp_enm );

	tmp_enm.x = 27;
	tmp_enm.y = 20;

	ENM.push_back( tmp_enm );

	tmp_enm.x = 13;
	tmp_enm.y = 10;

	ENM.push_back( tmp_enm );

/* Data Init end */
	WalkMap_t< BLOCK_WIDTH > _WM_ ( App, map_width, map_height, tileP, Hits, hero, ENM );



/* Data init 2 */
	vector< log_t > logs;
	log_t temp_log;

	temp_log.name = "Reindeer";
	temp_log.text = "Where have you been, Santa ? ";
	temp_log.music_path = "resource/Dungeon1.ogg";
	temp_log.CG_path = "resource/Apple_Wallpaper_Template_by_my_nightmare_reborn.jpg";

	temp_log.image_path.push_back( "resource/A.png" );
	temp_log.image_align.push_back( 1 );

	logs.push_back( temp_log );

	temp_log.name = "Santa";
	temp_log.text = "Ho ho ho ho, I overslept sorry. What time is now ? ";

	logs.push_back( temp_log );

	temp_log.name = "Reindeer";
	temp_log.text = "It's summer already and you are 10 years late !";

	logs.push_back( temp_log );

	temp_log.name = "Santa";
	temp_log.text = "I guess I'll become the mid summer santa... ho ho ho ho ";

	logs.push_back( temp_log );

	temp_log.name = "Reindeer";
	temp_log.text = "You trust me ?";

	logs.push_back( temp_log );

	temp_log.name = "Reindeer";
	temp_log.text = "How foolish ?";

	logs.push_back( temp_log );

	temp_log.name = "Reindeer";
	temp_log.text = "You ain't late... I lied. You are in the wrong continent..";

	logs.push_back( temp_log );

	temp_log.name = "Reindeer";
	temp_log.text = "And I won't carry you anymore you drunk santa !";

	logs.push_back( temp_log );

	temp_log.name = "Santa";
	temp_log.text = "Damn My Reindeer... I won't give bad Reindeer like you a present";

	logs.push_back( temp_log );

	temp_log.name = "Reindeer";
	temp_log.text = "So be it";

	logs.push_back( temp_log );

	temp_log.name = "Narator";
	temp_log.text = "- Bad Ending -";

	logs.push_back( temp_log );

	VisualNovel_t _VN_( App, logs );



/* Data init 2 end */




	chmod( MOD_WALKMAP );



	while ( App.isOpen() ){
		while ( App.pollEvent(Event) ){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
			if( now_mod == MOD_VISUAL_NOVEL ){
				if( Event.type == sf::Event::KeyPressed )
					if( Event.key.code == sf::Keyboard::Return ){
						_VN_.next();
					}
			}
		}

		float dt = Timer.getElapsedTime().asSeconds();
		Timer.restart();
		switch( now_mod )
		{
		case MOD_VISUAL_NOVEL:
			_VN_.update();
			break;
		case MOD_WALKMAP:
			_WM_.update( dt );
			break;
		}

		App.clear( sf::Color( 0,0,0 ) ); // black background

		switch( now_mod ){
		case MOD_VISUAL_NOVEL:
			_VN_.draw();
			break;
		case MOD_WALKMAP:
			_WM_.draw();
			break;
		}

		App.display();
	}

	return 0;
}

