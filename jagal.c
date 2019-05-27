#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(void) {
    int n, k;
    scanf("%d %d", &n, &k);
    char *input = (char *)malloc(sizeof(char)*(n+1));
    scanf(" %s", input);
    int num = 0;
    int num1 = 0;
    int flag = 0;
    int out;

    if(input[0] == 'W') flag = 1;

    for(int i=0; i<n; i++) {
        if(input[i] == 'W')
            num += (int)pow(2, n-i-1);
    }
   
    for(int i=0; i<k; i++) {
        num1 = num << 1 ;       
        num1 += flag;
        num = num ^ num1;
    } 

    for(int i = 31; i >=0; i--) {
        out = num >> i;
        if(i < n) {
            if ( out & 1) 
                printf("W");
            else
                printf("B");
        }
    }

    printf("\n");

    return 0;
}
