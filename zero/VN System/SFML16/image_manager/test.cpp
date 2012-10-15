#include "image_manager.cpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode( 1024, 768, 32 ), "Image-Manager" );

	image_manager img_mgr;
	img_mgr.add_resource_directory("media/" );
	img_mgr.add_resource_directory("media/images/" );
	// Just for testing we're removing a directory
	img_mgr.remove_resource_directory("media/images/" );
	
	sf::Sprite test_sprite[20];
	sf::Sprite other_sprite[20];
	// initialise all sprites
	for( int i = 0; i < 20; ++i )
	{
		test_sprite[i].SetImage( img_mgr.get_image( "test.png" ) );
		other_sprite[i].SetImage( img_mgr.get_image( "other.png" ) );

		test_sprite[i].SetPosition( i*40, 0 );
		other_sprite[i].SetPosition( i*50, 100 );
	}

    while (window.IsOpened())
    {
        sf::Event Event;
        while (window.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                window.Close();
			// You should not do this, but we will; just to verify that it works correctly ;)
			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space )
				img_mgr.delete_image( "test.png" );
			if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::LControl )
				img_mgr.delete_image( img_mgr.get_image( "other.png" ) );
        }

        window.Clear();

		// Draw sprites
		for( int i = 0; i < 20; ++i )
		{
			window.Draw( test_sprite[i] );
			window.Draw( other_sprite[i] );
		}

        window.Display();
    }

    return EXIT_SUCCESS;
}