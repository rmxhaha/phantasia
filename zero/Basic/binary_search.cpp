// binary_search example
#include <iostream>
#include <algorithm>
#include <vector>

#include <cstdlib>
#include <ctime>

using namespace std;

bool myfunction (int i,int j) { return (i<j); }

int main () {
    srand( time(0));
    vector<int> v;
    v.reserve( 10000000 );
    for( int i = 0; i < 10000000; ++i ) v.push_back( rand() % 40 );

    stable_sort (v.begin(), v.end());
    cout << "R" << endl;
    cout << ( find(v.begin(),v.end(),4) - v.begin() ) << endl;

    // using default comparison:
    cout << ( lower_bound( v.begin(), v.end(), 4 ) - v.begin() );

    return 0;
}
