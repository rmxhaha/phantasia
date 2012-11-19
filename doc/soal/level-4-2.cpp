#include <iostream>

using namespace std;

void print( unsigned int q ){
    if( q == 0 ) return;

    print( q - 1 );

    for( int i = 0; i < q; ++i ) cout << "*";
    cout << endl;

    print( q - 1 );

}

int main(){
    int p;
    cin >> p;
    print( p );

    return 0;
}
