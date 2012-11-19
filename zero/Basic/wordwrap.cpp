	string wordwrap( const string& text, int width ){
		vector< string > words = explode( ' ', text );
		vector< bool > newline( words.size() );
		string output;
		width += text_display.getPosition().x;

		for( int index = 0; index < words.size(); ++index ){

			output.clear();

			for( int i = 0; i <= index; i++ ){
				output += words[i] + ( newline[ i ] ? '\n' : ' ' );
			}

			text_display.setString( output );

			if( text_display.findCharacterPos( output.size() ).x >= width ){
				newline[ index - 1 ] = true;
			}
		}

		output.clear();

		for( int i = 0; i < words.size(); i++ ){
			output += words[i] + ( newline[ i ] ? '\n' : ' ' );
		}

		return output;
	}