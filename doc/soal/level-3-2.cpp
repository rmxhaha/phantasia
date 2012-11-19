#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool check( char a, char b ){
	return ( a > b );
}

int main(){
    string input;
    cout << "Enter a string :";
    cin >> input;

    sort( input.begin(), input.end(), check );
    cout << input << endl;
    cin.get();
    return 0;
}
