#include <iostream>
#include <vector>
#include <string>

using namespace std;

string tolower( string input ){
	for( int i = input.length(); i--; ){
		switch( input[ i ] ){
			case 'A': input[i] = 'a'; break;
			case 'B': input[i] = 'b'; break;
			case 'C': input[i] = 'c'; break;
			case 'D': input[i] = 'd'; break;
			case 'E': input[i] = 'e'; break;
			case 'F': input[i] = 'f'; break;
			case 'G': input[i] = 'g'; break;
			case 'H': input[i] = 'h'; break;
			case 'I': input[i] = 'i'; break;
			case 'J': input[i] = 'j'; break;
			case 'K': input[i] = 'k'; break;
			case 'L': input[i] = 'l'; break;
			case 'M': input[i] = 'm'; break;
			case 'N': input[i] = 'n'; break;
			case 'O': input[i] = 'o'; break;
			case 'P': input[i] = 'p'; break;
			case 'Q': input[i] = 'q'; break;
			case 'R': input[i] = 'r'; break;
			case 'S': input[i] = 's'; break;
			case 'T': input[i] = 't'; break;
			case 'U': input[i] = 'u'; break;
			case 'V': input[i] = 'v'; break;
			case 'W': input[i] = 'w'; break;
			case 'X': input[i] = 'x'; break;
			case 'Y': input[i] = 'y'; break;
			case 'Z': input[i] = 'z'; break;
		}
	}

	return input;
}

char char_width[256];
const int log_width = 300;

class _Log {

public:
	_Log(){}

	string name;
	string text;
	string char_image_url;

	void clean();
private:

	void clean_name();
	void clean_log_text();
	void clean_char_image_url();
};

void _Log::clean(){
	clean_name();
	clean_log_text();
	clean_char_image_url();
}

void _Log::clean_name(){
	int check_index = 0;
	int write_index = 0;

	//search for the first characters which is not a space
	check_index = name.find_first_not_of(' ');

	//there is no need to clean if there is not a single space in the sentence
	if( check_index == -1 ) return;

	//Clean double or more spaces together
	while( check_index < name.length() ) {
		if( name[check_index] == ' ' ) {
			while( name[check_index] == ' ' && check_index < name.length() ) check_index++;
			check_index--;
		}

		if( write_index != check_index ) name[ write_index ] = name[ check_index ];

		write_index++;
		check_index++;
	}

	//Clean the last spaces
	if( name[write_index-1] == ' ' ) {
		name.resize( write_index-1 );
	} else {
		name.resize( write_index );
	}

	if( tolower( name ) == "narator" ) name = tolower( name );
}

void _Log::clean_log_text(){
	int check_index = 0;
	int write_index = 0;

	if( name != "narator" ) { //name has already been cleaned

		//search for the first characters which is not a space
		check_index = text.find_first_not_of(' ');

		if( check_index == -1 ) {
			//there is no need to clean if there is not a single space in the sentence
			text.insert( text.begin(), '\"' );
			text.insert( text.end(), '\"' );
			goto cleaned;
		} else if( check_index == 0 ){
			text.insert( text.begin(), '\"' );
		} else {
			text[ write_index ] = '\"';
			write_index++;
		}

		//Clean double or more spaces together
		while( check_index < text.length() ){
			if( text[check_index] == ' ' ){
				while( text[check_index] == ' ' && check_index < text.length() ) check_index++;
				check_index--;
			}
						
			if( write_index != check_index ) text[ write_index ] = text[ check_index ];

			write_index++;
			check_index++;
		}

		//add quotation mark add the end of the sentence
		if( write_index == text.length() ){
			text.insert( text.end(), '\"' );
		}else{
			if( text[ write_index - 1 ] == ' ' ) {
				text[ write_index - 1 ] = '\"';
				text.resize( write_index );
			} else {
				text[ write_index ] = '\"';
				text.resize( write_index + 1 );
			}
		}
	} else {
		//search for the first characters which is not a space
		check_index = text.find_first_not_of(' ');

		//there is no need to clean if there is not a single space in the sentence
		if( check_index == -1 ) goto cleaned;
		//Clean doublxe or more spaces together
		while( check_index < text.length() ) {
			if( text[check_index] == ' ' ) {
				while( text[check_index] == ' ' && check_index < text.length() ) check_index++;
				check_index--;
			}

			if( write_index != check_index ) text[ write_index ] = text[ check_index ];

			write_index++;
			check_index++;
		}

		//Clean the last spaces
		if( text[write_index-1] == ' ' ) {
			text.resize( write_index-1 );
		} else {
			text.resize( write_index );
		}
	}


	cleaned:
	
	//word wrapping
	int i, temp = 0, temp2, temp_i;
	for( i = 0; i < text.length(); ++i ){
		temp_i = i;
		temp2 = 0;
		for( ; text[i] != ' ' && i < text.length() ; ++i )temp2 += char_width[ text[i] ];

		if( temp + temp2 >= log_width ){
			text.insert( text.begin() + temp_i, '\n' );
			temp = temp2;
		} else {
			temp += temp2;
		}
	}

	return;
}

void _Log::clean_char_image_url(){
	string& input = this -> char_image_url;
	//remove all spaces

	int check_index = input.find_first_not_of(' ');
	int write_index = 0;

	while( check_index < input.length() ){
		if( input[check_index] == '\\' ){
			check_index = input.find_first_not_of( '\\', check_index );
			if( check_index != -1 ){
				check_index--;
			} else {
				check_index = input.find_last_of( '\\' );
			}
		}

		input[write_index] = input[check_index];
		write_index++;
		check_index++;
	}

	input.resize( write_index );
}


int main(){

	//temporary fill
	for( int i = 256; i--; ) char_width[i] = 10;

	_Log test;

	test.name = "  Creator    ";
	test.text = "  Agent K You  are  on a mission now abcd ";
	test.char_image_url = " 1.jpg";

//	cin >> test.name >> test.text >> test.char_image_url;

	test.clean();

	cout << test.name << endl;
	cout << test.text << endl;
	cout << test.char_image_url << endl;


	return 0;
}
