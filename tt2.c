#include "header_all.h"

#define HEIGHT 40
#define WIDTH 80
#define SIZE_N 60
#define wwp(k) newwin(k->h,k->w,k->y,k->x)
#define wws(k) newwin(k.h,k.w,k.y,k.x)
#define rr(k) rand()%k

/* sem_t mutex; */

// FUNC P-Type
int kbhit(void);

int main(int argc, char **argv) { 
    srand((unsigned)time(NULL)*getpid());

    // NCURSES
    initscr();
    clear();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    scrollok(stdscr, TRUE);
    curs_set(FALSE); 

    WINDOW *win_main, *win_game, *win_next, *win_score, *block[4];

    win_main=newwin(HEIGHT, WIDTH, 0, 0);
    box(win_main, 0, 0);
    wrefresh(win_main);

    win_game=newwin(HEIGHT-2, WIDTH/2+10, 1, 2);
    box(win_game,0, 0);
    wrefresh(win_game);

    win_next=newwin(HEIGHT/4, WIDTH/4, 4, 56);
    box(win_next,0, 0);
    wrefresh(win_next);

    win_score=newwin(HEIGHT/9, WIDTH/4, 15, 56);
    box(win_score,0, 0);
    wrefresh(win_score);

    int h=3, w=6, y=2, x=4, ch;
    block[0]=newwin(h, w, y, x);

    for(int i=0; i<4; i++) {
        box(block[i],0, 0);
        wrefresh(block[i]);
    }

    while(y < HEIGHT-2) {
        if(kbhit()) {
            ch =  wgetch(win_game);
            switch(ch) 
            {
                case 'k': case KEY_UP:
                    y++;                    
                    break;
                case 'j': case KEY_DOWN:
                    y++;
                    break;
                ase 'h': case KEY_LEFT:
                    x--; 
                    break;

                case 'l': case KEY_RIGHT:
                    x++;
                    break;
                case ' ':
                    x++;
                    break;
                default:
                    ;
                    break;
            }
        }
        else {
            if(x<4) x=4;
            if(x>44)x=44;
            box(win_game, 0, 0);
            box(win_next, 0, 0);
            box(win_score, 0, 0);
            box(block[0], 0, 0);
            refresh(); 
            block[0] = newwin(h, w, y++, x);
            box(block[0], 0, 0);
            wrefresh(win_game);
            wrefresh(win_next);
            wrefresh(win_score);
            wrefresh(block[0]);
            usleep(700000);
            /* sleep(1); */
        }
    }

    while( (ch=wgetch(win_main)) != '\n') ;
    endwin();

    return 0;
}


int kbhit(void) {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    }
    return 0;
}
