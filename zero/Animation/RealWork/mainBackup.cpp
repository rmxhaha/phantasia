/*
	- Synergy -
	Code By Remax
*/

#include <iostream>
#include <cmath>
#include <list>
#include <iomanip>
#include <typeinfo>

using namespace std;

const float PI = atan2( 0.f, -1.f );
const float FULL_CIRCLE = PI * 2;
namespace Fx {

	enum transition_t
	{
		CONSTANT_ANIMATION,
		DEACCEL_ANIMATION,
		ACCEL_ANIMATION,
		SIN_ANIMATION,
		COS_ANIMATION,
		NSIN_ANIMATION,
		NCOS_ANIMATION,

		RATIO23_ANIMATION,
		RATIO32_ANIMATION,
		RATIO11_ANIMATION
	};

	bool isLiner( transition_t trans ){
		switch( trans ){
			case CONSTANT_ANIMATION: case DEACCEL_ANIMATION: case ACCEL_ANIMATION: case RATIO23_ANIMATION: case RATIO32_ANIMATION: case RATIO11_ANIMATION: return true;
			default: return false;
		}
	}

	bool isHarmonic( transition_t trans ){
		switch( trans ){
			case SIN_ANIMATION: case COS_ANIMATION: case NSIN_ANIMATION: case NCOS_ANIMATION: return true;
			default: return false;
		}
	}

	template< class T = float > class delay_tween {
	public:
		delay_tween( T& elem, T delta_data, float delay_time, int repeat = 1 ){
			this -> element_ = &elem;
			this -> delta_data = delta_data;
			this -> delay_time = delay_time;
			this -> repeat = repeat;
			this -> now_time = 0.f;
		}

		void update( float dt ){
			if( !isAlive() ) return;
			now_time += dt;

			if( now_time >= delay_time ){
				*element_ += delta_data;
				now_time = now_time - delay_time;
				repeat --;
			}
		}

		bool isAlive() const {
			return repeat != 0;
		}

		const T * const element() const { return element_; }
	private:
		T * element_;
		T delta_data;
		float delay_time;
		float now_time;
		int repeat;
	};

	template< class T = float > class tween { //Fx::tween
	public:
		tween( T& elem, T delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 ) {
			this -> now_time = 0.f;
			this -> delta_data = delta_data;
			this -> element_ = &elem;
			this -> duration = duration;
			this -> delay_time = delay_time;
			this -> repeat = repeat;
		}

		void update( const float& dt )
		{
			if( completed() ) return;
			if( dt == 0.f ) return;

			float t2 = now_time + dt;

			if( now_time < delay_time ) {
				if( t2 <= delay_time ){
					now_time = t2;

					return;	//still waiting for the delay
				}

				now_time = delay_time;
			}

			if( duration == 0.f ) { //emergency case ( will never happenened ) but just be safe if modified by another programmer
				*element_ += delta_data;
			} else {
				float etime = delay_time + duration;
				if( now_time < etime && etime <= t2 ){
					t2 = etime;
				}

				calculate( now_time - delay_time, t2 - delay_time );

				now_time = t2;
			}
		}
	private:
		bool completed()
		{
			/*
				explenation:
					repeat <= -1 	means infinite loop
					repeat == 0 	means it won't loop anymore
			*/
			if( duration + delay_time <= now_time ){ //check first
				if( repeat == 0 ) goto end;
				if( repeat > 0 ) repeat--;

				now_time = 0.f;
			}

			end:
			return !isAlive(); //if dead means it's completed
		}


		// "calculate()" has no safety feature so keep it safe before calling
		virtual void calculate( float t1, float t2 )
		{
			/*
			switch( trans ) {
				case CONSTANT_ANIMATION:
					*element_ += ( t2 - t1 ) * delta_data / duration;
					break;

				case DEACCEL_ANIMATION:
					*element_ += delta_data * ( pow( t2,2 ) - pow( t1, 2 ) ) / duration / duration;
					break;

				case ACCEL_ANIMATION:
					*element_ += delta_data * ( pow( duration - t1, 2 ) - pow( duration - t2, 2 ) ) / duration / duration;
					break;
			}
			*/
		}
	public:
		bool isRunning() const {
			return ( delay_time < now_time && now_time < duration + delay_time );
		}

		bool isAlive() const {
			return repeat != 0;
		}

		const T * const element() const { return element_; }
	private:
		T * element_;
		T delta_data;
		float delay_time;
		float now_time;
		float duration;
		int repeat;

		friend class const_tween;
		friend class deaccel_tween;
		friend class accel_tween;
		friend class sin_tween;
		friend class cos_tween;
		friend class nsin_tween;
		friend class ncos_tween;
		friend class ratio_tween;
	};

	class const_tween : public tween<float> {
    public:
		const_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {}

	private:
		void calculate( float t1, float t2 ) {
			*element_ += ( t2 - t1 ) * delta_data / duration;
		}
	};

