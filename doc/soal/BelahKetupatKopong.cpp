#include <iostream>

using namespace std;

int main(){
    unsigned int p;
    cin >> p;

    if( p == 0 ){
        return 0;
    } else if( p == 1 ){
        cout << "^";
    } else {
        int limit = (p - 1)*2 + 1;

        for( int i = 0; i < limit/2; ++i) cout << " ";
        cout << "*" << endl;

        for( int i = p-2; i >= 1; i-- ){
            for( int j = 0; j < i; ++j ){
                cout << " ";
            }

            cout << "*";

            for( int j = i+1; j < limit-i-1; ++j ){
                cout << " ";
            }

            cout << "*";

            cout << endl;
        }

        for( int i = 0; i < p-1; i++ ){
            for( int j = 0; j < i; ++j ){
                cout << " ";
            }

            cout << "*";

            for( int j = i+1; j < limit-i-1; ++j ){
                cout << " ";
            }

            cout << "*";

            cout << endl;
        }

        for( int i = 0; i < limit/2; ++i) cout << " ";
        cout << "*" << endl;


        cout << endl;
    }
}
