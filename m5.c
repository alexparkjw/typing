#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R 3
#define C 3

void memCpy(void *dest, void *src, size_t size);
void print(int **a, int r, int c);
int **gp = NULL;

int main(void) {
    // 배열 
    int arr[R][C] = { {1, 2, 3}, {4, 5, 6}, {7,8,9} };
    print(arr, R, C);

    // 포인터
    int **dp = (int**)malloc(sizeof(int*)*R);
    for(int i=0; i<R; i++) dp[i] = (int*)malloc(sizeof(int)*C);

    // 배열 -> 포인터
    memcpy(dp, arr, sizeof(int**));

    // 로컬포인터 -> 글로벌포인터
    gp = dp;
    
    //확인용출력
    print(arr, R, C);
    print(dp, R, C);
    print(gp, R, C);

    // 메모리 해제
    for(int i=0; i<R; i++)
        free(dp[i]);
    free(dp);

    return EXIT_SUCCESS;
}

void print(int **a, int r, int c) {
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) 
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