	class accel_tween : public tween<float> {
	public:
		accel_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {}

	private:
		void calculate( float t1, float t2 ){
			*element_ += delta_data * ( pow( t2,2 ) - pow( t1, 2 ) ) / duration / duration;
		}
	};

	class deaccel_tween : public tween<float> {
	public:
		deaccel_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {}

	private:
		void calculate( float t1, float t2 ) {
			*element_ += delta_data * ( pow( duration - t1, 2 ) - pow( duration - t2, 2 ) ) / duration / duration;
		}
	};

	class sin_tween : public tween<float> {
	public:
		sin_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {
			omega = PI * 2 / duration;
		}
	private:
		void calculate( float t1, float t2 ){
			*element_ += delta_data * ( sin( omega * t2 ) - sin( omega * t1 ) );
		}

		float omega;
	};

	class cos_tween : public tween<float> {
	public:
		cos_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {
			omega = PI * 2 / duration;
		}
	private:
		void calculate( float t1, float t2 ){
			*element_ += delta_data * ( cos( omega * t2 ) - cos( omega * t1 ) );
		}

		float omega;
	};

	class nsin_tween : public tween<float> {
	public:
		nsin_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {
			omega = PI * 2 / duration;
		}
	private:
		void calculate( float t1, float t2 ){
			*element_ += delta_data * ( sin( omega * t1 ) - sin( omega * t2 ) );
		}

		float omega;
	};

	class ncos_tween : public tween<float> {
	public:
		ncos_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {
			omega = PI * 2 / duration;
		}
	private:
		void calculate( float t1, float t2 ){
			*element_ += delta_data * ( cos( omega * t1 ) - cos( omega * t2 ) );
		}

		float omega;
	};

	class ratio_tween : public tween<float> {
	public:
		ratio_tween( float& elem, float delta_data, float duration = 0.3f, float delay_time = 0.f, int repeat = 1, float p_ = 0.6 )
		: tween<float>( elem, delta_data, duration, delay_time, repeat ) {
			p = p_;
		}

		float ratio() const {
			return p;
		}

	private:
		float p;

		void calculate( float t1, float t2 ) {
			float boundary = duration * p;

			if( t1 < boundary && boundary < t2 ){
				*element_ += delta_data / duration / duration * (
					p * ( t2*t2 - t1*t1 ) +
					( 1 - p ) * ( pow( duration - t1, 2 ) - pow( duration - t2, 2 ) )
				);
			}

			else if( t2 <= boundary ){
				*element_ += delta_data / duration / duration / p * ( t2*t2 - t1*t1 );
			}

			else if( t1 >= boundary ){
				*element_ += delta_data / duration / duration / ( 1 - p ) * ( pow( duration - t1, 2 ) - pow( duration - t2, 2 ) );
			}
		}

	};

}


class Effect_t {
private:

	list< Fx::const_tween > 	const_tween_pool;
	list< Fx::accel_tween > 	accel_tween_pool;
	list< Fx::deaccel_tween > 	deaccel_tween_pool;
	list< Fx::sin_tween > 		sin_tween_pool;
	list< Fx::cos_tween > 		cos_tween_pool;
	list< Fx::nsin_tween > 		nsin_tween_pool;
	list< Fx::ncos_tween > 		ncos_tween_pool;

