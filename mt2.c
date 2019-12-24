#include "header_all.h"

#define HEIGHT 40
#define WIDTH 80
#define SIZE_N 4

#define rr(k) rand()%(k)

typedef struct db {
    char *data_str;
    int *data_int;
    pthread_t *tid;
}strcut_db;

typedef struct stack {
    struct db *data;
    int size_datatype;
    int index_data;
    int len_alloc;
}trcut_stack;

char *strDup(const char * str);
int strCmp(void * vp1, void * vp2);
int len_func(FILE *fp);
char **save_func(FILE *fp, int len);

int kbhit(void);
void *rain(void *n);
void *bar(void *n);


// SHARE DATA
pthread_t tids[SIZE_N];
pthread_attr_t attr;
char **data;

int main(void) { 
    // CONVERT DATA FILE to ARRAY
    FILE *fp = fopen("words.txt", "r");
    int n_data = len_func(fp);
    data = save_func(fp, n_data);

    // RANDOMIZE INDEX
    srand((unsigned)time(NULL)*getpid());
    int n_rand[SIZE_N];
    n_rand[0]=SIZE_N;
    for(int i=1; i<SIZE_N; i++)
       n_rand[i] = rr(n_data);


    // INIT NCURSES
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    scrollok(stdscr, true);
    curs_set(FALSE);

    // INIT PTHREAD 
    pthread_attr_init(&attr);
    pthread_create(&tids[0], &attr, bar, n_rand);

    for(int i=1; i<SIZE_N; i++) {
        pthread_attr_init(&attr);
        // FUCTION CALL
        pthread_create(&tids[i], &attr, rain, data[n_rand[i]]);
        sleep(1);
    }
    pthread_join(tids[0], NULL);
    for(int i=1; i<SIZE_N; i++) 
        pthread_join(tids[i], NULL);


    // END
    /* while( (ch=getch()) != '\n') ; */
    endwin();
    exit (0);
}

void *bar(void *n) {
    int *index_data = (int*)n;
    int y=(HEIGHT-6), x=(WIDTH)/2-10;
    int ch,index_temp=0;
    char temp[100];
    while(true) {
        if(kbhit()) {
            ch = getch();
            temp[index_temp] = ch;

            if(ch != '\n' || ch !=' ') {
                temp[+index_temp] = '\0';
                for(int i=1; i<index_data[0]; i++) {
                    if(strcmp(temp, data[index_data[i]])==0) {
                        index_data[0]--;
                        pthread_cancel(tids[i]);
                    }
                }
            }else {
                index_temp++;
            }


        } else {
            curs_set(TRUE);
            mvprintw(y,x++, "%c", ch );    
            refresh();
            curs_set(FALSE);
            sleep(1);
        }
    }
    pthread_exit(0);
}

void *rain(void *w) {
    char *ldata = (char*)w;
    int y=1, x=rr(WIDTH- strlen(ldata)-2); 

    while(y < HEIGHT-8) {
        mvprintw(y++, x, "%s", ldata);
        refresh();
        sleep(1);
    }
    pthread_exit(0);
}

int kbhit(void) {
    int ch = getch();
    if (ch != ERR) {
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
    char buff[100];
    int len = 0;
    while(fgets(buff, sizeof(buff) - 1, fp) != NULL) {
        /* fputs(buff, stdout); */
        len++;
    }
    fseek(fp, 0, SEEK_SET); /* rewind(fp) */
    return len;
}

char **save_func(FILE *fp, int len) {
    fseek(fp, 0, SEEK_SET); /* rewind(fp) */
    char buffer[100], **arr = (char**)malloc(sizeof(char*)*len);
    for (int i = 0, n;i<len; i++) {
        fgets(buffer, sizeof(buffer)-1 , fp);
        buffer[strcspn(buffer, "\n")] = 0;
        /* fprintf(fp, "%s\n", buffer); */
        arr[i] = (char*)malloc(sizeof(buffer));
        strcpy(arr[i], buffer);
        /* arr[i] = strdup(buffer); */
        /* printf("%s\n", arr[i]); */
    }
    return arr;
}
