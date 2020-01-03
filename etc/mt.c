#include "header_all.h"
#define SIZE_N 4

int kbhit(void);
void *foo(void *n);
void *bar(void *n);

pthread_t tids[SIZE_N];
pthread_attr_t attr;
int ch, x, y, index_temp;
char temp[500];

int main(void) { 
    srand((unsigned)time(NULL)*getpid());
    initscr();
    cbreak();
    nodelay(stdscr, true);
    noecho();
    scrollok(stdscr, true);

    int s[SIZE_N]={0,1,2,3};
    pthread_attr_init(&attr);
    pthread_create(&tids[0], &attr, bar, &s[0]);

    for(int i=1; i<SIZE_N; i++) {
        pthread_attr_init(&attr);
        // FUCTION CALL
        pthread_create(&tids[i], &attr, foo, &s[i]);
        sleep(1);
    }

    pthread_join(tids[0], NULL);
    for(int i=1; i<SIZE_N; i++) 
        pthread_join(tids[i], NULL);

    /* while( (ch=getch()) != '\n') ; */
    endwin();
    return 0;
}
void *foo(void *n) {
    while(true) {
        mvprintw(y++,x,"%d = %p", *(int*)n, pthread_self() );    
        sleep(1);
    }
    pthread_exit(0);
}
void *bar(void *n) {
    while(true) {
        if(kbhit()) {
            ch = getch();
            temp[index_temp] = ch;
            if(ch != '\n' || ch !=' ') 
                temp[++index_temp] = '\0';
            mvprintw(y++,x,"input >>>>>>>>>> %s", temp);
            switch(ch) 
            {
                case '1': 
                    mvprintw(y++,x,"111111111111111111");
                    pthread_cancel(tids[1]);
                    /* pthread_exit(tids[1]); */
                    break;
                case '2': 
                    mvprintw(y++,x,"222222222222222222");
                    pthread_cancel(tids[2]);
                    /* pthread_exit(tids[2]); */
                    break;
                case '3': 
                    mvprintw(y++,x,"33333333333333333333333");
                    pthread_cancel(tids[3]);
                    /* pthread_exit(tids[3]); */
                    break;
                case '\n': case ' ':
                    mvprintw(y++,x,"xxxxxxxxxxxxxxxxxxxxx");
                    for(int i=0; i<SIZE_N; i++) 
                        pthread_cancel(tids[i]);
                    for(int i=0; i<SIZE_N; i++) 
                        pthread_exit(tids[i]);
                    pthread_exit(0);
                    break;
                default:
                    ;
                    break;
            }
            refresh();
        }
        else {
            mvprintw(y++,x,"o:%s %d = %p", temp, *(int*)n, pthread_self() );    
            refresh();
            sleep(1);
        }
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
