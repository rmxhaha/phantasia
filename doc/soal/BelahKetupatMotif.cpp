#include <iostream>

using namespace std;

int main(){
    unsigned int p;
    unsigned int q;
    cin >> p;
    cin >> q;

    if( p == 0 ){
        return 0;
    } else if( p == 1 ){
        cout << "*";
    } else {
        int limit = (p - 1)*2 + 1;

        for( int i = 0; i < limit/2; ++i) cout << " ";
        cout << "*" << endl;

        for( int i = p-2; i >= 1; i-- ){
            for( int j = 0; j < i; ++j ){
                cout << " ";
            }

            cout << "*";

            for( int j = 1; j < limit-i*2-1; ++j ){
                cout << ( j%2 == 0 && ( j < q*2 || limit-i*2-1-j < q*2 ) ? "*" : " " );
            }

            cout << "*";

            cout << endl;
        }

        for( int i = 0; i < p-1; i++ ){
            for( int j = 0; j < i; ++j ){
                cout << " ";
            }

            cout << "*";

            for( int j = 1; j < limit-i*2-1; ++j ){
                cout << ( j%2 == 0 && ( j < q*2 || limit-i*2-1-j < q*2 ) ? "*" : " " );
            }

            cout << "*";

            cout << endl;
        }

        for( int i = 0; i < limit/2; ++i) cout << " ";
        cout << "*" << endl;


        cout << endl;
    }
}

