//
//  macro.c
//  typing program
//
//  Created by Alex on 01/01/2020.
//  Copyright © 2020 Alex. All rights reserved.
//

/* #include <stdio.h> */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
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

typedef struct Vector {
    int x, y, h, w, o, len;
    char **data;
    WINDOW *win;
    void (*set)(struct Vector*, int, int, int, int, int, char**);
    void (*put)(struct Vector*);
}vector;


#define WIDTH 100
#define HEIGHT 50
#define SIZE_N 21
#define endl printf("\n")
#define rr(k) rand()%(k)
#define vector(p) struct Vector (*p)=(struct Vector *)malloc(sizeof(struct Vector));\
                 (p)->set=set;\
                 (p)->put=put


void set(vector *self, int w, int h, int y, int x, int len, char **data);
void put(vector *self);
int kbhit(void);
char *strDup(const char * str);
int strCmp(void * vp1, void * vp2);
int len_func(FILE *fp);
char **save_func(FILE *fp, int len);

void *bar(void *n);
void *rain(void *n);

void print_menu(vector *menu, int highlight );
void print_basic(vector *basic);
int set_basic(WINDOW *win, char *temp, int lv );
int print_words(vector * words );
int print_sentence(vector *sentence );
int print_graph(vector *paragraph, int *l );
void option_menu(vector *option, int **num);
int set_option(vector *option);
void load_option(char *filename, int **op);
void save_option(char *filename, int **op);

// PTHREAD and SEMAPHORE
/* sem_t mutex; */
/* pthread_mutex_t mutex; */
struct rk_sema s;
pthread_t tids[SIZE_N];
pthread_attr_t attr;
/* pthread_cancel(tids[i]); */


// SHARE DATA
char **data_menu;
char **data_basic;
char **data_words;
char **data_sentence;
char **data_paragraph;
char **data_option;

volatile int count_down;
volatile int count_up;

