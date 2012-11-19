#include <iostream>

using namespace std;

int main(){
	int p;
	int * array;

	cin >> p;

	array = new int[p];

	for( int i = 0; i < p; i++ ){
		cin >> array[i];
	}

	for( int i = 0; i < p; i++ ){
		cout << array[i]*2 << " ";
	}

	cout << endl;
	cin.get();
}
