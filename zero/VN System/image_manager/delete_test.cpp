#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

int main() {
	sf::RenderWindow App(sf::VideoMode( 1280, 720 ), "VN System");
	App.SetFramerateLimit(20); //max FPS

	sf::Image p;
	p.LoadFromFile( "p.jpg" );
	sf::Sprite pp;
	pp.SetImage( p );


	sf::Event Event;
	int t = 1;

	while (App.IsOpened()){
		while (App.GetEvent(Event)){
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
        }


		App.Clear( sf::Color( 0,0,0 ) );

		App.Draw( pp );

		App.Display();

		if( t % 100 == 0 ) 	p.LoadFromFile( "" );
		t++;
	}

}
