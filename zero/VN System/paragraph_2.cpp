#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

sf::Font PTSANS;

sf::String log_string;
sf::String log_name;

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
	log_name.SetPosition(90.f, 450.f);
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
	//clean of the first spaces
	while( check_index < input.length() && input[check_index] == ' ' ) check_index++;

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

		clean_text( logs[linenum].name );
		clean_text( logs[linenum].text );
		clean_text( logs[linenum].char_image_url );

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
