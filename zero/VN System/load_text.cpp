#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct _Log{
	_Log(){};
	string name;
	string text;
	string char_image_url;
};

void clean_text( string& input ){
	int check_index = 0;
	int write_index = 0;
	//clean of the first spaces
	while( check_index < input.length() && input[check_index] == ' ' ) check_index++;

	//Clean double or more spaces together
	while( check_index < input.length() ){
		if( input[check_index] == ' ' ){
			while( input[check_index] == ' ' && check_index < input.length() ) check_index++;
			check_index--;
		}

		input[ write_index ] = input[ check_index ];

		write_index++;
		check_index++;
	}

	//Clean the last spaces
	if( input[write_index-1] == ' ' ){
		input.resize( write_index-1 );
	}else{
		input.resize( write_index );
	}
}

void load_text_file( vector<_Log>& logs, string filename ){
	logs.clear();

	ifstream textfile;
	string line;
	textfile.open( filename.c_str() );

	int linenum = 0;
	while( !textfile.eof() )
	{
		logs.push_back( _Log() );

		getline( textfile, line );
		int i = 0;
		for( ; i < line.length() && line[i] != ':'; ++i ){
			logs[linenum].char_image_url += line[i];
		}

		i++;

		for( ; i < line.length() && line[i] != ':' ; ++i ){
			logs[linenum].name += line[i];
		}

		i++;


		for( ; i < line.length(); ++i ){
			logs[linenum].text += line[i];
		}

		clean_text( logs[linenum].name );
		clean_text( logs[linenum].text );
		clean_text( logs[linenum].char_image_url );

		linenum++;
	}

	textfile.close();
}

int main(){

	vector<_Log> logs;
	load_text_file( logs, "Text.txt" );
    for( int i = 0; i < logs.size(); ++ i) cout << logs[i].name << " : " << logs[i].text << endl;


}
