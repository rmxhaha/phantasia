#include <iostream>
#include <vector>

using namespace std;

vector< void* > pool;

struct w {
    w() {}
	int x,y,z;
};

int main(){
	pool.resize( 100 );
	int * p = new int;
	w * lol = new w;

	*p = 5;

	pool[0] = p;
	pool[1] = new w;

	static_cast<w*>(pool[1]) -> x = 500;
	static_cast<w*>(pool[1]) -> y = 502;
	static_cast<w*>(pool[1]) -> z = 503;

	cout << *static_cast<int*> (pool[0]) << endl;
	cout << static_cast<w*> (pool[1]) -> y << endl;

	return 0;
}
