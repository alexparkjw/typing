#include<locale.h>
#include<wchar.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<stddef.h>

#define random_range 11172

void foo1() {
    for(wchar_t i=0xac00; i<0xd7a4; i++ ) { 
        if((i+12)%28==0) printf("\n");
        wprintf(L"%lc", i);
    }
    printf("\n");
}

void foo2() {
    for(wchar_t i=0xac00; i<0xd7a4; i+=(21*28)) {
        wprintf(L"%lc", i);
    }
    printf("\n");
}

void foo3() {
    for(wchar_t i=0xc544; i<0xc790; i++ ) { 
        wprintf(L"%lc", i);
    }
    printf("\n");
}

void foo4() {
    
    for(int i=0; i<8; i++ ) { 
        wprintf(L"%lc", rand()%random_range + 0xac00);
    }
    printf("\n");
}


void foo5() {
    for(wchar_t i=0x0; i<0xff; i++ ) { 
        if((i+12)%28==0) printf("\n");
        wprintf(L"%lc", i);
    }
    int i=0;
    printf("\n%d\n", ++i + i++);
}

int main(void) {
    srand(time(NULL));
    char *locale = setlocale(LC_ALL, "");

    foo5();
    return 0;
}

