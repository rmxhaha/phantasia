#include <iostream>

using namespace std;

class suicide {
public:
	suicide( int in ){
		for( int i = 0; i < pool.size(); ++i ){
			if( pool[i]->p == in ){
				delete this;
			}
		}
	}
	
private:
	int p;
	static vector<suicide *> pool;
};


int main(){
	
}