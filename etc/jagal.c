#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rotate(int n, int k, char *str) {
    char cp[n+1];
    strcpy(cp, str);
    for(int i=0; i<k; i++) {
        char tmp[2];
        tmp[0] = cp[0];
        tmp[1] = '\0';
        strcpy(cp, str+1);
        strcat(cp, tmp);
        strcpy(str, cp);
    }
}

int main(void) {
    int n, k;
    /* scanf("%d %d", &n, &k); */
    n = 3;
    k = 2;
    char input[n+1];
    /* scanf(" %s", input); */
    strcpy(input, "ABC");

    printf("%s\n", input);

    rotate(n, k, input);
    printf("%s\n", input);

    

    return 0;
}
