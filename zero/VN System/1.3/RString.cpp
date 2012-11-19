#ifndef RSTRING_CPP
#define RSTRING_CPP

#include <cstdlib>
char tolower( char input ) {
	switch( input ){
		case 'A': input = 'a'; break;
		case 'B': input = 'b'; break;
		case 'C': input = 'c'; break;
		case 'D': input = 'd'; break;
		case 'E': input = 'e'; break;
		case 'F': input = 'f'; break;
		case 'G': input = 'g'; break;
		case 'H': input = 'h'; break;
		case 'I': input = 'i'; break;
		case 'J': input = 'j'; break;
		case 'K': input = 'k'; break;
		case 'L': input = 'l'; break;
		case 'M': input = 'm'; break;
		case 'N': input = 'n'; break;
		case 'O': input = 'o'; break;
		case 'P': input = 'p'; break;
		case 'Q': input = 'q'; break;
		case 'R': input = 'r'; break;
		case 'S': input = 's'; break;
		case 'T': input = 't'; break;
		case 'U': input = 'u'; break;
		case 'V': input = 'v'; break;
		case 'W': input = 'w'; break;
		case 'X': input = 'x'; break;
		case 'Y': input = 'y'; break;
		case 'Z': input = 'z'; break;
	}

	return input;
}

string tolower( const string& input ){
	string output;
	output.resize( input.size() );
	
	for( int i = input.length(); i--; ){
		output[i] = tolower( input[i] );
	}

	return output;
}

int parseInt( const string& input ){
	return atoi( input.c_str() );
}

vector < string > explode( char sep, const string& s ) {
	vector < string > f;
	string temp;

	for (int i=0; i<s.size(); i++)
	{
			if ( s[i] == sep )
			{
					if ( temp != " " || temp != "" ) f.push_back(temp);
					temp = "";
			} else temp += s[i];
	}
	if ( temp != " " || temp != "" ) f.push_back(temp);

	return f;
}

#endif
