#include <stdio.h>
#include <ncurses.h>
#include "header.h"
#include "sem.h"

typedef struct Vector{
    int a[6]; // int x, y, h, w, o, len;
    char **data;
    WINDOW *win;
    void (*set)(struct Vector*, int*, char**);
}vector;

#define WIDTH 90
#define HEIGHT 45
#define SIZE_N 21
#define endl printf("\n")
#define rr(k) rand()%(k)
#define vector(p) struct Vector (*p)=(struct Vector *)malloc(sizeof(struct Vector));\
                 (p)->set=set;


void set(vector *self, int *a, char **data);
int kbhit(void);
char *strDup(const char * str);
int strCmp(void * vp1, void * vp2);
int len_func(FILE *fp);
char **save_func(FILE *fp, int len);

// PTHREAD and SEMAPHORE
/* sem_t mutex; */
/* pthread_mutex_t mutex; */
struct rk_sema s;
pthread_t tids[SIZE_N];
pthread_attr_t attr;


// SHARE DATA
volatile int count_down;
volatile int count_up;

char **data_menu = NULL;
char **data_basic = NULL;
char **data_words = NULL;
char **data_sentence = NULL;
char **data_paragraph = NULL;
char **data_option = NULL;

int main(void) {
    // CONVERT DATA FILE to ARRAY


    return EXIT_SUCCESS;
}

int kbhit(void)   {
    int ch = getch();
    if( ch != ERR) {     // no input
        ungetch(ch);
        return 1;
    }
    return 0;
}

char *strDup(const char * str){
    int n = strlen(str) + 1;
    char * dup = (char *)malloc(sizeof(char)*n);
    if (dup)
        strcpy(dup, str);
    return dup;
}

int strCmp(void * vp1, void * vp2) {
    char * s1 = (char *)vp1;
    char * s2 = (char *)vp2;
    return strcmp(s1, s2);
}

int len_func(FILE *fp) {
    fseek(fp, 0, SEEK_SET); /* rewind(fp) */
    int len = 0;
    char buff[256];
    while(fgets(buff, sizeof(buff) - 1, fp) != NULL) { len++; }
    return len;
}

char **save_func(FILE *fp, int len) {
    fseek(fp, 0, SEEK_SET); /* rewind(fp) */
    char buffer[256];
    char  **arr = (char**)malloc(sizeof(char*)*len);
    for (int i = 0; i<len; i++) {
        fgets(buffer, sizeof(buffer)-1 , fp);
        buffer[strcspn(buffer, "\n")] = 0;
        arr[i] = (char*)malloc(sizeof(buffer)+1);
        strcpy(arr[i], buffer);

        /* char *buffer=(char*)malloc(sizeof(char)*256); */
        /* fscanf(fp, " %[^\n]", buffer); */
        /* arr[i] = strdup(buffer); */
    }
    return arr;
}

