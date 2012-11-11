#include <iostream>

using namespace std;

int i = 1;

void limit(){
//	cout << i++ << endl;
    if( i == 65070 ) return;
    i++;
	limit();
}

int main(){
//	limit();
//  for( int j = 0; j < 65070; j++);

	return 0;
}
