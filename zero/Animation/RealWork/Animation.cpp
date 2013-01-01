/*
	By Remax From Synergy
		2012
*/

#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <iomanip>
#include <limits>
#include <ctime>
#include <cassert>


using namespace std;

template< size_t N >double round_by( const double& input ) {
	static const double power = pow( 10.f, N );
	return round( input * power ) / power;
}

const double PI = atan2( 0.f, -1.f );
const double HALF_PI = PI / 2;
const double TWO_PI = PI * 2;

namespace TweenFx {
	template< class T > class tween_t {
	public:
		typedef double (*AnimationFunc_t)( double );

		tween_t( T& elem, const T& delta_data_, AnimationFunc_t ACalc, double duration_ = 0.3f, double delay_time_ = 0.f, int repeat_ = 1 )
		{
			element_ = &elem;
			delta_data = delta_data_;
			AnimationFunc = ACalc;

			duration = duration_;
			delay_time = delay_time_;
			end_time = delay_time + duration;
			repeat = repeat_;

			repeat_index = 0;
			now_time = 0.0;
			Func_Range = ACalc( 1 ) - ACalc( 0 );
			sigma_dx_ = 0.0;
		}

		T sigma_dx() const {
			return sigma_dx_;
		}

		void update( const double& dt )
		{
			assert( dt > 0 );

			double last_time;
			double temporary;

			last_time = now_time;
			now_time += dt;

			if ( repeat == repeat_index ) return; // Animation has ended

			do {
				if ( now_time <= delay_time ) return; // waiting ...

				/* Delay no more */

				if ( duration == 0.f ) // special case
				{
					*element_ += delta_data;

					now_time = now_time - end_time;

					// Add repeat
					repeat_inc();

					continue;
				}

				if ( last_time < delay_time ) last_time = delay_time;

				if ( now_time < end_time ) {
					temporary = getDelta( last_time, now_time );
					*element_ += temporary;
					sigma_dx_ += temporary;

				} else if ( last_time < end_time ) {
					temporary = getDelta( last_time, end_time );
					*element_ += temporary;
					sigma_dx_ += temporary;

					now_time = now_time - end_time;

					// App repeat
					repeat_inc();

					// Just in case
					continue;
				}

			} while( end_time <= now_time && repeat != repeat_index );
		}

		bool hasFinished() const {
			return repeat_index == repeat;
		}

		double timeLeft() const {
			if( repeat < 0 ) return INFINITY;

			return ( repeat - repeat_index ) * end_time - now_time;
		}

		double timeElapsed() const {
			return repeat_index * end_time + now_time;
		}

		AnimationFunc_t getFunc() const {
			return AnimationFunc;
		}

		void reverse(){
			reverse( timeElapsed(), 0.f, AnimationFunc );
		}

		void reverse( const double& duration_, double delay_time_ = 0.f ){
			reverse( duration_, delay_time_, AnimationFunc );
		}

		void reverse( double duration_, double delay_time_ , AnimationFunc_t ACalc )
		{
			if( ACalc != AnimationFunc ) {
				AnimationFunc = ACalc;
				Func_Range = ACalc( 1 ) - ACalc( 0 );
			}

			delta_data = -sigma_dx();

			duration = duration_;
			delay_time = delay_time_;
			end_time = duration_ + delay_time_;

			repeat = 1;
			repeat_index = 0;
			now_time = 0.0;
			sigma_dx_ = 0.0;
		}
		
		void force_end(){
			repeat_index = repeat;
		}
		
	private:

		double getDelta( double t1, double t2 ) {
			assert( t1 < t2 );
			assert( duration > 0 );

//			cout << setw( 10 ) << t1 << setw( 10 ) << t2;

			t1 -= delay_time;
			t2 -= delay_time;

			assert( 0.f <= t1 );
			assert( t1 <= duration );
			assert( 0.f <= t2 );
			assert( t2 <= duration );

			t1 = AnimationFunc( t1 / duration );
			t2 = AnimationFunc( t2 / duration );

			return ( ( t2 - t1 ) / Func_Range ) * delta_data;

//			cout << setw( 10 ) << t1 << setw( 10 ) << t2 << setw( 10 ) << ( t2 - t1 ) / Func_Range << endl;
		}

		void repeat_inc(){
			if( repeat > 0 && repeat_index == repeat ) return; // All the animation have been done

			// Be safe for : Unlimited loop
			if( repeat_index < numeric_limits<int>::max() ) repeat_index ++;
		}

		// Function pointer to the wanted function
		AnimationFunc_t AnimationFunc;

		// Function Range
		double Func_Range;

		// Target Element
		T * element_;

		// Delta to target
		T delta_data;

		// Delta since the beginning
		T sigma_dx_;

		// Delay Before the animation starts
		double delay_time;

		double now_time;

		// total time per cycle
		double end_time;

		// Animation Duration
		double duration;

		// How many times to repeat
		int repeat;

		// How many times have the animation been repeated
		int repeat_index;
	};

