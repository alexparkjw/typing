#include<locale.h>
#include<wchar.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
/* #include<stddef.h> */

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

int main(void) {
    srand(time(NULL));
    char *locale = setlocale(LC_ALL, "");

    int k = 0x2660;
    for(int j=0; j<30; j++) {
        for(int i=0; i<12; i++) 
            wprintf(L"%lc", k++); 
        printf("\n");
    }



    return 0;
}

