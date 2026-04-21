#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R 3
#define C 3

void print1(int r, int c, void *vp);
void print2(int r, int c, void *vp);
int **gp = NULL;

int main(void) {
    // 배열 
    int arr[R][C] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    printf("배열은 -> print1()\n");
    print2(R, C, arr);

    // 포인터
    int **dp = (int**)malloc(sizeof(int*)*R);
    for(int i=0; i<R; i++) 
        dp[i] = (int*)malloc(sizeof(int)*C);

    // 배열 -> 포인터
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++) {
            dp[i][j] = arr[i][j];
        }
    }

    // 로컬포인터 -> 글로벌포인터
    gp = dp;
    
    //확인용 출력1
    printf("for()문 복사는 -> print1()\n");
    print1(R, C, dp);
    print1(R, C, gp);

    // 배열 -> 포인터 맴카피로....하면
    memcpy(dp, arr, sizeof(int**)*R*C);

    // 확인용 출력 2
    printf("memcopy() 복사는 -> print2()\n");
    print2(R, C, dp);
    print2(R, C, gp);

    // 메모리 해제
    for(int i=0; i<R; i++)  {
        if(dp[i] != NULL)
            ;
    }
    if(dp != NULL)
        free(dp);

    return EXIT_SUCCESS;
}

void print1(int r, int c, void *vp) {
    int **a = vp; 
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) 
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}


void print2(int r, int c, void *vp) {
    int (*a)[c] = vp; 
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) 
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}












