#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

using namespace std;

class _Image;

enum char_name_t{
	STEFAN,
	DARRYL,
	CANDRA,
	ANTHONY,
	WILLSON,
	JONATHAN,
	GLADOS
};

string char_name( char_name_t char_id ){
	switch( char_id ){
		case STEFAN: return "Stefan";
		case DARRYL: return "Darryl";
		case CANDRA: return "Candra";
		case ANTHONY: return "Anthony";
		case WILLSON: return "Willson";
		case JONATHAN: return "Jonathan";
		case GLADOS: return "GLaDOS";
	}
}


struct _Log {
	_Log( char_name_t char_id, string text ){
		this -> char_id = char_id;
		this -> text = text;
	}
	string name(){
		return char_name( char_id );
	}

	char_name_t char_id;
	string text;
	_Image * char_image;
};

int main(){
	sf::RenderWindow App(sf::VideoMode(800, 600), "VN System");
	App.SetFramerateLimit(20); //max FPS

    // Load a font from a file
    sf::Font MyFont;
    if (!MyFont.LoadFromFile("PTN57F.ttf", 50)) return -1;

	sf::String log_string;
	log_string.SetFont( MyFont );
	log_string.SetColor( sf::Color(255, 255, 255) );
	log_string.SetPosition(100.f, 500.f);
//	log_string.SetRotation(15.f);
	log_string.SetSize(24.f);

	sf::String log_name;
	log_name.SetFont( MyFont );
	log_name.SetColor( sf::Color(255, 255, 255) );
	log_name.SetPosition(90.f, 450.f);
//	log_name.SetRotation(15.f);
	log_name.SetSize(40.f);


	_Log logs[] = {
		_Log( JONATHAN, "HAHAHAHAHAHAHAHAHAHAHHAHAHAHHHHHHHHHHHHHHHHHHHHHHHHHHHHHAAAAAAAAAAAAAAAAAAAAAAAAa"),
		_Log( WILLSON, "Hello !\nHow are you ?"),
		_Log( CANDRA, "Ehmmm.....\nTesting Testing Testing" ),
		_Log( STEFAN, "What are you testing ?" ),
		_Log( ANTHONY, "==\"" ),
		_Log( GLADOS, "It's been a loong time\nHow have you been ?" ),
		_Log( GLADOS, "[Robotic Sound]\n[Buzzing sound]" ),
		_Log( GLADOS, "Love....." ),
		_Log( GLADOS, "I think we can put our differences behind us for science you monster" )
	};



	sf::Event Event;
	int log_index = 0;

	while (App.IsOpened()){
		while (App.GetEvent(Event)){
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
			if( Event.Type == sf::Event::KeyPressed ){
				if( Event.Key.Code == sf::Key::Return )
					if( log_index < sizeof(logs) / sizeof(_Log)-1 ) log_index++;

			}
        }


		App.Clear( sf::Color( 0,0,0 ) );

		log_string.SetText( logs[log_index].text );
		App.Draw(log_string);

		log_name.SetText( logs[log_index].name() );
		App.Draw(log_name);

		App.Display();

	}

}
