#include <iostream>
#include <SFML/System.hpp>

using namespace std;

const int NUM = 5;

int tasks[ NUM ];

void threadFunc( int arg ){
    while( tasks[arg] != -1 ){
        if( tasks[arg] != 0 ){
            cout << tasks[arg] << endl;
            tasks[arg] = 0;
        }
    }
}

int main(){
    sf::Thread thread(&threadFunc, 1);
    thread.launch();
    int i = 1;
    while( i <= 5 ){
        if( tasks[1] == 0 ){
             tasks[1] = i;
             i++;
        }
//        cout << i;
    }
    cout << "R";

    return 0;
}
