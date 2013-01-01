#ifndef VISUAL_NOVEL_CPP_
#define VISUAL_NOVEL_CPP_


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cassert>
#include <iostream>
#include <string>

#include "ResourceManager.cpp"
#include "RString.cpp"


/*
log_t
	used to contain Visual Novel's Frame information

	such as :
	-> name
	-> text
	-> image paths and alignments
	-> soundFx path
*/

class log_t {
	public:
		log_t(){ cleaned = false; }
		log_t( const string& n, const string& t)
		: name(n), text(t) {}
		
		~log_t(){}

		string name; // Name of the person talking
		string text; // The person's dialogue

		string music_path;
		string CG_path;
		// Note : leave it blank if there aren't any

		vector<string>	image_path;		// Characters images' URL
		vector<int>		image_align;	// Image alignment ( 5 )
		vector<string>	soundFx_path;	// Sound Effects' URL
		// Note : Will be played in a queue
		
		bool cleaned;
		// Note : please don't change this var from outside unless necesary
		
		// clean messy text coming from the writers
		void clean()
		{
			if( !cleaned ) {
				clean_name();
				clean_log_text();
				clean_path();
				
				cleaned = true;
			}
		}
		// Note : cleaning twice will cause even more messy text so don't
		

	private:

		// cleaning method for URL only
		// Note : This function is made to keep things DRY
		void clean_path_( string& input )
		{
			if( input.length() == 0 ) return;

			unsigned write_index = 0;
			unsigned check_index = 0;
			int search_index = 0;
			
			while( check_index < input.length() ){
				if( input[check_index] == '\\' ){
					search_index = input.find_first_not_of( '\\', check_index );
					if( search_index != -1 ){
						check_index = search_index - 1;
					} else {
						check_index = input.find_last_of( '\\' );
					}
				}

				input[write_index] = input[check_index];
				write_index++;
				check_index++;
			}


			if( write_index != input.length() ) input.resize( write_index + 1 );
		}

		void clean_name()
		{
			unsigned check_index = 0;
			unsigned write_index = 0;
			int search_index = 0;
			
			//search for the first characters which is not a space
			search_index = name.find_first_not_of(' ');

			if( search_index == -1 ){
				//there is no need to clean if all the characters are spaces
				if( name.size() > 0 ) name.clear();
				return;
			} else {
				check_index = ( unsigned ) search_index;
			}

			//Clean double or more spaces together
			while( check_index < name.length() ) {
				if( name[check_index] == ' ' ) {
					while( name[check_index] == ' ' && check_index < name.length() ) check_index++;
					check_index--;
				}

				if( write_index != check_index ) name[ write_index ] = name[ check_index ];

				write_index++;
				check_index++;
			}

			//Clean the last spaces
			if( name[write_index-1] == ' ' ) {
				name.resize( write_index-1 );
			} else {
				name.resize( write_index );
			}
			
			if( tolower( name ) == "narator" ) name = tolower( name );
		}

		void clean_log_text()
		{
			unsigned check_index = 0;
			unsigned write_index = 0;
			int search_index = 0;
			
			//name has already been cleaned
			if( name != "narator" ) { 

				//search for the first characters which is not a space
				search_index = text.find_first_not_of(' ');
				check_index = ( unsigned ) search_index;

				if( search_index == -1 ) {

					//there is no need to clean if there is not a single space in the sentence
					text.insert( text.begin(), '\"' );
					text.insert( text.begin(), '\"' );
					goto cleaned;
					
				} else if( search_index == 0 ){
					text.insert( text.begin(), '\"' );
				} else {
					text[ write_index ] = '\"';
					write_index++;
				}
				
				//Clean double or more spaces together
				while( check_index < text.length() ){
					if( text[check_index] == ' ' ){
						while( text[check_index] == ' ' && check_index < text.length() ) check_index++;
						check_index--;
					}

					if( write_index != check_index ) text[ write_index ] = text[ check_index ];

					write_index++;
					check_index++;
				}

				//add quotation mark add the end of the sentence
				if( write_index == text.length() ){
					text.insert( text.end(), '\"' );
				}else{
					if( text[ write_index - 1 ] == ' ' ) {
						text[ write_index - 1 ] = '\"';
						text.resize( write_index );
					} else {
						text[ write_index ] = '\"';
						text.resize( write_index + 1 );
					}
				}
			} else {
				//search for the first characters which is not a space
				search_index = text.find_first_not_of(' ');

				//there is no need to clean if there is not a single space in the sentence
				if( search_index == -1 ) 
					goto cleaned; 
				
				check_index = ( unsigned ) search_index;
				
				//Clean doublxe or more spaces together
				while( check_index < text.length() ) {
					if( text[check_index] == ' ' ) {
						while( text[check_index] == ' ' && check_index < text.length() ) check_index++;
						check_index--;
					}

					if( write_index != check_index ) text[ write_index ] = text[ check_index ];

					write_index++;
					check_index++;
				}

				//Clean the last spaces
				if( text[write_index-1] == ' ' ) {
					text.resize( write_index-1 );
				} else {
					text.resize( write_index );
				}
			}


			cleaned:

			return;
		}

