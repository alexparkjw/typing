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

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif

struct rk_sema {
#ifdef __APPLE__
    dispatch_semaphore_t    sem;
#else
    sem_t                   sem;
#endif
};

static inline void rk_sema_init(struct rk_sema *s, uint32_t value) {
#ifdef __APPLE__
    dispatch_semaphore_t *sem = &s->sem;

    *sem = dispatch_semaphore_create(value);
#else
    sem_init(&s->sem, 0, value);
#endif
}

static inline void rk_sema_wait(struct rk_sema *s) {
#ifdef __APPLE__
    dispatch_semaphore_wait(s->sem, DISPATCH_TIME_FOREVER);
#else
    int r;
    do {
            r = sem_wait(&s->sem);
    } while (r == -1 && errno == EINTR);
#endif
}

static inline void rk_sema_post(struct rk_sema *s) {
#ifdef __APPLE__
    dispatch_semaphore_signal(s->sem);
#else
    sem_post(&s->sem);
#endif
}

#define WIDTH 90
#define HEIGHT 50
#define SIZE_N 21
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
/* sem_t mutex; */
/* pthread_mutex_t mutex; */
struct rk_sema s;
pthread_t tids[SIZE_N];
pthread_attr_t attr;
/* pthread_cancel(tids[i]); */

char **data;
volatile int count_down;
volatile int count_up;

int main(void) {
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
    /* sem_open((char*)&mutex, 0, 1); */
    /* sem_init(&mutex, 0, 1); */
    rk_sema_init(&s, 1);
    pthread_attr_init(&attr);
    pthread_create(&tids[0], &attr, bar, n_rand);
    usleep(1000000);

    for(int i=1; i<SIZE_N; i++) {
        pthread_attr_init(&attr);
        // FUCTION CALL
        pthread_create(&tids[i], &attr, rain, data[n_rand[i]]);
        usleep(1000000);
        /* sleep(2); */
    }
    pthread_join(tids[0], NULL);
    for(int i=1; i<SIZE_N; i++) 
        pthread_join(tids[i], NULL);

    /* sem_unlink((char*)&mutex); */
    /* sem_destroy(&mutex); */

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
    WINDOW *life=newwin(3, 10, (HEIGHT- 6), (WIDTH)/2-20);

    while(SIZE_N - count_up - count_down > 1 ) {
        /* curs_set(TRUE); */
        if(kbhit())
        {
            ch = getch();
            switch (ch)
            {
                case 127: case 8: case KEY_BACKSPACE:
                    rk_sema_wait(&s);
                    mvwprintw(type, y, --x, " \b");
                    wrefresh(type);
                    rk_sema_post(&s);
                    i--;
                    break;
                case 10: case ' ': 
                    temp[i] = '\0';
                    while(x > 0) {
                        rk_sema_wait(&s);
                        mvwprintw(type, y, --x, " \b");
                        wrefresh(type);
                        rk_sema_post(&s);
                    }
                    i=0;
                    break;
                default:
                    temp[i]=ch;
                    rk_sema_wait(&s);
                    mvwprintw(type, y, x++, "%c", temp[i]);
                    wrefresh(type);
                    rk_sema_post(&s);
                    i++;
                    break;
            }
            if(i<0) i=0;
            if(x<2) x=2;
            rk_sema_wait(&s);
            mvwprintw(type, y, x, "");
            box(type, 0, 0);
            wrefresh(type);
            rk_sema_post(&s);
        } 
        else 
        {
            rk_sema_wait(&s);
            mvwprintw(type, y, x, "");
            box(type, 0, 0);
            wrefresh(type);
            rk_sema_post(&s);
            if(ch==10 || ch==' ') {
                /* curs_set(FALSE); */
                for(int i=1; i<index_data[0]; i++) {
                    if(strcmp(temp, data[index_data[i]])==0 && tids[i] != NULL) {
                        count_up++;
                        rk_sema_wait(&s);
                        erase();
                        refresh();
                        rk_sema_post(&s);
                        pthread_cancel(tids[i]);
                    }
                }
            }
            ch = 0;
            /* sleep(1); */
        }
        rk_sema_wait(&s);
        mvwprintw(life, 1, 1, "%3d %3d", SIZE_N-1-count_up, SIZE_N-1-count_down);
        box(life, 0, 0);
        wrefresh(life);
        rk_sema_post(&s);
    }
    pthread_exit(0);
}

void *rain(void *wi) {
    char *ldata = (char*)wi;
    int len = strlen(ldata);
    int h=3, w=len+2, y=1, x=rr(WIDTH-len-2); 

    WINDOW *block=newwin(h,w,y,x); 
    while(y < HEIGHT-8) {
        /* sem_wait(&mutex); */
        rk_sema_wait(&s);
        werase(block);
        wrefresh(block);
        block=newwin(h,w,y++,x);
        mvwprintw(block, 1, 1, "%s", ldata);
        box(block, 0, 0);
        wrefresh(block);
        /* sem_post(&mutex); */
        rk_sema_post(&s);
        /* sleep(1); */
        usleep(300000);
    }
    rk_sema_wait(&s);
    count_down++;
    werase(block);
    wrefresh(block);
    rk_sema_post(&s);
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





