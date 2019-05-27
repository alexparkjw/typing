#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(void) {
    int n, k;
    scanf("%d %d", &n, &k);
    char *input = (char *)malloc(sizeof(char)*(n+1));
    scanf(" %s", input);

    int num = 0, num1 = 0,  num2 = 0;
    int flag = 0,  out;

    if(input[0] == 'W') flag = 1;

    for(int i=0; i<n; i++) {
        if(input[i] == 'W')
            num += (int)pow(2, n-i-1);
    }
   
    num1 = num;
    for(int i=0; i<k; i++) {
        num2 = num1 << 1 ;       
        num2 += flag;
        num1 = num1 ^ num2;
    } 

    num2 = num1 ^ num;

    printf("%d\n", num2);

    /* for(int i = 31; i >=0; i--) { */
    /*     out = num2 >> i; */
    /*     if(i < n) { */
    /*         if ( out & 1) */ 
    /*             printf("W"); */
    /*         else */
    /*             printf("B"); */
    /*     } */
    /* } */
    /* printf("\n"); */

    return 0;
}
