#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <streambuf>

#include "RImage.cpp"
#include "RString.cpp"

using namespace std;

const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;

sf::Music SoundMusic;

namespace VisualNovel {
	// =================== CLASS DECLERATION ===================
	class log_t {
	public:
		log_t(){}
		~log_t(){}

		string name;
		string text;

		string music_path; //leave it blank if there aren't any

		vector<string> image_path;
		vector<int> image_align;
		vector<string> soundFx_path;

		void clean()
		{
			clean_name();
			clean_log_text();
			clean_path();
		}

	private:

		void clean_path_( string& input )
		{
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

		void clean_name()
		{
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

		void clean_log_text()
		{
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

			return;
		}

		void clean_path()
		{
			int i = 0;
			for( i = 0; i < image_path.size() ; ++i ){
				clean_path_( image_path[i] );
			}

			for( i = 0; i < soundFx_path.size() ; ++i ){
				clean_path_( soundFx_path[i] );
			}

			clean_path_( music_path );
		}
	};

	struct scene_t {
		vector< log_t > log_array;

		int limit() const {
			return log_array.size();
		}
	};

	struct log_char_image_t{
		int x;
		image_t * image;

		void draw( sf::RenderWindow& App ){
			assert( image -> loaded() );

			image -> draw( App, x, WINDOW_HEIGHT - image -> height() );
		}

		void init( const string& fileLoc, int align ){
			image = get_image( fileLoc );

			align %= 5;
			x = (float) align * WINDOW_WIDTH / 5.f;
		}

		log_char_image_t( const string& fileLoc, int align ){
			init( fileLoc, align );
		}

		log_char_image_t(){
		}

	};


	// ======================= END =============================

	// =================== VARIABLES DECLERATION ===================

	/* Log Management */
	scene_t * 				now_scene;
	log_t *				 	now_log = 0;
	int 					log_index = 0;
	map< string, scene_t >	scenes;

	/*	Text Display */
	sf::Text log_text_display;
	sf::Text log_name_display;
	sf::Font font_display;

	/* Sound Fx */
	sf::Music SoundFx;

	/* Log Coordination settings */
	const float log_x = 55;
	const float log_y = 570;
	const float log_padding_x = 40;
	const float log_padding_y = 10;
	const float log_to_name_y = 12;

	image_t * log_background = 0;

	/* Temporary Log Informations */
	image_t * 					log_CG = 0;
	vector<log_char_image_t> 	log_char_image;
	queue<string *> 			sound_queue;

	string log_text;
	string log_name;

	// ======================= END =============================

	// =================== PRIVATE FUNCTIONS DECLERATION ===================

	/*
		PRIVATE function are not supposed to be called from outside VisualNovel namespace
		even though it's accessible from global it can lead to errors
	*/
	void Init(){
		log_background = get_image( "resource/LogBackground.png" );

		static bool loaded = false;
		if( !loaded ){
			font_display.loadFromFile( "resource/PTN57F.ttf" );

			log_text_display.setFont( font_display );
			log_text_display.setCharacterSize( 24 );
			log_text_display.setColor( sf::Color( 255, 255, 255 ) );
			log_text_display.setPosition( log_x + log_padding_x, log_y + log_padding_y );

			log_name_display.setFont( font_display );
			log_name_display.setCharacterSize( 40 );
			log_name_display.setColor( sf::Color( 255, 255, 255 ) );
			log_name_display.setPosition( log_x + log_padding_x, log_y - log_to_name_y - 40 );
		}
	}

	void wordwrap(){
		vector< string > words = explode( ' ', log_text );
		vector< bool > newline( words.size() );

		int width = log_x + log_background -> width() - log_padding_x * 2;

		for( int index = 0; index < words.size(); ++index ){

			log_text.clear();

			for( int i = 0; i <= index; i++ ){
				log_text += words[i] + ( newline[ i ] ? '\n' : ' ' );
			}

			log_text_display.setString( log_text );

			if( log_text_display.findCharacterPos( log_text.size() ).x >= width ){
				newline[ index - 1 ] = true;
			}
		}

		log_text.clear();

		for( int i = 0; i < words.size(); i++ ){
			log_text += words[i] + ( newline[ i ] ? '\n' : ' ' );
		}
	}

	/*
		All "Manage" Function is called everytime NextLog is called
		All Manage Function are the function that are used for managing the log resources ( images, sounds, text )
	*/

	void ManageText(){
		wordwrap(); //setString is included inside this function
		log_name_display.setString( log_name );
	}

	void ManageImage(){
		assert( now_log != 0 );

		log_char_image.resize( now_log -> image_path.size() );

		for( int i = 0; i < now_log -> image_path.size(); ++i ){
			log_char_image[i].init( now_log -> image_path[i], now_log -> image_align[i] );
		}

	}

	void ManageSound(){
		assert( now_log != 0 );

		while( !sound_queue.empty() ) sound_queue.pop();

		for( int i = 0; i < now_log -> soundFx_path.size(); ++i ){
			sound_queue.push( &(now_log->soundFx_path[i]) );
		}
		if( now_log -> music_path.size() != 0 ){
			SoundMusic.openFromFile( now_log -> music_path );
		}
	}

	void ManageLog(){
		assert( now_log != 0 );

		//clean everything text first
		now_log -> clean();

		log_name = now_log -> name;
		log_text = now_log -> text;

		ManageText();

		ManageImage();
		ManageSound();
	}

	/*
		ext() are used for determining whenever an file is sound file, image file, text file, or other files ( unknown )
	*/
	
	enum extension_t {
		SOUND_EXTENSION,
		IMAGE_EXTENSION,
		TEXT_EXTENSION,
		UNKNOWN_EXTENSION
	};
	
	extension_t ext( const string& input ){
		string ext3 = input.substr( input.size() - 3 );
		string ext4 = input.substr( input.size() - 4 );

		if( ext3 == "ogg" || ext3 == "wav" || ext4 == "flac" ){
			return SOUND_EXTENSION;
		}

		else if( ext3 == "png" || ext3 == "jpg" || ext3 == "tga" || ext3 == "bmp" || ext3 == "psd" ){
			return IMAGE_EXTENSION;
		}

		else if( ext3 == "txt" || ext3 == "vns" ){
			return TEXT_EXTENSION;
		}

		else return UNKNOWN_EXTENSION;
	}

	// ======================= END =============================

	// =================== PUBLIC FUNCTIONS DECLERATION ===================
	
	void HandleSound(){
		/* Handle sound_queue and music */

		if( SoundFx.getStatus() == 0 ){ //status : STOPPED
			if( !sound_queue.empty() ){
				SoundFx.openFromFile( *sound_queue.front() );
				SoundFx.play();

				sound_queue.pop();
			}
		}

		if( ::SoundMusic.getStatus() == 0 && now_log -> music_path.size() != 0 ){
			::SoundMusic.play();
		}
	}

	void DeclareImageNeeds(){
		assert( log_background != 0 );

		log_background -> needed( true );

		if( log_CG != 0 )
			log_CG -> needed( true );

		for( int i = 0; i < log_char_image.size(); ++i )
			log_char_image[i].image -> needed( true );
	}

	void draw( sf::RenderWindow& App ){
		if( now_scene != 0 ){
			if( log_CG != 0 )
				log_CG -> draw( App,0,0 );

			for( int i = 0; i < log_char_image.size(); ++i ){
				log_char_image[i].draw( App );
			}

			log_background -> draw( App, log_x, log_y );
			App.draw( log_text_display );
			App.draw( log_name_display );
		}
	}

	void ActiveScene( const string& scene_name ){
		now_scene = &scenes[ scene_name ];
		assert( now_scene -> log_array.size() != 0 );
		log_index = 0;

		now_log = &now_scene -> log_array[ log_index ];

		ManageLog();

	}

	void Deactivate(){
		now_scene = 0;
		now_log = 0;
	}
	
	bool isActive(){
		return now_scene != 0;
	}

	bool NextLog(){
		assert( isActive() );

		if( ++log_index < now_scene -> log_array.size() ){
			now_log = &now_scene -> log_array[ log_index ];

			ManageLog();
			return true;
		}

		return false;
	}

	void ParseText( const string& fileLoc ){
		ifstream file;
		file.open( fileLoc.c_str(), ios::binary );

		vector< string > array;
		scene_t * scn = 0;
		string line;
		string music_path;

		while( getline( file, line ) ){
			if( line.size() > 0 && line[ line.size() - 1 ] == '\r' ) line.resize( line.size()-1 );

			if( tolower( line.substr( 0, 2 ) == "//" ) ){
				assert( scn != 0 );

				music_path = line.substr( 2 );
			} else {
				array = explode( ':', line );

				if( array.size() == 2 && array[1] == ""){
					scn = &scenes[ array[0] ];

					//it's dangerous if it's not cleared
					music_path.clear();
				}
				else if( array.size() >= 2 ){
					assert( scn != 0 );

					scn -> log_array.resize( scn -> log_array.size() + 1 );

					if( music_path.size() != 0 ){
						//copy the parsed music_path
						scn -> log_array.back().music_path = music_path;

						//remove it after copy
						music_path.clear();
					}

					int i = 0;
					scn -> log_array.back().name = array[i++];
					scn -> log_array.back().text = array[i++];
					while( i < array.size() ){
						if( ext( array[i] ) == IMAGE_EXTENSION ){
							scn -> log_array.back().image_path.push_back( array[i++] );
							assert( i != array.size() );
							scn -> log_array.back().image_align.push_back( parseInt( array[i++] ) );
							continue;
						}

						else if( ext( array[i]) == SOUND_EXTENSION ){
							scn -> log_array.back().soundFx_path.push_back( array[i++] );
							continue;
						}

						i++;
					}
				}
			}

        }
		file.close();
	}

	void PrintLog(){
		for( auto 	itr = VisualNovel::scenes.begin();
					itr != VisualNovel::scenes.end();
					itr++ )
		{
			//Scene Name ( usually the name of the person we are talking to )
			cout << "~ " << itr -> first << " ~\n";

			for( auto 	itr2 = itr -> second.log_array.begin();
						itr2 != itr -> second.log_array.end();
						itr2++ )
			{
				cout << itr2 -> name << " : ";	// Person Name
				cout << itr2 -> text << endl;	// His/Her Dialog

				if( itr2 -> music_path.length() != 0 ){
					cout << "m> " << itr2 -> music_path << endl;
				}

				for( int i = 0;
					i != itr2->image_path.size(); ++i )
					{
						cout << "i> " << itr2->image_path[i] << " -> " << itr2->image_align[i] << endl;
					}
				for( int i = 0;
					i != itr2 -> soundFx_path.size(); ++i )
					{
						cout << "s> " << itr2->soundFx_path[i] << endl;
					}
			}
		}
	}

	bool WITH_CG(){
		return ( log_CG != 0 );
	}

	// ======================= END =============================

}

/*
	Do not do the following
		using namespace VisualNovel
		( can cause naming conflict )

*/


int main( int argc, char ** argv ){
	cout << sizeof( VisualNovel::log_t ) << endl;

	VisualNovel::Init();
	VisualNovel::ParseText( "textone.vns" );
	VisualNovel::PrintLog();
	VisualNovel::ActiveScene( "charl" );
	VisualNovel::DeclareImageNeeds();
	ManageImage();

	sf::RenderWindow App(sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "Visual Novel 1.3", 1 << 2 ); //480p for testing
	App.setFramerateLimit( 12 ); //Maximum FPS

	sf::Event Event;
	
	while ( App.isOpen() ){
		while (App.pollEvent(Event)){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
			if( Event.type == sf::Event::KeyPressed )
				if( Event.key.code == sf::Keyboard::Return ){
					if( VisualNovel::NextLog() ){
					VisualNovel::DeclareImageNeeds();
					ManageImage();
					} else VisualNovel::Deactivate();
				}
		}


		App.clear( sf::Color( 0,0,0 ) );
		VisualNovel::draw( App );
		VisualNovel::HandleSound();

//		App.draw( VisualNovel::interface_text );

		App.display();

	}

	return 0;

}
