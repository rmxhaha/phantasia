#include <SFML/Graphics.hpp>

sf::Image blank;
void null_image( sf::Texture& texture ){
	texture.loadFromImage( blank );
}

int main(){
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	window.setFramerateLimit( 20 ); //max FPS

	blank.create( 1, 1, sf::Color(0,0,0,0) );

	sf::Texture texture;

	if (! texture.loadFromFile("image.jpg"))
		return EXIT_FAILURE;

	sf::Sprite sprite(texture);

	int p = 1;

    // Start the game loop
     while (window.isOpen())
     {
         // Process events
         sf::Event event;
         while (window.pollEvent(event))
         {
             // Close window : exit
             if (event.type == sf::Event::Closed)
                 window.close();
         }

         // Clear screen
         window.clear();

         // Draw the sprite
         window.draw(sprite);
		if( p % 100 == 0 ) null_image( texture );
         // Update the window
         window.display();
		 p++;
     }



     return EXIT_SUCCESS;

}
