/*
	By Remax From Synergy
		2012
*/

#include <iostream>
#include <cmath>
#include <list>
#include <iomanip>
#include <limits>
#include <ctime>
#include <cassert>


using namespace std;

float round_by_3( const float& input ){
	return floor( input * 1000.f ) / 1000.f;
}

namespace TweenFx {
	template< class T > class tween {
	public:
		typedef float (*AnimationCalc_t)( float );

		tween( T& elem, const T& delta_data_, AnimationCalc_t ACalc, float duration_ = 0.3f, float delay_time_ = 0.f, int repeat_ = 1 ){
			element_ = &elem;
			delta_data = delta_data_;
			AnimationCalc = ACalc;

			duration = duration_;
			delay_time = delay_time_;
			repeat = repeat_;
			repeat_index = 0;
			now_time = 0.0;
		}

		T sigma_dx() {
			return delta_data * ( repeat + AnimationCalc( now_time / duration ) );
		}

		T dx() {
			return AnimationCalc( now_time / duration );
		}

		void update( const float& dt )
		{
			assert( dt > 0 );

			float end_time = delay_time + duration;
			float last_time;

			last_time = now_time;
			now_time += dt;

			do {

				if( now_time <= delay_time ) return; // waiting ...

				/* Delay no more */

				if( duration == 0.f ) // special case
				{
					*element_ += delta_data;

					now_time = now_time - end_time;

					// Add repeat
					repeat_inc();

					continue;
				}

				if( last_time < delay_time ) last_time = delay_time;

				if( now_time < end_time ) {
					manip_elem( last_time, now_time );

				} else if( last_time < end_time ) {
					manip_elem( last_time, end_time );

					now_time = now_time - end_time;

					// App repeat
					repeat_inc();

					// due to inaccurate float data
					// Data is rounded by 3 everytime the animation finishes
					*element_ = round_by_3( *element_ );

					// Just in case
					continue;
				}

			} while( end_time <= now_time && repeat != repeat_index );
		}

	private:

		void manip_elem( float t1, float t2 ){
			assert( t1 < t2 );
			assert( duration > 0 );

			t1 -= delay_time;
			t2 -= delay_time;

			assert( 0 <= t1 && t1 <= duration );
			assert( 0 <= t2 && t2 <= duration );

			t1 = AnimationCalc( t1 / duration );
			t2 = t2 < duration ? AnimationCalc( t2 / duration ) : 1.0;

			*element_ += ( t2 - t1 ) * delta_data;
		}

		void repeat_inc(){
			if( repeat > 0 && repeat_index == repeat ) return; // All the animation have been done

			// Be safe for : Unlimited loop
			if( repeat_index < numeric_limits<int>::max() ) repeat_index ++;
		}

		// Function pointer to the wanted function
		AnimationCalc_t AnimationCalc;

		// Target Element
		T * element_;

		// Delta to target
		T delta_data;

		// Delay Before the animation starts
		float delay_time;

		float now_time;

		// Animation Duration
		float duration;

		// How many times to repeat
		int repeat;

		// How many times have the animation been repeated
		int repeat_index;

	};

	float Constant( float p ){
		return p;
	}

	float Deaccel( float p ){
		return 1 - pow( 2, -10 * p );
	}

	float Accel( float p ){
		return pow(2, 10 * (p - 1)) - 0.001;
	}

	float SeeSaw( float p ){
		if( p < 0.5 ){
			return 0.5 * pow( 2, 10 * ( p * 2 - 1 ) );
		} else {
			return 0.5 * (2 - pow(2, -10 * ( p * 2 - 1 )));
		}
	}

};


int main(){
	float a = 0;
	TweenFx::tween<float> xoxo( a, 100.f, TweenFx::SeeSaw,1.f, 0.5f, 2 );

	for( int i = 0; i < 151; ++i ){
		xoxo.update( 0.01f );
		cout << a << endl;
	}

	return 0;
}
