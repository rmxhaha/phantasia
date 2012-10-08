#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void break_up( vector<string>& data, string input ){
	string temp;

	temp.clear();

	for( int i = 0; i < input.length(); ++i ){
		temp += input[i];
		if( input[i] == ' ' ){
			data.push_back( temp );
			temp.clear();
			cout << "R";
		}
	}

}


int main( int argc, char * argv[] ){
	string input;
	cin >> input;

	vector<string> p;

	break_up( p, input );
	
	cout << p.size() << endl;
	
	for( int i = 0; i < p.size(); ++i ){
		cout << p[i] << endl;
	}
	
	return 0;
}

