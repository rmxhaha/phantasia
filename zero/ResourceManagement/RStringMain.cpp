#include "RString.cpp"
#include <iostream>

using namespace std;

int main(){
	cout << tolower( "AbCdEf" ) << endl;

	vector<string> array;
	array = explode( ' ', "A Quick Brown Fox Jumps over the lazy dog" );

	cout << "Number of words : " << array.size() << endl;

	for( int i = 0; i < array.size(); ++i ){
		cout << array[i] << " ";
	}

	cout << endl;
}
