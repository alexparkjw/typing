#include<locale.h>
#include<wchar.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<stddef.h>

#define random_range 11172

void foo1();
void foo2();
void foo3();
void foo4();
void foo5();

int main(void) {
    srand(time(NULL));
    char *locale = setlocale(LC_ALL, "");

    wchar_t k[] = { 0b1000, 0b1, 0b1100, 0b10011, 0b10110, 0b1100,
      0b1000, 0b1100, 0b1010, 0b10001, 0b111, 0b110, 0b11010
    };
    int n=sizeof(k)/sizeof(k[0]);

    for(wchar_t j=0xac00; j<0xd7a4; j++) {
        for(int i=0; i<n; i++) 
            wprintf(L"%lc ", k[i]+j);
        printf("\n");    
    }

    return 0;
}

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
    /* printf("\n%d\n", ++i + i++); */
}

