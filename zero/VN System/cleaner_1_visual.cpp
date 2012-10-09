#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

sf::Font PTSANS;

bool PTSANS_loaded = false;

sf::String log_string;
sf::String log_name;


int log_setup_1(){
	//Load Font
	if( PTSANS_loaded == false ) if (!PTSANS.LoadFromFile("PTN57F.ttf", 50)) return -1;

	//Setup Strings
	log_string.SetFont( PTSANS );
	log_string.SetSize(24.f);
	log_string.SetColor( sf::Color(255, 255, 255) );
	log_string.SetPosition(100.f, 500.f);

	log_name.SetFont( PTSANS );
	log_name.SetSize(40.f);
	log_name.SetColor( sf::Color(255, 255, 255) );
	log_name.SetPosition(100.f, 450.f);
}






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

int parseInt( const string& input ){
	return 0;
}


float char_width[256];
const int log_width = 900;

//============================= LOG CLASS =============================
class _Log {

public:
	_Log(){}

	string name;
	string text;
	string image_path;
	string sound_path;
	int left; //cleaned during loading

	void clean();
private:

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

	//remove all spaces
	int check_index = input.find_first_not_of(' ');
	int write_index = 0;
	int temp_index;
	
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
	

	input.resize( temp_index );}

void _Log::clean_image_path(){
	clean_path( image_path );
}

void _Log::clean_sound_path(){
	clean_path( sound_path );
}

// ============================= END =============================

// ============================= FILE OPERATION =============================

void load_text_file( vector<_Log>& logs, string filename ){
	logs.clear();

	ifstream textfile;
	string line;
	textfile.open( filename.c_str() );

	int linenum = 0;
	while( !textfile.eof() )
	{
		logs.push_back( _Log() );

		getline( textfile, line );

		int i = 0;

		for( ; i < line.length() && line[i] != ';'; ++i ) {
			logs[linenum].image_path += line[i];
		}

		i++;

		for( ; i < line.length() && line[i] != ':' ; ++i ){
			logs[linenum].name += line[i];
		}

		i++;


		for( ; i < line.length(); ++i ){
			logs[linenum].text += line[i];
		}

		logs[linenum].clean();

		linenum++;
	}

	textfile.close();
}



// ============================= END =============================

int main(){
	string tq;

	for( int i = 255; i--; ){
		tq = (char) i;
		log_string.SetText( sf::Unicode::Text( tq ) );
		char_width[i] = log_string.GetCharacterPos( 1 ).x;
	}


	sf::RenderWindow App(sf::VideoMode(800, 600), "VN System");
	App.SetFramerateLimit(20); //max FPS

	log_setup_1();

	int log_index = 0;
	sf::Event Event;


	vector<_Log> logs;
	load_text_file( logs, "Text.txt");

	while (App.IsOpened()){
		while (App.GetEvent(Event)){
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
			if( Event.Type == sf::Event::KeyPressed ){
				if( Event.Key.Code == sf::Key::Return )
					if( log_index < logs.size()-1 ) log_index++;

			}
        }


		App.Clear( sf::Color( 0,0,0 ) );

		if( logs[log_index].name != "narator" ){
			log_string.SetText( logs[log_index].text );
			App.Draw(log_string);

			log_name.SetText( logs[log_index].name );
			App.Draw(log_name);
		}else{
			log_string.SetText( logs[log_index].text );
			App.Draw(log_string);

		}

		App.Display();

	}

	return 0;
}
