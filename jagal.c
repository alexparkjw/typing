#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int rotate(int num_a, int n, int k) {
    int num_c = num_a, num_b, flag;
    for(int i=0; i<k; i++) {
        flag = num_c & (int)pow(2, n-1);
        num_b = num_c << 1; 
        num_b += flag;
        num_c = num_c ^ num_b;
    }
    return num_c;
}


int main(void) {
    int n, k;
    scanf("%d %d", &n, &k);
    char *input = (char *)malloc(sizeof(char)*(n+1));
    scanf(" %s", input);

    // convert input to num;
    int num;
    for(int i=0; i<n; i++) {
        if(input[i] == 'W')
            num += (int)pow(2, n-i-1);
    }
   
    // counting
    int count = 1;
    for(int i=0, size=(int)pow(2, n-1); i<size; i++) {
        if(rotate(i, n, k) == rotate(num, n, k)) 
            count++;
    }
    printf("%d\n", count);

    /* for(int i = 31; i >=0; i--) { */
    /*     out = num_c >> i; */
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
