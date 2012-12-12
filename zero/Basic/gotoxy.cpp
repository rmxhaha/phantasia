#include <windows.h>
#include <cstdio>
#include <ctime>
void gotoxy( short x, short y )
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ;
    COORD position = { x, y } ;

    SetConsoleCursorPosition( hStdout, position ) ;
}

int main(){
    for( int i = 0; i < 1000; ++i ){

        gotoxy( i, i );
        printf("#");

        Sleep( 100 );

        gotoxy( i, i );
        printf(" ");

    }

    return 0;
}
