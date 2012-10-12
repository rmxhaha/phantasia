#include <iostream>
#include <vector>

using namespace std;

//pass by reference
void parent(int& aaa){
	aaa = 7;
}

//pass by reference
void parent_2( int * aaa ) {
	*aaa = 5;
}

//parent sama parent_2 dalam case ini sama melakukan hal yang sama
//tapi kalo parent ngepass langsung nama variable nya dan langsung diambil memory address nya
//dan kalo parent_2 ngepass memory address nya



void parent_3( int * bbb ){
	for( int i = 0; i < 5; i++ ){
		bbb[i] = i;
	}
}

void parent_4( vector<int>& p ){
	for( int i = 0; i < p.size(); i++ ){
		p[i] = i;
	}
}




int main(){
	int a = 3;
	cout << a << endl;

	parent( a ); //isi a jadi 7

	cout << a << endl;

	parent_2( &a ); //isi a jadi 5

	cout << a << endl;


	int * b = new int[5];
	//isi b acak-acakan

	for( int i = 0; i < 5; i++ ) cout << *(b+i) << endl;
	cout << endl << endl;

	parent_3( b );
	//isi b jadi 0,1,2,3,4
	for( int i = 0; i < 5; i++ ) cout << *(b+i) << endl;
	cout << endl << endl;
	
	//emg bener lu bsa ngepass array pake pointer tapi meningan ngepass vector by reference kyk di parent_4
	vector<int> ppp;
	ppp.resize( 5 );
	
	//isi ppp 0,0,0,0,0
	for( int i = 0; i < 5; i++ ) cout << ppp[i] << endl;
	cout << endl << endl;
	
	parent_4( ppp );
	for( int i = 0; i < 5; i++ ) cout << ppp[i] << endl;
	cout << endl << endl;

	//isi ppp jadi 0,1,2,3,4




}
