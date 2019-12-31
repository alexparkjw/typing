/* #include <stdio.h> */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <curses.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

#define WIDTH 90
#define HEIGHT 50
#define SIZE_N 10
#define endl printf("\n")
#define rr(k) rand()%(k)
#define class(p) struct Class (p);\
                 (p).set=set;\
                 (p).put=put

#define class2(p) struct Class (*p)=(struct Class *)malloc(sizeof(struct Class));\
                 (p)->set=set;\
                 (p)->put=put

typedef struct Class {
	int x;
	int y;
	void (*set)(struct Class*, int, int);
    void (*put)(struct Class*);
}class;

void set(class *self, int w, int h );
void put(class *self);
int kbhit(void);
char *strDup(const char * str);
int strCmp(void * vp1, void * vp2);
int len_func(FILE *fp);
char **save_func(FILE *fp, int len);

void *bar(void *n);
void *rain(void *n);

// SHARE DATA
pthread_t tids[SIZE_N];
pthread_attr_t attr;
/* pthread_cancel(tids[i]); */

char **data;
volatile int count;

int main(void) {
	/* class(pos); */
	/* pos.set(&pos, 10, 20); */
    /* pos.put(&pos); */

	/* class2(pos2); */
	/* pos2->set(pos2, 30, 10); */
    /* pos2->put(pos2); */
	
    // CONVERT DATA FILE to ARRAY
    FILE *fp = fopen("words.txt", "r");
    int n_data = len_func(fp);
    data = save_func(fp, n_data);
    fclose(fp);

    // RANDOMIZE INDEX
    srand((unsigned)time(NULL)*getpid());
    int n_rand[SIZE_N] = { SIZE_N, };
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
    usleep(2000000);

    for(int i=1; i<SIZE_N; i++) {
        pthread_attr_init(&attr);
        // FUCTION CALL
        pthread_create(&tids[i], &attr, rain, data[n_rand[i]]);
        usleep(2000000);
        /* sleep(2); */
    }
    pthread_join(tids[0], NULL);
    for(int i=1; i<SIZE_N; i++) 
        pthread_join(tids[i], NULL);

    // END
    /* while( (ch=getch()) != '\n') ; */
    endwin();
    return 0; 
}

void *bar(void *n) {
    int *index_data = (int*)n;
    int i=0, x=2, y=2, ch=0;
    char temp[100];
    WINDOW *type=newwin(5, 20, (HEIGHT- 6), (WIDTH)/2-10);

    while(true && count < SIZE_N-1) {
        /* curs_set(TRUE); */
        if(kbhit())
        {
            ch = getch();
            switch (ch)
            {
                case 127: case 8: case KEY_BACKSPACE:
                    mvwprintw(type, y, --x, " \b");
                    wrefresh(type);
                    i--;
                    break;
                case 10: case ' ': 
                    temp[i] = '\0';
                    while(x > 0) {
                        mvwprintw(type, y, --x, " \b");
                        wrefresh(type);
                    }
                    i=0;
                    break;
                default:
                    temp[i]=ch;
                    mvwprintw(type, y, x++, "%c", temp[i]);
                    wrefresh(type);
                    i++;
                    break;
            }
            if(i<0) i=0;
            if(x<2) x=2;
            mvwprintw(type, y, x, "");
            box(type, 0, 0);
            wrefresh(type);
        } 
        else 
        {
            if(ch==10 || ch==' ') {
                /* curs_set(FALSE); */
                for(int i=1; i<index_data[0]; i++) {
                    if(strcmp(temp, data[index_data[i]])==0 ) {
                        count++;
                        erase();
                        refresh();
                        pthread_cancel(tids[i]);
                    }
                }
            }
            ch = 0;
            /* sleep(1); */
        }
    }
    pthread_exit(0);
}

void *rain(void *wi) {
    char *ldata = (char*)wi;
    int len = strlen(ldata);
    int h=3, w=len+2, y=1, x=rr(WIDTH-len-2); 

    WINDOW *block=newwin(h,w,y,x); 
    while(y < HEIGHT-8) {
        werase(block);
        wrefresh(block);
        block=newwin(h,w,y++,x);
        mvwprintw(block, 1, 1, "%s", ldata);
        box(block, 0, 0);
        wrefresh(block);
        /* sleep(1); */
        usleep(600000);
    }
    count++;
    werase(block);
    wrefresh(block);
    pthread_exit(0);
}

void set(class *self, int w, int h ) {
    if(w>0 && w<WIDTH)
        self->x=w;
    else if(w>0)
        self->x=0;
    else
        self->x=WIDTH;

    if(h>0 && h<HEIGHT)
        self->y=h;
    else if(h<0)
        self->y=0;
    else
        self->y=HEIGHT;
}

void put(class *self) {
    printf("%d, %d\n", self->x, self->y);
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





