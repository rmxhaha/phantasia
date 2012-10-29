#include <iostream>
#include <vector>

using namespace std;
int p = 200;

struct a {
	a( int lol ){
		p = lol;
	}

	int p;
};

vector<a> qwe;

int& get(){
	return p;
}

int main(){
	qwe.push_back( a(2000) );
	cout << qwe.back().p;

	cout << p << endl;

	int & x = get();
	int * y = &get();

	p = 50;
	cout << *y << endl;


	cout << p << endl;



	return 0;
}