	template< class T > class timeline_t {
	public:
		timeline_t(){
			paused = false;
		}
		
		tween_t<T> * append( tween_t<T> * data ){
			array.push_back( data );
			return data;
		}

		void update( unsigned dt ){
			this -> update( ( double ) dt / 1000.0 );
		}

		void update( const double& dt ){
			assert( dt > 0.0 );
			if( !paused ){
				auto itr = array.begin();
				while( itr != array.end() ){
					if( (**itr).hasFinished() ){
						itr = array.erase( itr );
						continue;
					}

					(**itr).update( dt );

					itr ++;
				}
			}
		}
		
		void pause() { paused = true; }
			
		void resume() { paused = false; }

		
	private:
		bool paused; // 3 bit loss
		list< tween_t<T>* > array;
	};
	
	timeline_t<double> TweenEngine;
	
	typedef tween_t<double> tween;
	
		
	
	double Constant( double p ) {
		return p;
	}

	double Deaccel( double p ) {
		return 1 - pow( 2, -10 * p );
	}

	double Accel( double p ) {
		return pow(2, 10 * (p - 1)) - 0.001;
	}

	double SeeSaw( double p ) {
		if( p < 0.5 ){
			return 0.5 * pow( 2, 10 * ( p * 2 - 1 ) );
		} else {
			return 0.5 * (2 - pow(2, -10 * ( p * 2 - 1 )));
		}
	}

	double SinUnder( double p ) {
		return 1.0 - cos( p * HALF_PI );
	}

	double SinUpper( double p ) {
		return sin( p * HALF_PI );
	}

	double BounceIn( double p ) {
		p = 1 - p;
		if ( p < 1 / 2.75) {
			return 1.0 - (7.5625 * p * p);

		} else if ( p < 2 / 2.75 ) {
			p -= 1.5 / 2.75;
			return 1 - (7.5625 * p * p + .75);

		} else if (p < 2.5 / 2.75) {
			p -= 2.25 / 2.75;
			return 1 - (7.5625 * p * p + .9375);

		} else {
			p -= 2.625 / 2.75;
			return 1 - (7.5625 * p * p + .984375);
		}
	}


};

/*
	BUGS LISTS
		-
	WANTED FEATURE
		-
*/

int main() {
	using namespace TweenFx;

	cout << sizeof( tween_t<double> ) << endl;

/*
	double z = 0.f;
	for( int i = 0; i <= 5; i ++ ) {
		z += ( SeeSaw( ( i + 1 ) / 5 ) - SeeSaw( i / 5 ) );
	}

	cout << z << endl;
	cout << SeeSaw( 1 ) << endl;
	cout << SeeSaw( 0 ) << endl;

	return 0;
*/

	double a = 20000.0;
	clock_t t  = clock();
	
	for( double i = 0.0; i<100000000.0; i+= 1.0 );
	clock_t timper10M = clock() - t;
	
	cout << "Time on 10M itr " << timper10M << endl;
	
	t = clock();
	

	cout << a << endl;
	
	TweenEngine.append( new tween( a, 100.0, TweenFx::SeeSaw, 0.5f, 0.5f, -1 ) );
	tween * xoxo = TweenEngine.append( new tween( a, 10000.0, TweenFx::Deaccel, 0.5f, 0.5f, 2 ) );

	for( unsigned g = 0; g < 20000; g ++ ){
		for ( unsigned i = 0; i < 200; ++i ) {
			TweenEngine.update( 10u );
		}

		xoxo->reverse( 2.f );

		for ( unsigned i = 0; i < 200; ++i ) {
			TweenEngine.update( 0.01 );
		}
		
		xoxo->reverse( 2.f );
	}

	cout << a << endl;
	
	double p = ( clock() - t ) / 8000.0;
	cout << "Time per 1000 update : "<<  p << " Miliseconds" << endl;
	cout << "Average Iteration per 1000 calculation : " << p / ( timper10M / 100000000.0 );
	return 0;
}
