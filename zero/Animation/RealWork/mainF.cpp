#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

struct object {
	float x,y, opacity;
	sf::IntRect Rect;
};

typedef unsigned int uint;

template<class T> class ADeaccel {
private:
	T* element;
	T dtarget;

	float now_time;
	float duration;
public:
	tween()
	{
		element = 0;
		now_time = duration = 0.f;
	}
	
	tween( T& trgt, T delta_trgt, float total_time )
	{
		element = 
	}
	
};


template<class T> class tween {

private:
	T* element;
	T dtarget;

	float now_time;
	float top_time;
	float duration;
	float pts;

public:
	tween( T& elem, T delta_trgt, float total_time, float _pts )
	{
		element = &elem;
		dtarget = delta_trgt;
		now_time = 0.f;
		top_time = total_time * _pts;
		duration = total_time;
		pts = _pts;
	}

	void update( float dt ){
		*element += calculate( now_time, now_time + dt );

		now_time += dt;
	}

	T calculate( uint t1, uint t2 ){
		if( t2 < t1 ) swap( t1, t2 );

		if( t1 < 0 ) t1 = 0;
		if( t2 < 0 ) return 0;
		if( t1 > duration ) return 0;
		if( t2 > duration ) t2 = duration;

		if( t1 < top_time && top_time < t2 ){
			return calculate(t1,top_time) + calculate(top_time,t2); //don't need to over optimize
		}

		if( t2 <= top_time ){
			return dtarget / duration / duration / pts * ( t2*t2 - t1*t1 );
		}

		if( t1 >= top_time ){
			return dtarget / duration / duration / (1-pts) * ( pow( duration-t1, 2 ) - pow( duration-t2, 2 ) );
		}

		cout << "IMPOSSIBLE CASE DEFECT : " << t1 << " " << top_time << " " << t2 << endl;
	}
};

typedef tween<float> 	FloatTween;
typedef tween<int> 		IntTween;


/*
	Although IntTween is available
	The calculation fails in many testcase
	Therefore avoid it as much as possible
	
	
	Tween -> modify calculate
	so that it won't run until the delay time is over
	
	
	object * p;
	p->animateX( 100, 1.f ); 
				destination_x, delta_time

				//not delta_x but destination_x
				
	p->animateY
	p->animateO //opacity
	p->animateW
	p->animateH
	
	p->move( target_x, target_y, duration );
	p->fadeOut( duration ); set opacity to 0%
	p->fadeIn( duration ); set opacity to 100 %
	p->fadeTo( trgt, duration ); set opacity to trgt %
	
	p->collide( float x, float y );
	p->glow( duration, effect_type );
	
	
	AKey->isRunning();	
	AKey->cancel();
	
	//un needed 
	AKey->pause();
	Akey->stop();
	Akey->resume();
	
	const int shortd
	const int 
	
	
	image_t improvement
		collide( float x, float y ); -> not transparent means collide
	
*/

int main(){
	cout << sizeof( tween<float> ) << endl;

	int q = 100;

	cout << q << endl;

	IntTween * tt = new IntTween( q, 1000, 200.f, 0.5f );

	for( int i = 0 ; i <= 200; i += 5 ){
		tt -> update( 10.f );
	}

	cout << q << endl << endl << endl;



	object p;
	p.x = 100;


	cout << p.x << endl;
	FloatTween * t = new FloatTween( p.x, 1000.f, 100, 0.5f );

	for( int i = 0; i <= 100; i+=10 )
	{
		cout << i << " " << p.x << endl;
		t->update( 10 );
	}

	cout << p.x << endl;



	return 0;
}