		void clean_path()
		{
			unsigned i = 0;
			for( i = 0; i < image_path.size() ; ++i ){
				clean_path_( image_path[i] );
			}

			for( i = 0; i < soundFx_path.size() ; ++i ){
				clean_path_( soundFx_path[i] );
			}

			clean_path_( music_path );
			clean_path_( CG_path );
		}
	};

class VisualNovel_t {
private:
	/*
	log_char_image_t
		contains image's alignment and images' resource

	Note : 	Every time the player moves to a different frame 
			information from log_t::image_path will be processed here
			so it useable by "draw" function
	*/

	struct log_char_image_t {
		int x;
		image_t * image;

		void draw( sf::RenderWindow& App ){
			assert( image -> loaded() );

			image -> draw( App, x, WINDOW_HEIGHT - image -> height() );
		}

		void init( image_t * img, int align ){
			image = img;

			align %= 5;

			// divide frame by 5
			x = align * WINDOW_WIDTH / 5.f;
		}

		log_char_image_t( image_t * img, int align ){
			init( img, align );
		}
		
		log_char_image_t(){
			image = 0;
			x = 0;
		}
	};
public:
	VisualNovel_t( sf::RenderWindow& App_, vector<log_t>& logs_ ){		
		assert( logs_.size() > 0 );

		this -> logs = logs_;
		this -> App = &App_;
		
	/* Log Init */
		log_cg = NULL;
		now_log = logs.begin();

	/* Image Init */
		log_background = imagePool.get_image( "resource/LogBackground.png" );

	/* Audio Init */
		SoundMusic.setVolume( MUSIC_VOLUME );
		SoundMusic.setLoop( true );
		SoundQueue.volume( MUSIC_VOLUME );

	/* Font Init */
		font_display.loadFromFile( "resource/PTN57F.ttf" );

		log_text_display.setFont( font_display );
		log_text_display.setCharacterSize( 24 );
		log_text_display.setColor( sf::Color( 255, 255, 255 ) );
		log_text_display.setPosition( log_x + log_padding_x, log_y + log_padding_y );

		log_name_display.setFont( font_display );
		log_name_display.setCharacterSize( 40 );
		log_name_display.setColor( sf::Color( 255, 255, 255 ) );
		log_name_display.setPosition( log_x + log_padding_x, log_y - log_to_name_y - 40 );

	/* Last Init */
		ManageEverything();
	}
	
	~VisualNovel_t(){
		// nothing to do here
	}
	
	void draw(){
		if( log_cg != NULL ) log_cg -> draw( *App, 0, 0 );
		
		for( unsigned i = log_char_image.size(); i--; ){
			log_char_image[i].draw( *App );
		}
		
		log_background -> draw( *App, log_x, log_y );
		App->draw( log_text_display );
		App->draw( log_name_display );
	}
	
	void update(){
	
		// update sound_queue_t
		SoundQueue.update();
	}
	
