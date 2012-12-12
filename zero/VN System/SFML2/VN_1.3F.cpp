/*
	Created by remax from Synergy
		2012
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <streambuf>
#include <string>
#include <cstdlib>

using namespace std;

#include "RString.cpp"
#include "RImage.cpp"

const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;

/* Part of the global variables */
sf::Music soundMusic;

namespace VisualNovel {
	// ============================= CLASS LOG =============================

	class _Log {

	public:
		_Log();
		~_Log();

		string name;
		string text;
		vector<string> image_path;
		vector<int> image_align;
		vector<string> soundFx_path;

		void clean();
	private:
		void clean_name();
		void clean_log_text();
		void clean_path();
	};

	// ============================= END =============================

	struct scene_t {
		vector< _Log > log_queue;
		map< int, string > music_queue;
	};

	map< string, scene_t > scenes; 		// code_name , scene
	scene_t * currentScene; 			// Points to one of the scene

	/*
		Note:

		if ( currentScene == 0 ) then
			VisualNovel should be inactive
	*/

	int LogIndex = 0;
	int SoundIndex = 0;

	sf::Music soundFx;

	vector< image_t* > char_images; 	// Char Images on the screen
	image_t * background_image = 0;		// CG
	image_t * log_background_image = 0;	// log background

	sf::Font PT_SANS; 					// Font
	bool PT_SANS_loaded = false;
	bool char_width_initilized = false;
	
	sf::Text log_string; 				// Text Interface
	sf::Text log_name;   				// Text Name Interface

	float char_width[ 256 ];			//width of each characters on the screen
	const float log_width = 1750;		//fast patch for width of the background log for word wrapping
	const float music_volume = 60.f;
	
	
	void init_char_width()
	{
		if( !PT_SANS_loaded ) {
			if( !PT_SANS.loadFromFile( "PTN57F.ttf" ) ) return;
			
			PT_SANS_loaded = true;
		}

		if( !char_width_initilized ){
			string tq;

			int tmp = log_string.getPosition().x;

			for( int i = 255; i--; ){
				tq = (char) i;
				log_string.setString( tq );
				char_width[i] = log_string.findCharacterPos( 1 ).x - tmp;
			}
			
			char_width_initilized = true;
		}
	}
	
	void resource_init() 
	{

		//load the font
		if( !PT_SANS_loaded ) {
			if ( !PT_SANS.loadFromFile( "PTN57F.ttf") ) return;
			
			PT_SANS_loaded = true;
		}
		
		if( log_background_image == 0 )
			log_background_image = get_image( "Temp_log.png" );

		log_string.setFont( PT_SANS );
		log_string.setCharacterSize(24);
		log_string.setColor( sf::Color(255, 255, 255) );
		log_string.setPosition(90.f, 600.f);

		log_name.setFont( PT_SANS );
		log_name.setCharacterSize(40);
		log_name.setColor( sf::Color(255, 255, 255) );
		log_name.setPosition(90.f, 535.f);

//		log_back.setTexture( log_texture, false );
//		log_back.setPosition( 0.f, 585.f );
	}
	
	bool load_text( const string& fileLoc ) //load the scenes
	{
		if( scenes.size() != 0 ) scenes.clear();
		
		return true;
	}
	
	
	
	void init( const string& fileLoc ){
		if( !load_text( fileLoc ) ) return;
	}
	
	
	// ============================= INTERFACE ==============================

	void load_log_resource() {
		currentScene -> log_queue[ LogIndex ].image_path;
	}

	void print_log( const string& code_name ){
		vector<_Log>& logs = scenes[code_name].log_queue;

		for( int i = 0; i < logs.size() ; ++ i ){
			for( int j = 0; j < logs[i].image_path.size(); j++ )
				cout << "Image>" << logs[i].image_path[j] << endl;
			for( int j = 0; j < logs[i].soundFx_path.size(); j++ )
				cout << "sFx>" << logs[i].soundFx_path[j] << endl;

			cout << logs[i].name << endl;
			cout << logs[i].text << endl;
		}
	}

	// ============================= CLASS LOG DEFINITION ===================

	void clean_path_( string& input ){
		if( input.length() == 0 ) return;

		int write_index = 0;
		int check_index = 0;
		while( check_index < input.length() ){
			if( input[check_index] == '\\' ){
				check_index = input.find_first_not_of( '\\', check_index );
				if( check_index != -1 ){
					check_index--;
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

	_Log::_Log() { }
	_Log::~_Log() { }

	void _Log::clean() {
		clean_name();
		clean_log_text();
		clean_path();
	}

	void _Log::clean_name() {
		int check_index = 0;
		int write_index = 0;

		//search for the first characters which is not a space
		check_index = name.find_first_not_of(' ');

		if( check_index == -1 ){
			//there is no need to clean if all the characters are spaces
			name.clear();
			return;
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

	void _Log::clean_log_text() {
		int check_index = 0;
		int write_index = 0;

		if( name != "narator" ) { //name has already been cleaned

			//search for the first characters which is not a space
			check_index = text.find_first_not_of(' ');

			if( check_index == -1 ) {
				//there is no need to clean if there is not a single space in the sentence
				text.insert( text.begin(), '\"' );
				text.insert( text.end(), '\"' );
				goto cleaned;
			} else if( check_index == 0 ){
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
			check_index = text.find_first_not_of(' ');

			//there is no need to clean if there is not a single space in the sentence
			if( check_index == -1 ) goto cleaned;
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

		//word wrapping
		int i, temp = 0, temp2, temp_i;
		for( i = 0; i < text.length(); ++i ){
			temp_i = i;
			temp2 = 0;

			for( ; text[i] != ' ' && i < text.length() ; ++i ){
				temp2 += char_width[ text[i] ];
			}

			temp2 += char_width[' '];


			if( temp + temp2 >= log_width ){
				text.insert( text.begin() + temp_i, '\n' );
				temp = temp2;
			} else {
				temp = temp + temp2;
			}
		}

		return;
	}

	void _Log::clean_path() {
		int i = 0;
		for( i = 0; i < image_path.size() ; ++i ){
			clean_path_( image_path[i] );
		}

		for( i = 0; i < soundFx_path.size() ; ++i ){
			clean_path_( soundFx_path[i] );
		}
	}

	// ======================================================================

}



int main(){
	sf::RenderWindow App(sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "VN System", 1 << 2 );
	App.setFramerateLimit( 20 ); //max FPS

	sf::Event Event;

	while (App.isOpen()){
		while (App.pollEvent(Event)){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
		}


		App.clear( sf::Color( 0,0,0 ) );

		App.display();

	}

	return 0;
}