int main(void) {
    // CONVERT DATA FILE to ARRAY
    FILE *fp_menu = fopen("menu.txt", "r");
    int len_menu = len_func(fp_menu);
    data_menu = save_func(fp_menu, len_menu);
    fclose(fp_menu);

    FILE *fp_basic = fopen("basic.txt", "r");
    int len_basic = len_func(fp_basic);
    data_basic = save_func(fp_basic, len_basic);
    fclose(fp_basic);

    FILE *fp_words = fopen("words.txt", "r");
    int len_words = len_func(fp_words);
    data_words = save_func(fp_words, len_words);
    fclose(fp_words);

    FILE *fp_sentence = fopen("sentence.txt", "r");
    int len_sentence = len_func(fp_sentence);
    data_sentence = save_func(fp_sentence, len_sentence);
    fclose(fp_sentence);

    FILE *fp_paragraph = fopen("paragraph.txt", "r");
    int len_paragraph = len_func(fp_paragraph);
    data_paragraph = save_func(fp_paragraph, len_paragraph);
    fclose(fp_paragraph);

    FILE *fp_option = fopen("option.txt", "r");
    int len_option = len_func(fp_option);
    data_option = save_func(fp_option, len_option);
    fclose(fp_option);

    // RANDOMIZE INDEX
    srand((unsigned)time(NULL)*getpid());
    int n_rand[SIZE_N] = { SIZE_N, };
    for(int i=1; i<SIZE_N; i++)
       n_rand[i] = rr(len_words);

    // INIT NCURSES
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    scrollok(stdscr, true);
    /* curs_set(FALSE); */


    //combine vectors
    vector(menu);
    vector(basic);
    vector(words);
    vector(sentence);
    vector(paragraph);
    vector(option);
    menu->set(menu, 4, 4, (HEIGHT-HEIGHT/4)/2, (WIDTH-WIDTH/4)/2, len_menu, data_menu);
    menu->set( basic, 4, 2, (HEIGHT - HEIGHT/4)/2, (WIDTH - WIDTH/2)/2, len_basic, data_basic);
    words->set( words, 6, 1, (HEIGHT - HEIGHT/6)/2, (WIDTH - WIDTH/1)/2, len_words, data_words);
    sentence->set( sentence, 6, 1, (HEIGHT - HEIGHT/6)/2, (WIDTH - WIDTH/1)/2, len_sentence, data_sentence);
    paragraph->set( paragraph, 2, 1, (HEIGHT - HEIGHT/2)/2, (WIDTH - WIDTH/1)/2, len_paragraph, data_paragraph);
    option->set(option, 4, 4, (HEIGHT - HEIGHT/4)/2, (WIDTH - WIDTH/4)/2, len_option, data_option);
        
    int *op[]= { &basic->o, &words->o, &sentence->o, &paragraph->o};
    load_option("option.conf", op);


    keypad(menu->win, TRUE);
    int c = 0, l=0, highlight = 1;
    while(true) {
        curs_set(FALSE); 
        print_menu(menu, highlight);
        wrefresh(menu->win);
        c = wgetch(menu->win);
        switch(c)
        {
            case 'k': case KEY_UP:
                if(highlight == 1)
                    highlight = menu->len;
                else
                    --highlight;
                break;
            case 'j': case KEY_DOWN:
                if(highlight == menu->len)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case '\n':
                if(highlight == 1) {
                    werase(menu->win);
                    wrefresh(menu->win);
                    print_basic(basic) ;
                    werase(basic->win);
                    wrefresh(basic->win);
                }
                else if(highlight == 2) {
                    werase(menu->win);
                    wrefresh(menu->win);
                    for(int i=0; i < words->o; i++) {
                        if(print_words(words) )
                            break;
                    }
                    werase(words->win);
                    wrefresh(words->win);
                }
                else if( highlight == 3) {
                    werase(menu->win);
                    wrefresh(menu->win);
                    for(int i=0; i < sentence->o; i++) {
                        if(print_sentence(sentence) )
                            break;
                    }
                    werase(sentence->win);
                    wrefresh(sentence->win);
                }
                else if(highlight == 4) {
                    werase(menu->win);
                    wrefresh(menu->win);
                    for(int i=0; i< paragraph->o; i++) {
                        if( print_graph(paragraph, &l) )
                            break;
                    }
                    werase(paragraph->win);
                    wrefresh(paragraph->win);
                }
                else if(highlight == 5) {
                    werase(menu->win);
                    wrefresh(menu->win);

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
                        pthread_create(&tids[i], &attr, rain, data_words[n_rand[i]]);
                        usleep(1000000);
                        /* sleep(2); */
                    }
                    pthread_join(tids[0], NULL);
                    for(int i=1; i<SIZE_N; i++) 
                        pthread_join(tids[i], NULL);
                    erase();
                    refresh();
                }
                else if(highlight == 6) {
                    werase(menu->win);
                    wrefresh(menu->win);
                    option_menu(option, op); 
                    save_option("option.conf", op);
                    werase(option->win);
                    wrefresh(option->win);
                }
                else if(highlight == 7) {
                    clrtoeol();
                    refresh();
                    endwin();
                    return 0;
                }
                break;
            default:
                refresh();
                break;
        }
    }



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
                    if(strcmp(temp, data_words[index_data[i]])==0 && tids[i] != NULL) {
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
        mvwprintw(life, 1, 1, "%3d %3d", count_up, SIZE_N-1-count_down);
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

void set(vector *self, int h, int w, int y, int x, int len, char **data) {
    if(h>=0 && h<HEIGHT) self->h=HEIGHT/h;
    else if(h<0) self->h=0;
    else self->h=HEIGHT;

    if(w>=0 && w<WIDTH) self->w=WIDTH/w;
    else if(w<0) self->w=0;
    else self->w=WIDTH;

    if(y>=0 && y<HEIGHT) self->y=y;
    else if(y<0) self->y=0;
    else self->y=(HEIGHT-y)/2;

    if(x>=0 && x<WIDTH) self->x=x;
    else if(x<0) self->x=0;
    else self->x=(WIDTH-x)/2;

    self->win = newwin(self->h, self->w, self->y, self->x);
    self->data = data;
    self->len = len;
}

void put(vector *self) {
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
    char buff[200];
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


void print_menu(vector *menu, int highlight ) {
    int y=2, x=2;
    box(menu->win, (int)0, (int)0);
    for(int i = 0; i<menu->len; i++, y++) {
        if(highlight == i + 1) {
            wattron(menu->win, A_REVERSE);
            mvwprintw(menu->win, y, x, "%s", menu->data[i]);
            wattroff(menu->win, A_REVERSE);
        }
        else
            mvwprintw(menu->win, y, x, "%s", menu->data[i]);
    }
    /* werase(menu->win); */
    wrefresh(menu->win);
}

void print_basic(vector *basic) {
    curs_set(FALSE); 
    wrefresh(basic->win);
    char temp[150];
    int highlight=1;

    int c = 0;
    keypad(basic->win, TRUE);
    while(1) {
        wrefresh(basic->win);
        print_menu(basic, highlight);
        c = wgetch(basic->win);
        switch(c)
        {
            case 'k': case KEY_UP:
                if(highlight == 1) highlight = basic->len;
                else
                    --highlight;
                break;
            case 'j': case KEY_DOWN:
                if(highlight == basic->len)
                    highlight = 1;
                    else 
                    ++highlight;
                break;
            case '\n':
                if( highlight >= 1 && highlight <=5) {
                    werase(basic->win);
                    wrefresh(basic->win);
                    for(int i=0; i< basic->o; i++) {
                        if(set_basic(basic->win, temp, highlight-1))
                            return; 
                    }
                    werase(basic->win);
                    wrefresh(basic->win);
                }
                else
                    return;
                refresh();
                break;
            case 27:
                refresh();
                return;
                break;
            default:
                refresh();
                return ;
                break;
        }
    }
}


int set_basic(WINDOW *win, char *temp, int lv ) {
    curs_set(TRUE); 
    int ch;
    int i=0, y=2, x=2;
    box(win, 0, 0);
    wrefresh(win);

    char *ran[]={ 
        "asdfasdfghjkljkl;",
        "qwerqwertyuiopuiop",
        "zxcvzxcvbnmbnm,.",
        "[]{}\\|-_=+(),.<>/?",
        "1234567890"
    };
    int k = strlen(ran[lv]);
    char ra[30];
    for(int i=0; i<6; i++) {
        for(int j=0; j<5; j++) {
           if(j==4)
               ra[i*5+j]=' ';
           else
               ra[i*5+j]=ran[lv][rand()%k];
        }
    }
    ra[29]='\0';

    mvwprintw(win, y++, x, "%s", ra);
    mvwprintw(win, y, x, "");
    keypad(win, TRUE);

    time_t start, end;
    double delta;
    time(&start);

    while( (ch=wgetch(win)) != '\n' && strlen(ra) != i) {
        switch (ch) 
        {
            case 27:
                werase(win);
                wrefresh(win);
                return 1;
                break;
            case 127: case 8: case KEY_BACKSPACE:
                mvwprintw(win, y, --x, " \b");
                mvwprintw(win, y-2, x, " \b");
                i--;
                break;
            default:
                temp[i]=ch;
                mvwprintw(win, y, x++, "%c", temp[i]);
                if(ra[i] == temp[i])
                    mvwprintw(win, y-2, x-1, " ");
                else{
                    mvwprintw(win, y-2, x-1, "v");
                    beep();
                }
                i++;
        }
        if(i<0) i=0;
        if(x<2) x=2;
        mvwprintw(win, y, x, "");
    }
    temp[i]='\0';

    time(&end);
    delta = difftime(end, start);

    if(temp[0] != '\0') { mvwprintw(win, y+2, 2, "%.2lf per minute", strlen(temp)/delta*60);
        while( (ch=wgetch(win)) != '\n' && ch != ' ') { }
    }
    werase(win);
    wrefresh(win);
    return 0;
}


int print_words(vector * words ) {
    char temp[150];
    char mix[150];
    int ch, i=0, y=2, x=2;

    curs_set(TRUE); 
    box(words->win, 0, 0);
    wrefresh(words->win);
    strcpy(mix, words->data[rand()%words->len]);
    for(int i=0; i<6; i++) {
        strcat(mix, " ");
        strcat(mix, words->data[rand()%words->len]);
    }

    mvwprintw(words->win, y++, x, "%s", mix);
    mvwprintw(words->win, y, x, "");
    keypad(words->win, TRUE);

    time_t start, end;
    double delta;
    time(&start);

    while( (ch=wgetch(words->win)) != '\n' && strlen(mix) != i) {
        switch (ch) 
        {
            case 27:
                werase(words->win);
                wrefresh(words->win);
                return 1;
                break;
            case 127: case 8: case KEY_BACKSPACE:
                mvwprintw(words->win, y, --x, " \b");
                mvwprintw(words->win, y-2, x, " \b");
                i--;
                break;
            default:
                temp[i]=ch;
                mvwprintw(words->win, y, x++, "%c", temp[i]);
                if(mix[i] == temp[i])
                    mvwprintw(words->win, y-2, x-1, " ");
                else {
                    mvwprintw(words->win, y-2, x-1, "v");
                    beep();
                }
                i++;
        }
        if(i<0) i=0;
        if(x<2) x=2;
        mvwprintw(words->win, y, x, "");
    }
    temp[i]='\0';

    time(&end);
    delta = difftime(end, start);;

    if(temp[0] != '\0') {
        mvwprintw(words->win, y+2, 2, "%.2lf per minute", strlen(temp)/delta*60);
        while( (ch=wgetch(words->win)) != '\n' && ch != ' ') { }
    }
    werase(words->win);
    wrefresh(words->win);
    return 0;
}

int print_sentence(vector *sentence ) {
    char temp[150];
    char mix[150];
    strcpy(mix, sentence->data[rand()%sentence->len]);
    int ch, i=0, y=2, x=2;

    curs_set(TRUE); 
    box(sentence->win, 0, 0);
    wrefresh(sentence->win);
    mvwprintw(sentence->win, y++, x, "%s", mix);
    mvwprintw(sentence->win, y, x, "");
    keypad(sentence->win, TRUE);

    time_t start, end;
    double delta;
    time(&start);

    while( (ch=wgetch(sentence->win)) != '\n' && strlen(mix) != i) {
        switch (ch) 
        {
            case 27:
                werase(sentence->win);
                wrefresh(sentence->win);
                return 1;
                break;
            case 127: case 8: case KEY_BACKSPACE:
                mvwprintw(sentence->win, y, --x, " \b");
                mvwprintw(sentence->win, y-2, x, " \b");
                i--;
                break;
            default:
                temp[i]=ch;
                mvwprintw(sentence->win, y, x++, "%c", temp[i]);
                if(mix[i] == temp[i])
                    mvwprintw(sentence->win, y-2, x-1, " ");
                else{
                    mvwprintw(sentence->win, y-2, x-1, "v");
                    beep();
                }
                i++;
        }
        if(i<0) i=0;
        if(x<2) x=2;
        mvwprintw(sentence->win, y, x, "");
    }
    temp[i]='\0';

    time(&end);
    delta = difftime(end, start);;

    if(temp[0] != '\0') {
        mvwprintw(sentence->win, y+2, 2, "%.2lf per minute", strlen(temp)/delta*60);
        while( (ch=wgetch(sentence->win)) != '\n' && ch != ' ') { }
    }
    werase(sentence->win);
    wrefresh(sentence->win);
    return 0;
}

int print_graph(vector *paragraph, int *l ) {
    char temp[5][150];
    int ch,i=0, y=2, x=2;

    curs_set(TRUE); 
    box(paragraph->win, 0, 0);
    wrefresh(paragraph->win);

    for(int i=0; i<5; i++) {
        if(paragraph->data[*l+i][strlen(paragraph->data[*l+i])-1] == ' ')
            paragraph->data[*l+i][strlen(paragraph->data[*l+i])-1] = '\0';
        mvwprintw(paragraph->win, i*4 + y, x, "%s", paragraph->data[*l+i]);
    }
    y++;

    keypad(paragraph->win, TRUE);

    time_t start, end;
    double delta;
    time(&start);

    for(int j=0; j<5; j++, x=2) {
        mvwprintw(paragraph->win, 4*j+y, x, "");
        while( (ch=wgetch(paragraph->win)) != '\n' && strlen(paragraph->data[*l+j]) != i) {
            switch (ch) 
            {
                case 27:
                    werase(paragraph->win);
                    wrefresh(paragraph->win);
                    return 1;
                    break;
                case 127: case 8: case KEY_BACKSPACE:
                    mvwprintw(paragraph->win, 4*j+y, --x, " \b");
                    mvwprintw(paragraph->win, 4*j+y-2, x, " \b");
                    i--;
                    break;
                default:
                    temp[j][i]=ch;
                    mvwprintw(paragraph->win, 4*j+y, x++, "%c", temp[j][i]);
                    if(paragraph->data[*l+j][i] == temp[j][i])
                        mvwprintw(paragraph->win, 4*j+y-2, x-1, " ");
                    else{
                        mvwprintw(paragraph->win, 4*j+y-2, x-1, "v");
                        beep();
                    }
                    i++;
            }
            if(i<0) i=0;
            if(x<2) x=2;
            mvwprintw(paragraph->win, 4*j+y, x, "");
        }
        temp[j][i]='\0';
        i=0;
    }

    time(&end);
    delta = difftime(end, start);;

    int shortlen=0;
    for(int i=0; i<5; i++)
        shortlen+=strlen(temp[i]);

    if(temp[0][0] != '\0') {
        mvwprintw(paragraph->win, 4*4+y+2, 2, "%.2lf per minute", ((double)shortlen) / delta*60);
        while( (ch=wgetch(paragraph->win)) != '\n' && ch != ' ' ) { }
    }

    werase(paragraph->win);
    wrefresh(paragraph->win);
    *l += 5;
    return 0;
}

void option_menu(vector *option, int **num) {
    curs_set(FALSE); 
    wrefresh(option->win);
    int highlight=1;

    int c = 0;
    keypad(option->win, TRUE);
    while(1) {
        wrefresh(option->win);
        print_menu(option, highlight);
        c = wgetch(option->win);
        switch(c)
        {
            case 'k': case KEY_UP:
                if(highlight == 1)
                    highlight = option->len;
                else
                    --highlight;
                break;
            case 'j': case KEY_DOWN:
                if(highlight == option->len)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case '\n':
                if( highlight >= 1 && highlight < 6) {
                    werase(option->win);
                    wrefresh(option->win);
                    *num[highlight-1] = set_option(option); 
                    werase(option->win);
                    wrefresh(option->win);
                }
                else {
                    return;
                }
                break;
            case 27:
                werase(option->win);
                wrefresh(option->win);
                return ;
                break;
            default:
                refresh();
                break;
        }
    }
}

int set_option(vector *option) {
    int ch, ret=10;
    int i=0, y=2, x=2;
    char str[100];

    curs_set(TRUE); 
    box(option->win, 0, 0);
    wrefresh(option->win);
    mvwprintw(option->win, y++, x, "Enter Value for option  : ");
    keypad(option->win, TRUE);
    while( (ch=wgetch(option->win)) != '\n') {
        switch (ch) 
        {
            case 127: case 8: case KEY_BACKSPACE:
                mvwprintw(option->win, y, --x, " \b");
                i--;
                break;
            default:
                str[i]=ch;
                mvwprintw(option->win, y, x++, "%c", str[i]);
                i++;
        }
        if(i<0) i=0;
    }
    str[i]='\0';
    if(str[0] != '\0') {
        mvwprintw(option->win, y+1, 2, "set up %s", str);
        ret=atoi(str);
        while( (ch=wgetch(option->win)) != '\n') { }
    }

    werase(option->win);
    wrefresh(option->win);
    return ret;
}

void load_option(char *filename, int **op) {
    FILE *fp=fopen(filename, "r");
    if(fp == NULL) {
        printf("%s file cannot open \n", filename);
        return ;
    }
    for(int i=0; i<4; i++ )
        fscanf(fp, " %d", op[i]);

    fclose(fp);
}

void save_option(char *filename, int **op) {
    FILE *fp=fopen(filename, "w+");
    if(fp == NULL) {
        printf("%s file cannot open \n", filename);
        return ;
    }
    for(int i=0; i<4; i++ )
        fprintf(fp, "%d\n", *op[i]);

    fclose(fp);
}