	bool next(){
		if( now_log+1 == logs.end() ) return false;

		++now_log;
		ManageEverything();
		return true;
	}
	
private:
	void ManageEverything(){
		assert( now_log != logs.end() );
		
		// Clean the string
		now_log->clean();

		
	/* Manage Image */
		imagePool.resetNeeds();
		
		log_background -> needed( true );
		
		// CG
		if( now_log -> CG_path.size() != 0 )
			log_cg = imagePool.get_image( now_log -> CG_path );
		
		// Characters Image
		log_char_image.resize( now_log -> image_path.size() );
				
		for( unsigned i = 0; i < now_log -> image_path.size(); ++i ){
			log_char_image[i].init( 
				imagePool.get_image( now_log -> image_path[i] ), 
				now_log -> image_align[i] 
			);
		}

		imagePool.manage(); // load everything needed

	/* Manage Text */
		
		// Word wrapping
		
		string& log_text = now_log->text;
		//divide the sentence into words
		vector< string > words = explode( ' ', log_text );
		vector< bool > newline( words.size() );

		int limit_right = log_x + log_background -> width() - log_padding_x * 2;

		for( unsigned index = 0; index < words.size(); ++index ){

			log_text.clear();

			for( unsigned i = 0; i <= index; i++ ){
				log_text += words[i] + ( newline[ i ] ? '\n' : ' ' );
			}

			log_text_display.setString( log_text );

			if( log_text_display.findCharacterPos( log_text.size() ).x >= limit_right ){
				newline[ index - 1 ] = true;
			}
		}

		log_text.clear();

		for( unsigned i = 0; i < words.size(); i++ ){
			log_text += words[i] + ( newline[ i ] ? '\n' : ' ' );
		}
		
		// settings up the sf::text
		log_text_display.setString( now_log->text );
		log_name_display.setString( now_log->name );
		
	/* Manage Audio */
		SoundQueue.force_end();
		for( unsigned i = 0; i < now_log-> soundFx_path.size(); ++i )
		{
			SoundQueue.append( now_log->soundFx_path[i] );
		}
		
		if( now_log -> music_path.size() != 0 ){
			if( SoundMusic.getStatus() == sf::SoundSource::Status::Stopped ){
				if( now_log == logs.begin() || (now_log-1)-> music_path != now_log -> music_path ){
					SoundMusic.openFromFile( now_log -> music_path );
					SoundMusic.play();
				}
			}
		}
		
	}
	
	
	// =================== VARIABLES DECLERATION ===================

	/* Log Coordination settings */
	static const float log_x = 55.f;
	static const float log_y = 570.f;
	static const float log_padding_x = 40.f;
	static const float log_padding_y = 10.f;
	static const float log_to_name_y = 12.f;
	
	/*	Text Display */
	sf::Text log_text_display;
	sf::Text log_name_display;
	sf::Font font_display;

	/* Temporary Log Informations */
	image_t * log_background;
	image_t * log_cg;
	vector<log_char_image_t> log_char_image;
	
	/* Sound and Music */
	sf::Music SoundMusic;
	sound_queue_t SoundQueue;

	/* Image Management */
	ImagePool_t imagePool;
		
	/* Log Management */
	vector<log_t>::iterator now_log;
	vector< log_t > logs;
	
	/* Window Target */
	sf::RenderWindow * App;
	
	// ======================= END =============================

};
#endif


/*
int main() {
	sf::RenderWindow App(sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "Eterna Memoria 1.0.1", 1 << 2 );
	sf::Event Event;
	
	App.setFramerateLimit( 12 );

	log_t temp_log;

	temp_log.name = "ho";
	temp_log.text = "a b c d e f g h i j k l m n o p q r s t u v w x y z a b c d e f g h i j k l m n o p q r s t u v w x y z a b c d e f g h i j k l m n o p q r s t u v w x y z a b c d e f g h i j k l m n o p q r s t u v w x y z a b c d e f g h i j k l m n o p q r s t u v w x y z a b c d e f g h i j k l m n o p q r s t u v w x y z ";
	temp_log.music_path = "resource/Dungeon1.ogg";
	temp_log.CG_path = "resource/Apple_Wallpaper_Template_by_my_nightmare_reborn.jpg";
	temp_log.image_path.push_back( "resource/A.png" );
	temp_log.image_align.push_back( 1 );
	
	temp_log.soundFx_path.push_back( "resource/Quake.ogg");
	temp_log.soundFx_path.push_back( "resource/Wind.ogg");
	temp_log.soundFx_path.push_back( "resource/Quake.ogg");
	temp_log.soundFx_path.push_back( "resource/Wind.ogg");
	
	vector< log_t > logs = {
		log_t( "hiks", "hhahahaha" ),
		temp_log
	};

	logs[0].music_path = "resource/Dungeon1.ogg";
	
	VisualNovel_t _VN_( App, logs );
	
	while ( App.isOpen() ){
		while (App.pollEvent(Event)){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
			if( Event.type == sf::Event::KeyPressed )
				if( Event.key.code == sf::Keyboard::Return ){
					_VN_.next();
				}
		}

		_VN_.update();
		
		App.clear( sf::Color( 0,0,0 ) );
		
		_VN_.draw();
		
		App.display();
	}
	
	return 0;
}
*/