#include <iostream>

using namespace std;

static int basis[] = { 10000, 120, 10000, 10000, 380, 380 };

class person {
public:

	person( short id, short age, short fatherid, short motherid, short x, short y){
		id 			%= basis[0];
		age 		%= basis[1];
		fatherid	%= basis[2];
		motherid	%= basis[3];

		x 			+= basis[4]/2;
		y 			+= basis[4]/2;
		x			%= basis[4];
		y			%= basis[5];


		cout << id << endl;
		cout << age << endl;
		cout << fatherid << endl;
		cout << motherid << endl;
		cout << x << endl;
		cout << y << endl << endl;

		data_ = y;
		data_ *= basis[4];

		data_ += x;
		data_ *= basis[3];

		data_ += motherid;
		data_ *= basis[2];

		data_ += fatherid;
		data_ *= basis[1];

		data_ += age;
		data_ *= basis[0];

		data_ += id;
	}

	void extract(){
		unsigned long long p = data_;

		for( int i = 0; i < 6; ++i  ){
			cout << p % basis[i] << " " << basis[i] << endl;
			p /= basis[i];
		}

		cout << endl;
	}

	unsigned short getId(){
		return data_ % basis[0];
	}

	unsigned short getAge(){
		static const int limit = 1;
		unsigned long long data = data_;

        int i;

		for( i = 0; i < limit; ++i ){
			data /= basis[i];
		}

		return data % basis[i];
	}

	unsigned short getFatherId(){
		static const int limit = 2;
		unsigned long long data = data_;

        int i;

		for( i = 0; i < limit; ++i ){
			data /= basis[i];
		}

		return data % basis[i];
	}

	unsigned short getMotherId(){
		static const int limit = 3;
		unsigned long long data = data_;

        int i;

		for( i = 0; i < limit; ++i ){
			data /= basis[i];
		}

		return data % basis[i];
	}

	short getLocationX(){
		static const int limit = 4;
		unsigned long long data = data_;

        int i;

		for( i = 0; i < limit; ++i ){
			data /= basis[i];
		}

		return data % basis[i] - basis[i]/2;
	}

	short getLocationY(){
		static const int limit = 5;
		unsigned long long data = data_;

        int i;

		for( i = 0; i < limit; ++i ){
			data /= basis[i];
		}

		return  data % basis[i] - basis[i]/2;
	}

private:
	unsigned long long data_;
};

/*
	anda di perbolehkan maksimal mengunakan 64 bit data alias 8 byte
	gunakan semaksimal mungkin

	fungsi yang di perlukan
	unsigned short getId(); [ 0..9999 ]
	unsigned short getAge(); [ 0..199 ]
	unsigned short getFatherId() [ 0..9999 ]
	unsigned short getMotherId() [ 0..9999 ]
	short getLocationX() [ -190..189 ]
	short getLocationY() [ -190..189 ]

	yang ada di dalam kurung adalah range

*/

int main(){
	if( sizeof( person ) > 8 ){
		cout << "Data Struct person kebesaran !" << endl;
	}


	person * q = new person( 3200, 119, 2000, 5000, 129, 149 );

	cout << q -> getId() << endl;
	cout << q -> getAge() << endl;
	cout << q -> getFatherId() << endl;
	cout << q -> getMotherId() << endl;
	cout << q -> getLocationX() << endl;
	cout << q -> getLocationY() << endl;

	return 0;
}
