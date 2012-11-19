#include <iostream>
#include <algorithm>

using namespace std;

unsigned int fib( unsigned int p ){
	if( p == 0 ) return 0;
	if( p == 1 ) return 1;
	
	return fib( p - 1 ) + fib( p - 2 );
}

unsigned int fib2( unsigned int p ){
	if( p == 0 ) return 0;
	if( p == 1 ) return 1;
	
	int a = 1, b = 1;
	
	for( int i = 1; i < p; i++ ){
		b += a;
		swap( a,b );
	}
	
	return b;
}

int main(){
	
	for( int i = 1; i <= 100; i++ ){
		cout << "RECRUSIVE ANSWER : " << fib( i ) << endl;
		cout << "ITERATIVE ANSEWR : " << fib2( i ) << endl;
	}
	
/*
	cari fungsi fib2
*/	
	cout << endl;
	
	return 0;
}