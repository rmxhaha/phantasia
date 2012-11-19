#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main(){
    string input;
    cout << "Enter a string :";
    cin >> input;

    sort( input.begin(), input.end() );
    cout << input << endl;
    cin.get();
    return 0;
}
