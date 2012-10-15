#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

sf::Font PTSANS;

sf::String log_string;
sf::String log_name;

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

int log_setup(){
	//Load Font
	if (!PTSANS.LoadFromFile("PTN57F.ttf", 50)) return -1;

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


//make sure the text is cleaned before entering here
void break_up( string& text, int word_from_behind ){
	int count = 0, i;
	for( i = text.length(); i--; ){
		if( text[i] == ' ' ) count ++;
		if( count == word_from_behind ) break;
	}
	text.insert( text.begin()+i, '\n' );
}

void wrap_text( sf::String& input, string text, int width ){ //on development
	string temporary = text;
//	while( width <= input.GetSize().x ){
//		input.SetText( temporary );
//	}

	input.SetText( text );

}



struct _Log{
	_Log(){};
	string name;
	string text;
	string char_image_url;
};

void clean_text( string& input ){
	int check_index = 0;
	int write_index = 0;

	//search for the first characters which is not a space
	check_index = input.find_first_not_of(' ');

	//there is no need to clean if there is not a single space in the sentence
	if( check_index == -1 ) return;

	//Clean double or more spaces together
	while( check_index < input.length() ){
		if( input[check_index] == ' ' ){
			while( input[check_index] == ' ' && check_index < input.length() ) check_index++;
			check_index--;
		}

		input[ write_index ] = input[ check_index ];

		write_index++;
		check_index++;
	}

	//Clean the last spaces
	if( input[write_index-1] == ' ' ){
		input.resize( write_index-1 );
	}else{
		input.resize( write_index );
	}
}

void clean_text( _Log& log ){
	clean_text( log.name );
	clean_text( log.text );
}

void add_quote( string& input ){

	//add quotation mark add the start of the sentence
	input.insert( input.begin(), '\"' );

	//add quotation mark add the end of the sentence
	input.insert( input.end(), '\"' );

}

void clean_text_quote( _Log& log ){
	//clean the name from extra spaces
	clean_text( log.name );

	if( tolower(log.name) != "narator" ){
		//clean the last space first before adding quotation mark
		int index = log.text.find_last_not_of(' ');

		if( index >= 0 ) log.text.erase( log.text.begin()+index );


		//the person is not a narator add a quotation mark
		add_quote( log.text );
	} else {
		log.name = tolower( log.name );
	}

	//clean the text from extra spaces
	clean_text( log.text );
}

void remove_space( string& input ){
	int check_index = 0;
	int write_index = 0;

	while( check_index < input.length() ){
		if( input[check_index] != ' ' ){
			input[write_index] = input[check_index];
			write_index++;
		}

		check_index++;
	}

	input.resize( write_index );
}


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
		for( ; i < line.length() && line[i] != ':'; ++i ){
			logs[linenum].char_image_url += line[i];
		}

		i++;

		for( ; i < line.length() && line[i] != ':' ; ++i ){
			logs[linenum].name += line[i];
		}

		i++;


		for( ; i < line.length(); ++i ){
			logs[linenum].text += line[i];
		}

		clean_text_quote( logs[linenum] );
		remove_space( logs[linenum].char_image_url );

		linenum++;
	}

	textfile.close();
}



int main( int argc, char * argv[] ){
	sf::RenderWindow App(sf::VideoMode(800, 600), "VN System");
	App.SetFramerateLimit(20); //max FPS

	log_setup();

	sf::Event Event;
	int log_index = 0;
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

		log_string.SetText( logs[log_index].text );
		App.Draw(log_string);

		log_name.SetText( logs[log_index].name );
		App.Draw(log_name);

		App.Display();

	}

}
