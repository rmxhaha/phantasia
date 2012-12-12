#include <iostream>
#include <string.h>
#include <algorithm>

using namespace std;

struct p_t{
    int a;
    short b;
    short c;
};

int main(){
    p_t * testcase = new p_t();
    p_t * testcase_2 = new p_t();

    testcase -> a = 50;
    testcase -> b = 300;
    testcase -> c = 700;

    char * bits = new char[ sizeof(p_t)/sizeof(char) ];


    memcpy( bits, testcase, sizeof(p_t) );
    memcpy( testcase_2, bits, sizeof(p_t) );

    for( int i = 0; i < sizeof(p_t)/sizeof(char); i++ ) cout << (int) bits[i] << " ";

    cout << endl;


    cout << testcase_2 -> a << " " << testcase_2 -> b << " " << testcase_2 -> c << endl;
    cout << testcase -> a << " " << testcase -> b << " " << testcase -> c << endl;

    delete testcase;
    delete testcase_2;

    return 0;
}
