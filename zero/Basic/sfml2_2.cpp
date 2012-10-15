#include <SFML/Audio.hpp>
 #include <SFML/Graphics.hpp>
 
 int main()
 {
     // Create the main window
     sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
 
	 
     // Create a graphical text to display
     sf::Font font;
     if (!font.loadFromFile("PTN57F.ttf"))
         return EXIT_FAILURE;
     sf::Text text("Hello SFML", font, 50);
	 text.setScale( 1.5f, 1.5f );

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
		
		// Draw the string
         window.draw(text);
 
         // Update the window
         window.display();
     }
 
     return EXIT_SUCCESS;
 }