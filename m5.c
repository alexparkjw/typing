#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R 3
#define C 3

void print(int r, int c, int a[r][c]);
int **gp = NULL;

int main(void) {
    // 배열 
    int arr[R][C] = { {1, 2, 3}, {4, 5, 6}, {7,8,9} };

    // 포인터
    int **dp = (int**)malloc(sizeof(int*)*R);
    for(int i=0; i<R; i++) dp[i] = (int*)malloc(sizeof(int)*C);

    // 배열 -> 포인터
    memcpy(dp, arr, sizeof(int**)*R*C);

    // 로컬포인터 -> 글로벌포인터
    gp = dp;
    
    //확인용출력
    print(R, C, arr);
    print(R, C, dp);
    print(R, C, gp);

    // 메모리 해제
    for(int i=0; i<R; i++)
        free(dp[i]);
    free(dp);

    return EXIT_SUCCESS;
}

void print(int r, int c, int a[r][c]) {
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) 
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

