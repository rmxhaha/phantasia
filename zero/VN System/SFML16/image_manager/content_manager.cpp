#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace std;

struct image_count {
	string filename;
	sf::Image image;
	int turus;

	image_count() : turus( 0 ) {
	}
};

class image_manager {
public:
	image_manager();
	~image_manager();

private:
	image_manager( const image_manager& );
	image_manager& operator =( const image_manager& );

public:
	const sf::Image&	get_image( const std::string& filename );
	void 				delete_all();
	void				delete_image( const std::string& filename );
	void				add_resource_directory( const std::string& directory );
	void				remove_resource_directory( const std::string& directory );
	void				set_max_image( unsigned int length );

private:
	void 				delete_rare();

	int maximum;

	vector< image_count > images_;
	vector< string > resource_directories_;
};

image_manager::image_manager() : images_(), resource_directories_() {
	//default values
	maximum = 20;
}

image_manager::~image_manager() {
	images_.clear();
	resource_directories_.clear();
}

void image_manager::delete_rare() {
	vector< image_count >::iterator rarest = images_.begin();
	for( vector< image_count >::iterator it = images_.begin();
		it != images_.end();
		++it )
	{
		if( it -> turus < rarest -> turus ){
			rarest = it;
		}
	}

	images_.erase( rarest );
}

const sf::Image& image_manager::get_image( const std::string& filename ) {
	for( int i = 0; i < images_.size(); ++i ){
		if( filename == images_[i].filename ) {
			cout << "DEBUG : " << filename << " existed\n";

			images_[i].turus ++;
			return images_[i].image;
		}
	}

	image_count new_image;
	new_image.filename = filename;

	// Search project's main directory
	if( new_image.image.LoadFromFile( filename ) )
	{
		images_.push_back( new_image );
		std::cout << "DEBUG : loading " << filename << " ... " << endl;
		return images_.back().image;
	}

	for( vector<string>::iterator it = resource_directories_.begin();
		it != resource_directories_.end();
		++it )
	{
		if( new_image.image.LoadFromFile( (*it) + filename ) ){
			images_.push_back( new_image );
			std::cout << "DEBUG : loading " << filename << " ... " << endl;
			return images_.back().image;
		}
	}

	cout << "GAME_ERROR: Image was not found. It is filled with an empty image.\n";
	return new_image.image;
}

void image_manager::set_max_image( unsigned int length ){
	if( length == 0 ) length = 1;

	maximum = length;
}

void image_manager::delete_image( const std::string& filename ) {
	for( int i = 0; i < images_.size(); ++i ){
		if( images_[i].filename == filename ){
			images_.erase( images_.begin() + i );
			return;
		}
	}
}

void image_manager::add_resource_directory( const std::string& directory ){
	// Check whether the path already exists
	for( vector< string >::iterator it  = resource_directories_.begin();
		 it != resource_directories_.end();
		++it )
	{
		// The path exists. So it isn't necessary to add id once more.
		if( directory == (*it) )
			return;
	}

	// insert the directory
	resource_directories_.push_back( directory );
}

void image_manager::remove_resource_directory( const std::string& directory )
{
	for( std::vector<std::string>::iterator it  = resource_directories_.begin();
		 it != resource_directories_.end(); )
	{
		// The path exists. So it isn't necessary to add id once more.
		if( directory == (*it) )
			it = resource_directories_.erase( it );
		else
			++it;
	}
}

void image_manager::delete_all() {
	images_.clear();
}


int main(){
	sf::RenderWindow App(sf::VideoMode( 800, 600 ), "Content Manager");
	App.SetFramerateLimit(20); //max FPS

	image_manager manager;
	sf::Event Event;

	sf::Sprite Spr;
	sf::Sprite Spr2;
	Spr.SetX( 0 );
	Spr.SetY( 0 );
	Spr2.SetX( 1 );
	Spr2.SetY( 1 );

//	sf::Clock Clk;
	for( int i = 1; i <= 11; ++i ){
//		stringstream ss;
//		ss << i << ".jpg";
//		manager.get_image( ss.str() );
//		cout << Clk.GetElapsedTime() << endl;
//		Clk.Reset();
	}


	manager.set_max_image( 10 );

	Spr.SetImage( manager.get_image( "6.jpg" ) );
	Spr.SetImage( manager.get_image( "6.jpg" ) );
	Spr2.SetImage( manager.get_image( "7.jpg" ) );

	int i = 1;
	int p = 1;

	while (App.IsOpened()){
		while (App.GetEvent(Event)){
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();
			if( Event.Type == sf::Event::KeyPressed ){
				if( Event.Key.Code == sf::Key::A )
					Spr.Move( -10, 0 );
				if( Event.Key.Code == sf::Key::D )
					Spr.Move( 10, 0 );
				if( Event.Key.Code == sf::Key::S )
					Spr.Move( 0, 10 );
				if( Event.Key.Code == sf::Key::W )
					Spr.Move( 0, -10 );
//				if( Event.Key.Code == sf::Key::X )
//					manager.delete_image( "4.jpg" );

			}

		}





		// if( p % 20 == 0 ){
			// stringstream ss;
			// ss << i << ".jpg";
			// i = i % 10 + 1;
			// Spr.SetImage( manager.get_image( ss.str() ) );
		// }

		App.Clear( sf::Color( 0,0,0 ) );

		App.Draw( Spr2 );

		App.Draw( Spr );

		App.Display();


		p++;
	}

}
