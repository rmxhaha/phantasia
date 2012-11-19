#include "RImage.cpp"

int main(){
	image_t * image  = get_image( "image1.jpg" );
	image_t * image2 = get_image( "image2.jpg" );

	ResetImageNeeds();

	image -> needed( true );
	image2 -> needed( true );

	ManageImage();

	image -> setOpacity( 50 );

	sf::RenderWindow App(sf::VideoMode( 1280, 720 ), "VN System", 1 << 2 ); //480p for testing
	App.setFramerateLimit( 20 ); //Maximum FPS

	sf::Event Event;

	while ( App.isOpen() ){
		while (App.pollEvent(Event)){
            // Close window : exit
            if ( Event.type == sf::Event::Closed )
                App.close();
		}


		App.clear( sf::Color( 0,0,0 ) );

		image2 -> draw_unsafe( App,0,0 );
		image -> draw_unsafe( App,0,0 );

		App.display();

	}
	
	return 0;
}
