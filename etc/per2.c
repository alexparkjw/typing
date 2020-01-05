#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
 
void swap(void *vp1, void *vp2, int size);
void print_array(int a[], int s);
void print_2d_array(int **a, int r, int c);
int F_nCr(int n, int r);
int F_nPr(int n, int r);
void permute(int *a, int l, int r);
void combi(int *a, int *tmp, int start, int end,int index,int r);

int **list_c, **list_p, index_c, index_p;

#define Max 15

int main(void) {
    // given array of list
    int C[Max];
    for(int i=0; i<Max; i++) 
       C[i] = i+1;
    int n = sizeof(C)/sizeof(C[0]); 
    int r = 6;  // pick 4 of 5

    // calulate pumutation & combinatoin 
    int nPr = F_nPr(n, r);
    int nCr = F_nCr(n, r);
    // inside nCr n2=r, r2=r
    int nPr_2 = F_nPr(r, r);


    // first list for combination
    int *tmp=(int*)malloc(sizeof(int*)*r);
    list_c=(int**)malloc(sizeof(int*)*nCr);
    for(int i=0, s=nCr; i<s; i++) 
        list_c[i]=(int*)malloc(sizeof(int)*r);


    // second list for permutation of combination
    combi(C, tmp, 0, n-1, 0, r);    //nCr,
    list_p=(int**)malloc(sizeof(int*)*nCr*nPr_2);
        for(int i=0, s=nCr*nPr_2; i<s; i++) 
            list_p[i]=(int*)malloc(sizeof(int)*r);
     
    for(int i=0, s=nCr; i<s; i++) {
        permute(list_c[i], 0, r);              //nPr,
    }

    // 5
    /* char *str[nCr*nPr_2]; */
    /* int number[nCr*nPr_2]; */
    /* int count = 0; */
    /* for(int i=0, s=nCr*nPr_2; i<s; i++) { */
    /*     str[i]=(char*)malloc(sizeof(char)*50); */
    /*     for(int j=0; j<r; j++) */ 
            /* sprintf(str[i]+j, "%d", list_p[i][j]); */
        /* printf("%d : %s\n", i+1, str[i]); */
        /* number[i]=atoi(str[i]); */
        /* count++; */
    /* } */

    // result
    /* printf("count : "); */
    /* for(int i=0; i<r; i++) */
        /* printf("%d*", n-i); */
    /* printf("\b = %d\n", count); */


    // free heap
    free(tmp);
    for(int i=0, s=nCr; i<s; i++) 
        free(list_c[i]);
    free(list_c);
    /* for(int i=0, s=nCr*nPr_2; i<s; i++) */ 
    /*     free(list_p[i]); */
    free(list_p);
    return 0;
}

int F_nCr(int n, int r) {
   if(r>n) {
      printf("FATAL ERROR\n"); 
      return 0;
     }       
   if(n==0 || r==0 || n==r) {
      return 1;
   } else {
      return (int)lround( ((double)n/(double)(n-r)/(double)r) * exp(lgamma(n) - lgamma(n-r) - lgamma(r)));
   }
}


int F_nPr(int n, int r) {
   if(r>n) {
       printf("FATAL ERROR\n"); 
       return 0;
   }
   if(n==0 || r==0) {
      return 1;
   } else {
      if (n==r) {
         r = n - 1;
      }
      return (int)lround( ((double)n/(double)(n-r)) * exp(lgamma(n) - lgamma(n-r)));
   }
}

long int fact(int x)  {
    int i, f=1;
    for(i=2; i<=x; i++)
        f=f*i;
    return f;
}

void permute(int *a, int l, int r) {
    if (l == r) {
        print_array(a, r); // display
        /* memcpy(list_p[index_p++], a, sizeof(int)*r); */
        return;
    } 
    for (int i = l; i < r; i++) {
        swap((a+l), (a+i), sizeof(int));
        permute(a, l+1, r);
        swap((a+l), (a+i), sizeof(int)); 
    }
}

void combi(int *a, int *dis, int start, int end,int index,int r) {
    if (index == r) {
        /* print_array(dis, r); // display */
        memcpy(list_c[index_c++], dis, sizeof(int)*r);
        return;
    }
    for (int i=start; i<=end && end-i+1 >= r-index; i++) {
        dis[index] = a[i];
        combi(a, dis, i+1, end, index+1, r);
    }
}

void print_array(int a[], int s) {
    for (int i=0; i<s; i++) {
        /* fprintf(list, "%d ", a[i]); */ 
        printf("%d ", a[i]); 
    }
    printf("\n");
}

void print_2d_array(int **a, int r, int c) {
    /* int rg=1, size=r*c; */
    /* while(size) { */
    /*     size/=10; */
    /*     rg++; */
    /* } */
    for(int i=0; i<r; i++) {
        for(int j=0; j<c; j++) {
            /* printf("%*d ", rg, a[i][j]); */
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void swap(void *vp1, void *vp2, int size) {
    char buffer[size];
    memcpy(buffer, vp1, size);
    memcpy(vp1, vp2, size);
    memcpy(vp2, buffer, size);
}


