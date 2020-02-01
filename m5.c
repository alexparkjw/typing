#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R 3
#define C 3

void memCpy(void *dest, void *src, size_t size);
void print(int **arr, int r, int c);
int **gp = NULL;

int main(void) {
    // 배열 
    int arr[R][C] = { {1, 2, 3}, {4, 5, 6}, {7,8,9} };

    // 포인터
    int **dp = (int**)malloc(sizeof(int*)*R);
    for(int i=0; i<R; i++) dp[i] = (int*)malloc(sizeof(int)*C);

    // 배열 -> 포인터
    memCpy(dp, arr, sizeof(int**));

    // 로컬포인터 -> 글로벌포인터
    gp = dp;
    
    //확인용출력
    print(gp, R, C);

    // 메모리 해제
    for(int i=0; i<R; i++)
        free(dp[i]);
    free(dp);


    return EXIT_SUCCESS;
}

void memCpy(void *dest, void *src, size_t size) { 
    char *c_src = (char *)src; 
    char *c_dest = (char *)dest; 
    for (int i=0; i<size; i++) 
        c_dest[i] = c_src[i]; 
} 

void print(int **arr, int r, int c) {
    for(int j=0; j<r; j++) {
        for(int i=0; j<c; i++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}
