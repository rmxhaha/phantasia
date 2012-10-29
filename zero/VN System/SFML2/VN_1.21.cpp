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

const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;

char tolower( char input ) {
	switch( input ){
		case 'A': input = 'a'; break;
		case 'B': input = 'b'; break;
		case 'C': input = 'c'; break;
		case 'D': input = 'd'; break;
		case 'E': input = 'e'; break;
		case 'F': input = 'f'; break;
		case 'G': input = 'g'; break;
		case 'H': input = 'h'; break;
		case 'I': input = 'i'; break;
		case 'J': input = 'j'; break;
		case 'K': input = 'k'; break;
		case 'L': input = 'l'; break;
		case 'M': input = 'm'; break;
		case 'N': input = 'n'; break;
		case 'O': input = 'o'; break;
		case 'P': input = 'p'; break;
		case 'Q': input = 'q'; break;
		case 'R': input = 'r'; break;
		case 'S': input = 's'; break;
		case 'T': input = 't'; break;
		case 'U': input = 'u'; break;
		case 'V': input = 'v'; break;
		case 'W': input = 'w'; break;
		case 'X': input = 'x'; break;
		case 'Y': input = 'y'; break;
		case 'Z': input = 'z'; break;
	}

	return input;
}

string tolower( string input ){
	for( int i = input.length(); i--; ){
		input[i] = tolower( input[i] );
	}

	return input;
}

int parseInt( const string& input ){
	return atoi( input.c_str() );
}

vector < string > explode(char sep, string s) {
	vector < string > f;
	string temp = "";

	for (int i=0; i<s.size(); i++)
	{
			if ( s[i] == sep )
			{
					if ( temp != " " || temp != "" ) f.push_back(temp);
					temp = "";
			} else temp += s[i];
	}
	if ( temp != " " || temp != "" ) f.push_back(temp);

	return f;
}



namespace VN {
	class _Log;
	int log_index = 0;
	int soundFx_index = 0;
	float char_width[ 256 ];
	const float log_width = 1750;
	const float music_volume = 60.f;
	float music_volume_ = music_volume;

	sf::Music soundFx, Music;
	sf::Texture log_texture;
	sf::Sprite log_back;

	vector< sf::Texture > texture_;
	vector< sf::Sprite > sprite_;
	map< int, string > Music_queue;

	vector<_Log> logs;

	sf::Font PT_SANS;
	bool PT_SANS_loaded = false;

	sf::Text log_string;
	sf::Text log_name;

	void load_log_resource( _Log& data );

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

