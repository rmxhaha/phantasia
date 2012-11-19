#include <stdio.h>
#include <conio.h>

int main(){
    int p;
    int q;
    scanf("%i",&p);
    scanf("%i",&q);
    if( p <= 0 ){
        return 0;
    } else if( p == 1 ){
        printf("*");
    } else {
        int limit = (p - 1)*2 + 1;

        for( int i = 0; i < limit/2; ++i) printf(" ");
        printf("*\n");

        for( int i = p-2; i >= 1; i-- ){
            for( int j = 0; j < i; ++j ){
                printf(" ");
            }

            printf("*");

            for( int j = 1; j < limit-i*2-1; ++j ){
                printf( j%2 == 0 && ( j < q*2 || limit-i*2-1-j < q*2 ) ? "*" : " " );
            }

            printf("*\n");
        }

        for( int i = 0; i < p-1; i++ ){
            for( int j = 0; j < i; ++j ){
                printf(" ");
            }

            printf("*");

            for( int j = 1; j < limit-i*2-1; ++j ){
                printf( j%2 == 0 && ( j < q*2 || limit-i*2-1-j < q*2 ) ? "*" : " " );
            }

            printf("*\n");
        }

        for( int i = 0; i < limit/2; ++i) printf(" ");
		printf("*\n");

    }

    getch();
}

