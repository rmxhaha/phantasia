#include <iostream>

using namespace std;

int convert( const string& pp, int index ){
    if( index >= 0 ){
        return pp[index] + convert( pp, index-1 ) * 2;
    }else{
        return 0;
    }
}

int main(){
    string p = "00001010";
    cout << convert( p, 7 ) << endl;

}
