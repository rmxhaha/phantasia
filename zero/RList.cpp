/*
	- Remax -

	Sorted singly Linked list
*/

#include <iostream>
#include <random>

using namespace std;

template<class Key, class Value> struct RPair {
	RPair( Key a, Value b ){
		first = a;
		second = b;
	}

	Key first;
	Value second;

	RPair<Key,Value> * next;
};

template<class Key, class Value> class Rlist {
public:
	Rlist(){
		first = 0;
	}

	typedef RPair<Key,Value> ITR;

	template<class F>
	void foreach( F Func ){
		ITR * itr = first;

		while( itr != 0 ){
			Func( *itr );
			itr = itr->next;
		}

	}

	template<class F, class C>
	void foreach( F Func, C Check ){
		ITR * itr = first;

		while( itr != 0 && Check(*itr) ){
			Func( *itr );
			itr = itr->next;
		}

	}


	void add( Key k, Value v ){
		if( first == 0 ) {
			first = new ITR( k, v );
			first->next = 0;
		} else {
			ITR * itr = first;
			ITR * itr2 = 0;

			// Search for insertation
			while( itr->first < k ){
				if( itr->next == 0 ) break; /* insert to the end of array */
				itr2 = itr;
				itr = itr->next;
			}

			if( itr->first >= k ) {
				if( itr2 == 0 ){
					/* Append at the front */
					first = new ITR( k,v );
					first->next = itr;
				} else {
				/* Append somewhere in the middle */
				itr2->next = new ITR( k,v );
				itr2->next->next = itr;

				}
			}
				else // itr->first < k
			{
				/* Append at the back */
				itr->next = new ITR( k,v );
				itr->next->next = 0;
			}
		}
	}

private:
	ITR * first;
};

template<class T> void print( T& data ){
	std::cout << data.first << " " << data.second << std::endl;
}

int main(){
	Rlist<int, int> List;

	cin.get();

    std::random_device rd;

	cout << rd() << endl;

	return 0;

	for( int i = 0; i < 10000; i ++ )
	{
		List.add( 10000 + rd() % 90000, 10000 + rd() % 90000 );
	}

	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;
/*
	List.add( 3, 5 );
	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;

	List.add( 3, 76 );
	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;

	List.add( 10, 5 );
	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;

	List.add( 2, 5 );
	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;

	List.add( 1, 5 );
	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;

	List.add( 5, 5 );
	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;

	List.add( 4, 5 );
	List.foreach( print< RPair<int,int> > ); std::cout << std::endl;
*/

	return 0;
}

