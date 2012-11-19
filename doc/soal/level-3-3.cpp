#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

int main(){

    unsigned int T;
    string p;
    int q;
    map< string, int > array;

    cin >> T;

    while( T-- ){
        cin >> p;
        cin >> q;
        array[p] = q;
    }

    cin >> T;

    while( T-- ){
        cin >> p;
        cout << array[p] << endl;
    }

    return 0;
}
