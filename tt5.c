#include "header_all.h"

#define HEIGHT 40
#define WIDTH 80
#define SIZE_N 30
#define rr(k) rand()%(k)

typedef struct stack {
    void *data_elems;
    int size_datatype;
    int index_data;
    int len_alloc;
    pthread_t *id;
} stack;

void stackNew(stack* s, int size_datatype);
void stackDispose(stack* s);
static void stackGrow(stack* s);
void stackPush(stack* s, void* elemAddr, pthread_t *id);
void stackPop(stack* s, void* elemAddr, pthread_t *id);


// FUNC
char *strDup(const char * str);
int strCmp(void * vp1, void * vp2);
int len_func(FILE *fp);
char **save_func(FILE *fp, int len);
int kbhit();
void *rain(void *w);

// share data
sem_t mutex;
char **data;  
int n_data;
char tmp[100];
char data_now[100];
int ch=1, t_y=1, t_x=2, i=0; 
stack s1, s2;
char check[100];

pthread_t tids[SIZE_N], id_tmp, id_ex;

// MAIN
int main(int argc, char **argv) { 
    srand((unsigned)time(NULL)*getpid());

    // FILE to DATA structur such as p->data[i] 
    FILE *fp = fopen("words.txt", "r");
    n_data = len_func(fp);
    data = save_func(fp, n_data);

    // NCURSES
    initscr();
    clear();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);
    curs_set(FALSE); 

    WINDOW *win_main, *win_life, *win_score ;
    win_main=newwin(HEIGHT, WIDTH, 0, 0);
    box(win_main, 0, 0);
    wrefresh(win_main);

    win_life=newwin(5, 20, (HEIGHT- 6), (WIDTH)/2-10);
    box(win_life, 0, 0);
    wrefresh(win_life);

    sem_open((char*)&mutex, 0, 1);
    /* sem_init(&mutex, 0, 1); */

    /* pid_t pid = fork(); */    
    /* pthread_t tids[SIZE_N]; */
    /* pthread_attr_t attr; */
    /* pthread_attr_init(&attr); */

    // FUCTION CALL
    stackNew(&s1, sizeof(char*));
    stackNew(&s2, sizeof(char*));
    for(int i=0; i<SIZE_N; i++) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i], &attr, rain, win_life);
        sleep(2);
    }

    for(int i=0; i<SIZE_N; i++) {
        pthread_join(tids[i], NULL);
    }
    sem_unlink((char*)&mutex);
    /* sem_distory(&mutex); */

    // END
    int ch;
    while( (ch=wgetch(win_main)) != '\n') ;
    endwin();
    return 0;
}

void *rain(void *w) {
    sem_wait(&mutex);
    WINDOW *block, *win = (WINDOW *)w;

    int n_ran = rr(n_data);
    strcpy(data_now, data[n_ran]);

    int len = strlen(data[n_ran])+3;
    id_tmp = pthread_self(); 
    stackPush(&s1, data[n_ran], &id_tmp);
    int y=1, x = rr(WIDTH-len)+1;

    //type
    /* int ch=1, t_y=1, t_x=2, i=0; */ 
    while(y < HEIGHT-8) {
        if(pthread_self() == id_ex) {
            werase(win);
            wrefresh(win);
            werase(block);
            wrefresh(block);
            /* pthread_cancel(pthread_self()); */
            pthread_exit(0);
        }
        if(kbhit()) {
            curs_set(TRUE);
            ch =  wgetch(win);
            switch(ch) 
            {
                case 27:
                    werase(win);
                    wrefresh(win);
                    pthread_exit(0);
                    break;
                case 127: case 8: case KEY_BACKSPACE:
                    mvwprintw(win, t_y, --t_x, " \b");
                    wrefresh(win);
                    i--;
                    break;
                case '\n': case ' ':
                    // set
                    tmp[i]='\0', ch=1, t_y=1, t_x=2, i=0; 
                    werase(win);
                    mvwprintw(win, t_y+2, t_x, "%s, %s", data_now, data[n_ran]);
                    wrefresh(win);

                    while(s1.index_data != 0) {
                        stackPop(&s1, &check, &id_tmp);
                        if(strcmp(tmp, check) == 0) {
                            id_ex = id_tmp;
                            pthread_cancel(id_ex);
                        } else {
                            stackPush(&s2, &check, &id_tmp);
                        }
                    }
                    while(s2.index_data !=0) {
                        stackPop(&s2, &check, &id_tmp);
                        stackPush(&s1, &check, &id_tmp);
                    }
                    werase(block);
                    wrefresh(block);
                    i=0;
                    t_x=2;
                    if(pthread_self() == id_ex) {
                        werase(block);
                        wrefresh(block);
                        /* pthread_cancel(pthread_self()); */
                        pthread_exit(0);
                    }
                    break;
                default:
                    tmp[i]=ch;
                    mvwprintw(win, t_y, t_x++, "%c", tmp[i]);
                    i++;
            }
            if(i<0) i=0;
            if(t_x<2) t_x=2;
            mvwprintw(win, t_y, t_x, "");
            wrefresh(win);
            /* getch(); */
            /* refresh(); */
        }
        else {
            curs_set(FALSE);
            box(win, 0, 0);
            wrefresh(win);
            block = newwin(3, len, y++, x);
            mvwprintw(block, 1, 2, "%s", data[n_ran]);
            wrefresh(block);
            usleep(700000);
            /* sleep(1); */
            /* refresh(); */
        }
        /* usleep(700000); */
    }

    werase(block);
    wrefresh(block);

    sem_post(&mutex);
    /* pthread_cancel(pthread_self()); */
    pthread_exit(0);
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

int kbhit(void) {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    }
    return 0;
}

void stackNew(stack* s, int size_datatype){
    assert(size_datatype > 0);

    s->size_datatype = size_datatype;
    s->index_data    = 0;
    s->len_alloc     = 4;
    s->data_elems    = (char*)malloc(s->size_datatype * s->len_alloc);
    s->id            = (pthread_t*)malloc(sizeof(pthread_t) * s->len_alloc);

    assert(s->data_elems != NULL);
}

void stackDispose(stack* s) {
    free(s->data_elems);
}

static void stackGrow(stack* s){
    s->len_alloc *= 2;
    s->data_elems = realloc(s->data_elems, s->size_datatype * s->len_alloc );
    s->id = realloc(s->id, sizeof(pthread_t *)* s->len_alloc );
}

void stackPush(stack* s, void* elemAddr, pthread_t *id){
    if (s->index_data == s->len_alloc)
        stackGrow(s);

    void *target = (char*)s->data_elems + s->index_data * s->size_datatype;
    memcpy(target, elemAddr, s->size_datatype);
    s->id[s->index_data] = *id;
    s->index_data++;
}

void stackPop(stack* s, void* elemAddr, pthread_t *id ){
    void* source = (char*)s->data_elems + (s->index_data - 1)*s->size_datatype;
    memcpy(elemAddr, source, s->size_datatype);
    *id = s->id[s->index_data];
    s->index_data--;
}