	list< Fx::delay_tween<float> > delay_tween_pool;
	list< Fx::ratio_tween > ratio_tween_pool;

public:
	void Animate( float& from, float delta_data, Fx::transition_t trans = Fx::DEACCEL_ANIMATION, float duration = 0.3f, float delay = 0.f, int repeat = 1 ){
		using namespace Fx;

		if( delta_data == 0 ) return; //nothing to do here

		if( duration > 0.f ) goto normal_case;

		// duration <= 0.f
		if( delay <= 0.f ){
			//don't need to wait
			from += delta_data * repeat;

		} else {
			if( isHarmonic( trans ) ) return;

			// NOTE : The math causes problem
			//			sin( x / 0 ) is undefined
			//			sin( Infinity ) is undefined

			//			so don't count what is undefined

			if( isLiner( trans ) ){
				delay_tween_pool.push_back( Fx::delay_tween<float>( from, delta_data, delay, repeat ) );

				// NOTE : think of it's happening in time close to zero ( limit t->0 );
			}
		}

		//end of abnormal case
		return;

		//normal_case where nothing is strange
		normal_case:

		switch( trans )
		{
			case DEACCEL_ANIMATION:
				deaccel_tween_pool.push_back( Fx::deaccel_tween( from, delta_data, duration, delay, repeat ) );
				break;

			case CONSTANT_ANIMATION:
				const_tween_pool.push_back( Fx::const_tween( from, delta_data, duration, delay, repeat ) );
				break;

			case ACCEL_ANIMATION:
				accel_tween_pool.push_back( Fx::accel_tween( from, delta_data, duration, delay, repeat ) );
				break;

			case SIN_ANIMATION:
				sin_tween_pool.push_back( Fx::sin_tween( from, delta_data, duration, delay, repeat ) );
				break;

			case COS_ANIMATION:
				cos_tween_pool.push_back( Fx::cos_tween( from, delta_data, duration, delay, repeat ) );
				break;

			case NSIN_ANIMATION:
				nsin_tween_pool.push_back( Fx::nsin_tween( from, delta_data, duration, delay, repeat ) );
				break;

			case NCOS_ANIMATION:
				ncos_tween_pool.push_back( Fx::ncos_tween( from, delta_data, duration, delay, repeat ) );
				break;
			case RATIO23_ANIMATION:
				ratio_tween_pool.push_back( Fx::ratio_tween( from, delta_data, duration, delay, repeat, 0.4 ) );
				break;
			case RATIO11_ANIMATION:
				ratio_tween_pool.push_back( Fx::ratio_tween( from, delta_data, duration, delay, repeat, 0.5 ) );
				break;
			case RATIO32_ANIMATION:
				ratio_tween_pool.push_back( Fx::ratio_tween( from, delta_data, duration, delay, repeat, 0.6 ) );
				break;

				/*
					RATIO23_ANIMATION and RATIO32_ANIMATION have been tested
					and they are a total bug
					re written version of it is being developed
				*/
		}

	}

private:
	template< class TPool > void updateFxList( TPool& Pool, float dt )
	{

		// iterate trough the whole array
		for( auto itr = Pool.begin(); itr != Pool.end(); ){

			// remove animation that is dead
			if( !itr -> isAlive() ){

				// erase function return the element of the next
				itr = Pool.erase( itr );

				continue;
			}

			// do the update
			itr -> update( dt );

			// Next !
			itr++;
		}
	}

	template< class TPool > void removeFxFromList( TPool& Pool, const float& target )
	{
		for( auto itr = Pool.begin(); itr != Pool.end(); ){
			if( itr->element() == &target ){
				itr = Pool.erase( itr );

				continue;
			}

			itr++;
		}
	}

public:

	Effect_t(){

	}

	void Update( const float dt )
	{
		updateFxList( const_tween_pool, dt );
		updateFxList( accel_tween_pool, dt );
		updateFxList( deaccel_tween_pool, dt );

		updateFxList( sin_tween_pool, dt );
		updateFxList( cos_tween_pool, dt );
		updateFxList( nsin_tween_pool, dt );
		updateFxList( ncos_tween_pool, dt );

		updateFxList( delay_tween_pool, dt );

		updateFxList( ratio_tween_pool, dt );
	}

	void RemoveFx( const float& elem )
	{
		removeFxFromList( const_tween_pool, elem );
		removeFxFromList( accel_tween_pool, elem );
		removeFxFromList( deaccel_tween_pool, elem );

		removeFxFromList( sin_tween_pool, elem );
		removeFxFromList( cos_tween_pool, elem );
		removeFxFromList( nsin_tween_pool, elem );
		removeFxFromList( ncos_tween_pool, elem );

		removeFxFromList( delay_tween_pool, elem );
		removeFxFromList( ratio_tween_pool, elem );
	}



	/*
		Syntax in dev:

			Fx::Animate( float& from, float to, transition_t trans, float duration, float delay, int repeat ); //remmeber to keep documentation
			Fx::Update( float dt );
			Fx::Stop( x );

			if "Stop" is called then the current animation will stop
			if nothing is on the run ( isRunning() == false ) then nothing will happenened

			Fx::Remove( x );

			if "Remove" is called then all the animation pointing to that data will be removed

			Fx::Revert( x, trans, time );


			Fx::Blur( sf::Image, int blur_width, int blur_height, float duration, float delay );


			delta_data serve as a amplitude in harmonics tween ( sin_tween, cos_tween, etc. )

			Fix all harmonics that cause the imperfect calculation
			
			what's worth more counting the omega or keep the omega ?
	*/


};

int main(){
	Effect_t * Fx = new Effect_t();

	cout << sizeof( Effect_t ) << endl;

	float x = 0.f;
	float y = 0.f;

	Fx -> Animate( x, 1000.f, Fx::RATIO11_ANIMATION, 1.f, 0.f, 1 );
	Fx -> Animate( y, 1000.f, Fx::DEACCEL_ANIMATION, 1.f, .5f, -1 );

	for( int i = 0; i < 100; ++i )
	{
		cout << setw( 12 ) << x << setw( 12 ) << y << endl;

		Fx->Update( 0.1f );
	}

	cout << setw( 12 ) << x << setw( 12 ) << y << endl;

//	Fx::Option<float> ZZZ( x );
//	ZZZ.to( 2000.f ).transition( Fx::DEACCEL_ANIMATION ).delay( 0.f ).time( 1.f );

	return 0;
}
