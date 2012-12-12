#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <algorithm>

/************************************
* Double kill After Endless Dividing
*************************************/

using namespace std;


typedef unsigned long long ulong;
typedef unsigned int uint;

uint primes[] = {
    224284387,
    158383429
};

int main(){
    uint
        length_1 = (ulong)log10(primes[0]),
        p_1 = pow( 10, length_1 ),

        length_2 = (ulong)log10(primes[1]),
        p_2 = pow( 10, length_2 );
    ulong
        a_1 = p_1,
        a_2 = p_2,
        b_1,b_2;
    int base = 256;

    string out;


int count = 0;

int statistics[ base ];
for( int i = 0; i < base; i++ ) statistics[i] = 0;

endless:

	a_1 *= p_1;
	a_2 *= p_2;

    b_1 = a_1 / primes[0];
    b_2 = a_2 / primes[1];

    while( b_1 > base and b_2 > base ){
		// statistics[ b_1 % ( b_2 % base + 1 ) ] ++; // uneven distribution
        statistics[ ( b_1 ) % base ] ++;
        statistics[ ( b_2 ) % base ] ++;
		
		//b_1 = b_1 / ( b_2 % base + 1 );
		b_1 = b_1 / base;
        b_2 = b_2 / base;
		count++;
	}
	
	cout << flush;
	cin.get();

    if( count >= ( 1 << 20 ) ) goto end;
	
	cout << count << endl;

    a_1 %= primes[0];
    a_2 %= primes[1];

goto endless;

end:
	int min = *min_element( statistics, statistics+base );
	float rataan_s = 0.f;
	for( int i = 0; i < base; ++i ){
		cout << i << " " << statistics[i] << '\n';
		rataan_s += statistics[i];
	}
	cout << flush;
	
	rataan_s -= min * base;
	cout << rataan_s / base << endl;


/*
	for( int i = 0; i < base; ++i ){
		cout << setw( 3 ) << i << setw( 12 ) << statistics[i] << endl;
	}
	*/
}
