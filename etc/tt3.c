#include "header_all.h"
#include <assert.h>
#include <stdio.h>

#define HEIGHT 40
#define WIDTH 80
#define SIZE_N 30
#define rr(k) rand()%(k)

// FUNC
char *strDup(const char * str);
int strCmp(void * vp1, void * vp2);
int len_func(FILE *fp);
char **save_func(FILE *fp, int len);
int kbhit();
void *rain(void *w);

// share data
char **data;  
int n_data;
char tmp[100];
int ch=1, t_y=1, t_x=2, i=0; 
int n_ran[100];
int ind;

/* sem_t mutex; */
pthread_mutex_t mutex;
pthread_t tids[SIZE_N], id_tmp, id_ex;
pthread_attr_t attr;

// MAIN
int main(int argc, char **argv) { 
    srand((unsigned)time(NULL)*getpid());

    // FILE to DATA structur such as p->data[i] 
    FILE *fp = fopen("words.txt", "r");
    n_data = len_func(fp);
    data = save_func(fp, n_data);

    // NCURSES
    initscr(), clear();
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
    for(int i=0; i<SIZE_N; i++) {
        pthread_attr_init(&attr);
        pthread_create(&tids[i], &attr, rain, win_life);
        sleep(3);
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

void *block(void*w) {
    WINDOW *block=block;;

}

void *rain(void *w) {
    /* sem_wait(&mutex); */
    /* pthread_mutex_lock(&mutex); */
    WINDOW *block, *win = (WINDOW *)w;
    n_ran[++ind] = rr(n_data);
    int len = strlen(data[n_ran[ind]])+3;
    char check[100];
    strcpy(check, data[n_ran[ind]]);
    int y=1, x = rr(WIDTH-len)+1;

    while(y < HEIGHT-8) {
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
                    mvwprintw(win, t_y+2, t_x, "%s,%s", tmp,data[n_ran[ind]]);
                    wrefresh(win);
                    
                    for(int q=ind; q>0; q--) {
                        if(strcmp(tmp, data[n_ran[q]]) == 0) {
                            /* werase(block); */
                            /* wrefresh(block); */
                            erase();
                            /* refresh(); */
                            pthread_cancel(tids[q]);
                            /* pthread_exit(tids[q]); */
                        }
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
            mvwprintw(block, 1, 2, "%s", check);
            wrefresh(block);
            usleep(700000);
            /* sleep(1); */
            /* refresh(); */
        }
    }

    werase(block);
    wrefresh(block);

    /* pthread_mutex_unlock(&mutex); */
    /* sem_post(&mutex); */
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