		void exec();
		void clear();
		void draw_text( sf::RenderWindow& App );
		void draw_image( sf::RenderWindow& App );
		void clean();
	private:
		void clean_name();
		void clean_log_text();
		void clean_path();
	};

	_Log::_Log() {  }
	_Log::~_Log() {  }

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

	void _Log::clear() {
		name.clear();
		text.clear();

		image_path.clear();
		image_align.clear();
		soundFx_path.clear();
	}

	void _Log::draw_text( sf::RenderWindow& App ) {
		if( name != "narator" ){
			log_string.setString( text );
			App.draw( log_string );

			log_name.setString( name );
			App.draw( log_name );
		}else{
			log_string.setString( text );
			App.draw( log_string);

		}
	}

	void _Log::draw_image( sf::RenderWindow& App ) {
		for( int i = 0; i < sprite_.size(); i++ ){
			App.draw( sprite_[i] );
		}
	}

	void _Log::exec() {
		this -> clean();
		load_log_resource( *this );
	}


	// ============================= END =============================


	// ============================= FILE OPERATION =============================

	void load_log_resource( _Log& data ) {
		if( data.image_path.size() != data.image_align.size() ) {
			cout << "ERR : Image data mismatch" << endl;
			return;
		}

		int i;
		float x,y;
		sf::IntRect sprite_data;

		sprite_.clear();
		texture_.clear();

		texture_.resize( data.image_path.size() );
		sprite_.resize( data.image_path.size() );

		for( i = 0; i < data.image_path.size(); ++i ) {
			if( data.image_path[i].length() != 0 ){
				texture_[i].loadFromFile ( data.image_path[i] );
				sprite_[i].setTexture( texture_[i], false );

				sprite_data = sprite_[i].getTextureRect();
				y = WINDOW_HEIGHT - sprite_data.height;

				switch( data.image_align[i] ){

				case 1: x = 0.f; break;
				case 2: x = WINDOW_WIDTH * 0.25 - sprite_data.width /2; break;
				case 3: x = WINDOW_WIDTH * 0.5  - sprite_data.width /2; break;
				case 4: x = WINDOW_WIDTH * 0.75  - sprite_data.width /2; break;
				case 5: x = WINDOW_WIDTH - sprite_data.width; break;

				}
				sprite_[i].setPosition( x, y );
			}
		}

		if( Music_queue[ log_index ].length() != 0 ) {
			Music.openFromFile( Music_queue[ log_index ] );
			Music.play();
		}
	}

	void load_text_file( string fn ) {
		ifstream is;
		vector<string> temp;
		string data;

		is.open ( fn.c_str() , ios::binary );

		int index;
		logs.push_back( _Log() );

		while( getline( is, data ) ){
			if( data.size() > 0 && data[ data.size() -1 ] == '\r' ) data.resize( data.size()-1 );

			index = 0;

			if( tolower( data.substr( index, 5 ) ) == "music" ){
				index += 6;

				Music_queue[ logs.size() - 1 ] = data.substr( index );

				continue;
			}

			else if( tolower( data.substr( index, 7 ) ) == "display" ){
				index += 8;

				temp = explode( '|', data.substr( index ) );

				if( temp.size() % 2 == 1 ){
					cout << "ERR : Broken Image Data" << endl;
				} else {

					for( int i = 0; i < temp.size(); ) {
						logs.back().image_path.push_back( temp[ i++ ] );
						logs.back().image_align.push_back( parseInt( temp[ i++ ] ) );
					}
				}

				continue;
			}

			else if( tolower( data.substr( index, 4 ) ) == "play" ){
				index += 5;


				logs.back().soundFx_path = explode( '|', data.substr( index ) );
				continue;
			}

			else {
				if( data.size() == 0 ) continue;
				temp = explode( ':', data );

				if( temp.size() != 2 ){
					cout << "ERR : log data is broken" << endl;
				} else {
					logs.back().name = temp[0];
					logs.back().text = temp[1];
				}
				logs.push_back( _Log() );
			}
		}

		logs.pop_back();
	}

	void print_log(){
		//print logs for debugging

		for( int i = 0; i < logs.size() ; ++ i ){
			for( int j = 0; j < logs[i].image_path.size(); j++ )
				cout << "Image>" << logs[i].image_path[j] << endl;
			for( int j = 0; j < logs[i].soundFx_path.size(); j++ )
				cout << "sFx>" << logs[i].soundFx_path[j] << endl;

			cout << logs[i].name << endl;
			cout << logs[i].text << endl;
		}
	}


	// ============================= INIT OPERATION =============================

	void reset() {
		logs.clear();
		log_index = 0;
		soundFx_index = 0;

		texture_.clear();
		sprite_.clear();
		Music_queue.clear(); //clear it
	}

	void init_char_width() {
		if (!PT_SANS.loadFromFile("PTN57F.ttf" )) return;
		PT_SANS_loaded = true;

		string tq;

		int tmp = log_string.getPosition().x;

		for( int i = 255; i--; ){
			tq = (char) i;
			log_string.setString( tq );
			char_width[i] = log_string.findCharacterPos( 1 ).x - tmp;
		}


	}

	void resource_init () {
		//load the font
		if( PT_SANS_loaded == false ) {
			if (!PT_SANS.loadFromFile("PTN57F.ttf")) return;
			PT_SANS_loaded = true;
		}

		log_texture.loadFromFile( "Temp_log.png" );

		log_string.setFont( PT_SANS );
		log_string.setCharacterSize(24);
		log_string.setColor( sf::Color(255, 255, 255) );
		log_string.setPosition(90.f, 600.f);

		log_name.setFont( PT_SANS );
		log_name.setCharacterSize(40);
		log_name.setColor( sf::Color(255, 255, 255) );
		log_name.setPosition(90.f, 535.f);

		log_back.setTexture( log_texture, false );
		log_back.setPosition( 0.f, 585.f );
	}

	void init( string input ) {
		reset();
		init_char_width();
		resource_init();
		load_text_file( input );


		logs[log_index].exec();
	}

	// ============================= END =============================

	void Draw( sf::RenderWindow& App ) {
		logs[ log_index ].draw_image( App );
		App.draw( log_back );
		logs[ log_index ].draw_text( App );
	}

	void handle_sound() {
		
		if( soundFx.getStatus() == 0 ){ //status stop
			if( soundFx_index < logs[ log_index ].soundFx_path.size() ){
				cout << "R" << endl;
				soundFx.openFromFile( logs[ log_index ].soundFx_path[ soundFx_index ] );
				soundFx.play();
				soundFx_index++;
			}
		}
	}

	void Next() {
		if( log_index+1 < logs.size() ) {
			soundFx_index = 0;
			soundFx.stop();
			
			log_index++;
			logs[log_index].exec();
		}
	}

}

int main(){
	sf::RenderWindow App(sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "VN System", 1 << 2 );
	App.setFramerateLimit( 20 ); //max FPS

	VN::init( "Text.txt" );
	VN::print_log();

	sf::Event Event;

	while (App.isOpen()){
		while (App.pollEvent(Event)){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
			if( Event.type == sf::Event::KeyPressed )
				if( Event.key.code == sf::Keyboard::Return ) VN::Next();
		}


		App.clear( sf::Color( 0,0,0 ) );

		VN::Draw( App );
		VN::handle_sound();
		App.display();

	}

	return 0;
}


