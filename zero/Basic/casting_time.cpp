#include <cstdio>

struct gak_jelas {
    int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;
};

int main(){
    void * p = new gak_jelas;
//    *(int*)p = 5;
/*    for(
        ((gak_jelas*)p) -> x = 0;
        ((gak_jelas*)p) -> x < 100000000;
        ++ (((gak_jelas*)p) -> x) );
*/
    for(
        static_cast<gak_jelas*>(p) -> x = 0;
        static_cast<gak_jelas*>(p) -> x < 100000000;
        ++(static_cast<gak_jelas*>(p))->x
        );

    //conclusion :  static_cast slightly faster ( about 3 x 10^-7 ms ) than cstyle cast but
    //              it's better to use static_cast
    //              not because of it's speed but because of the number of parenthesis

}

