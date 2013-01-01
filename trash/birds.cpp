#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

class bird_t {
public:
	bird_t( int x, int y, int speed_ ){
		this -> x = x; // ini maksud gw naming conflict
		this -> y = y;
		
		speed = speed_;
		isFlying = false;
		isSleeping = true;
	}
	
	void flyTo( int x_, int y_ ){
		if( !isSleeping ){
			isFlying = true;
			
			direction = atan2( x_ - x, y_ - y );
			trgt_x = x_;
			trgt_y = y_;
		}
	}
	
	void update( float delta_time ){
		if( isFlying ){
			if( x != trgt_x && y != trgt_y ){
				x += cos( direction ) * speed * delta_time;
				y += sin( direction ) * speed * delta_time;
			} else isFlying = false;
		}
	}
	
	void wakeup(){
		isSleeping = false;
	}
	
	bool stillFlying(){
		return isFlying;
	}
	
private:
	int x;
	int y;
	int trgt_x;
	int trgt_y;
	
	float direction;
	
	bool isFlying;
	bool isSleeping;
	int speed;
};


int main(){
	vector< bird_t > birds;
	birds.push_back( bird_t( 10, 10, 20 ) );
	birds.push_back( bird_t( 20, 10, 20 ) );
	birds.push_back( bird_t( 30, 10, 20 ) );

	for( int i = 0; i < birds.size(); ++i ){
		birds[i].wakeup();
		birds[i].flyTo( 0,0 );
	}
	
	
	while( birds.size() != 0 )
	{
		for( int i = 0; i < birds.size(); ++i ){
			if( !birds[i].stillFlying() ){
				birds.erase( birds.begin() + i ); // kill the bird
			}
				else 
			{
				birds[i].update( 0.001 );
			}
		}
	}
	
	return 0;
}