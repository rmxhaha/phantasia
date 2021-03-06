/*
	Created by remax from Synergy
		2012
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;




string tolower( string input ){
	for( int i = input.length(); i--; ){
		switch( input[ i ] ){
			case 'A': input[i] = 'a'; break;
			case 'B': input[i] = 'b'; break;
			case 'C': input[i] = 'c'; break;
			case 'D': input[i] = 'd'; break;
			case 'E': input[i] = 'e'; break;
			case 'F': input[i] = 'f'; break;
			case 'G': input[i] = 'g'; break;
			case 'H': input[i] = 'h'; break;
			case 'I': input[i] = 'i'; break;
			case 'J': input[i] = 'j'; break;
			case 'K': input[i] = 'k'; break;
			case 'L': input[i] = 'l'; break;
			case 'M': input[i] = 'm'; break;
			case 'N': input[i] = 'n'; break;
			case 'O': input[i] = 'o'; break;
			case 'P': input[i] = 'p'; break;
			case 'Q': input[i] = 'q'; break;
			case 'R': input[i] = 'r'; break;
			case 'S': input[i] = 's'; break;
			case 'T': input[i] = 't'; break;
			case 'U': input[i] = 'u'; break;
			case 'V': input[i] = 'v'; break;
			case 'W': input[i] = 'w'; break;
			case 'X': input[i] = 'x'; break;
			case 'Y': input[i] = 'y'; break;
			case 'Z': input[i] = 'z'; break;
		}
	}

	return input;
}


//function nesting
int parseInt( const string& input ){
	return atoi( input.c_str() );
}




namespace VN {
    class _Log;

	int log_index = 0;
	float char_width[256];
	const float log_width = 1100;

	sf::Texture texture_1; //person_2
	sf::Texture texture_2; //person_1
	sf::Texture texture_3; //log_background

	sf::Sprite person_1;
	sf::Sprite person_2;
	sf::Sprite log_back;

	vector<_Log> logs;

	sf::Font PTSANS;

	bool PTSANS_loaded = false;

	sf::Text log_string;
	sf::Text log_name;

	int log_setup_1(){
		//Load Font
		if( PTSANS_loaded == false ){
			if (!PTSANS.loadFromFile("PTN57F.ttf")) return -1;
			PTSANS_loaded = true;
		}

		texture_3.loadFromFile( "Temp_log.png" );

		//Setup Strings
		log_string.setFont( PTSANS );
		log_string.setCharacterSize(24);
		log_string.setColor( sf::Color(255, 255, 255) );
		log_string.setPosition(90.f, 600.f);

		log_name.setFont( PTSANS );
		log_name.setCharacterSize(40);
		log_name.setColor( sf::Color(255, 255, 255) );
		log_name.setPosition(90.f, 535.f);

		log_back.setTexture( texture_3, false );
		log_back.setPosition( 0.f, 580.f );

	}

	//============================= LOG CLASS =============================
	class _Log {

	public:
		_Log() : image_loaded(0) {}
		~_Log(){ clean(); }
		string name;
		string text;
		string image_path;
		string sound_path;
		int align; //cleaned during loading


		void clean();
		void clear(); //delete all data
		void draw_text( sf::RenderWindow& App );
		void draw_image( sf::RenderWindow& App );

		void load_image();

	private:

		bool image_loaded;
		sf::Sprite Sprite;

		void clean_name();
		void clean_log_text();
		void clean_image_path();
		void clean_sound_path();

		void clean_path( string& input );
	};

	void _Log::clean(){
		clean_name();
		clean_log_text();
		clean_image_path();
		clean_sound_path();
	}

	void _Log::clean_name(){
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

	void _Log::clean_log_text(){
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

	void _Log::clean_path( string& input ){
		if( input.length() == 0 ) return;

		//remove all spaces
		int check_index = input.find_first_not_of(' ');
		int write_index = 0;
		int temp_index;

		if( check_index == -1 ){
			input.clear();
			return;
		}

		while( check_index < input.length() ){
			if( input[check_index] != ' ' ){
				temp_index = check_index;
			}

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


		input.resize( temp_index + 1 );
	}

	void _Log::clean_image_path(){
		clean_path( image_path );
	}

	void _Log::clean_sound_path(){
		clean_path( sound_path );
	}

	void _Log::clear(){
		name.clear();
		text.clear();
		image_path.clear();
		sound_path.clear();

		align = 0;
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
		if( !image_loaded ) load_image();
		App.draw( Sprite );
	}

	void _Log::load_image(){
		if( image_loaded == false && image_path.length() != 0 ) {
			texture_1.loadFromFile( image_path );

			Sprite.setTexture( texture_1, false );
			sf::IntRect spr_data = Sprite.getTextureRect();
			float x,y = WINDOW_HEIGHT - spr_data.height;

			switch( align ){

			case 1: x = 0.f; break;
			case 2: x = WINDOW_WIDTH * 0.25 - spr_data.width /2; break;
			case 3: x = WINDOW_WIDTH * 0.5  - spr_data.width /2; break;
			case 4: x = WINDOW_WIDTH * 0.75  - spr_data.width /2; break;
			case 5: x = WINDOW_WIDTH - spr_data.width; break;

			}

			Sprite.setPosition( x, y );

			cout << x << " " << y << endl;

			image_loaded = true;
		}
	}

	// ============================= END =============================

	// ============================= FILE OPERATION =============================

	void load_text_file( vector<_Log>& logs, string filename ){
		logs.clear();

		ifstream textfile;

		char c;
		string temp_string;

		int linenum = 0;

		textfile.open( filename.c_str() );

		while( textfile.good() )
		{
			logs.push_back( _Log() );
			goto skip2;

			skip:
			logs[linenum].clear();

			skip2:

			// =================================
			// FIRST PARAMETER : image_path
			// =================================

			c = textfile.get();

			while ( textfile.good() && c != '|' ) {
				if( c == '\n' ) goto skip;

				logs[linenum].image_path += c;
				c = textfile.get();
			}

			// =====================================
			// SECOND PARAMETER : image_alignment
			// =====================================

			c = textfile.get();

			temp_string.clear();
			while( textfile.good() && c != '|' ){
				if( c == '\n' ) goto skip;

				temp_string += c;
				c = textfile.get();
			}

			logs[linenum].align = parseInt( temp_string );

			// =====================================
			// THIRD PARAMETER : sound_path
			// =====================================

			c = textfile.get();

			while ( textfile.good() && c != '|' ) {
				if( c == '\n' ) goto skip;

				logs[linenum].sound_path += c;
				c = textfile.get();
			}

			// =====================================
			// FOURTH PARAMETER : name
			// =====================================

			c = textfile.get();

			while ( textfile.good() && c != ':' ) {
				if( c == '\n' )	goto skip;

				logs[linenum].name += c;
				c = textfile.get();
			}

			// =====================================
			// FIFTH PARAMETER : log_text
			// =====================================

			c = textfile.get();

			while ( textfile.good() && c != '\n' ) {
				logs[linenum].text += c;
				c = textfile.get();
			}

			logs[linenum].clean();

			linenum++;
		}

		textfile.close();
	}


	// ============================= END =============================

	// ============================= INIT OPERATION =============================

	void init_char_width(){
		if (!PTSANS.loadFromFile("PTN57F.ttf" )) return;
		PTSANS_loaded = true;

		string tq;
		
		int tmp = log_string.getPosition().x;
		
		for( int i = 255; i--; ){
			tq = (char) i;
			log_string.setString( tq );
			char_width[i] = log_string.findCharacterPos( 1 ).x - tmp;
		}


	}

	void init( string filename ){
		log_setup_1();
		init_char_width();

		load_text_file( logs, filename );


	}

	// ============================= END =============================

	void print_log(){
		//print logs for debugging

		for( int i = 0; i < logs.size() ; ++ i ){
			cout << logs[i].image_path << endl;
			cout << logs[i].sound_path << endl;
			cout << logs[i].name << endl;
			cout << logs[i].text << endl;
			cin.get();
		}
	}

	void Draw( sf::RenderWindow& App ) {
		logs[ log_index ].draw_image( App );
		App.draw( log_back );
		logs[ log_index ].draw_text( App );
	}

	//Call this to move on to the next log
	void Next(){
		if( log_index+1 < logs.size() ) {
			log_index++;
		}
	}

}



int main(){
	sf::RenderWindow App(sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "VN System", 1 << 2 );
	App.setFramerateLimit( 20 ); //max FPS

	VN::init( "Text.txt" );

	sf::Event Event;

	while (App.isOpen()){
		while (App.pollEvent(Event)){
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();
			if( Event.type == sf::Event::KeyPressed )
				if( Event.key.code == sf::Keyboard::Return ) VN::Next();
        }


		App.clear( sf::Color( 0,0,0 ) );

		VN::Draw( App );

		App.display();

	}

	return 0;
}
