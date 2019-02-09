#include<pthread.h>
#include<curses.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define HEIGHT 48
#define WIDTH 80
#define num 8

sem_t mutex;
    int i=0, x=1, y=1;
    char temp[32];

struct horse {
    char name[32];
    int h, w, y, x;
    WINDOW *win;
};

void * foo (void * v);
void *type(void *v);

int main(int argc, char **argv) { 
    srand(time(NULL));
    initscr();
    clear();
    cbreak();
    noecho();
    curs_set(FALSE); 

    struct horse s[]={ 
        { .name="aaaa", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=0, 
            .win=newwin(s[0].h, s[0].w, s[0].y, s[0].x) },
        { .name="bbbb", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=10, 
            .win=newwin(s[1].h, s[1].w, s[1].y, s[1].x) },
        { .name="cccc", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=20, 
            .win=newwin(s[2].h, s[2].w, s[2].y, s[2].x) },
        { .name="dddd", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=30, 
            .win=newwin(s[3].h, s[3].w, s[3].y, s[3].x) },
        { .name="eeee", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=40, 
            .win=newwin(s[4].h, s[4].w, s[4].y, s[4].x) },
        { .name="ffff", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=50, 
            .win=newwin(s[5].h, s[5].w, s[5].y, s[5].x) },
        { .name="gggg", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=60, 
            .win=newwin(s[6].h, s[6].w, s[6].y, s[6].x) },
        { .name="hhhh", .h=HEIGHT/10, .w=WIDTH/8, .y=1, .x=70, 
            .win=newwin(s[7].h, s[7].w, s[7].y, s[7].x) },
        { .name="     ", .h=HEIGHT/10, .w=WIDTH/8, .y=30, .x=(WIDTH-s[8].w)/2, 
            .win=newwin(s[8].h, s[8].w, s[8].y, s[8].x) },
    };

    sem_open((char*)&mutex, 0, 1);
    pthread_t tids[num+1];
    int sum[num]={}, r, k=0;
    char *rank[8]={" 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8" };

    
    for(int j=0; sum[0]+sum[1]+sum[2]+sum[3]+sum[4]+sum[5]+sum[6]+sum[7]!=320 ; j++) {
        for(int i=0; i<num; i++) {
            if(j != 0 && s[i].y <= 40) {
                werase(s[i].win);
                wrefresh(s[i].win);
                if(sum[i] < 40)
                    r = rand()%3+1;
                else
                    r = 0;

                s[i].y += r;
                s[i].win=newwin(s[i].h, s[i].w, s[i].y, s[i].x);
                sum[i]=s[i].y;
                if(s[i].y > 40) { 
                    s[i].y = 40; 
                    strcat(s[i].name, rank[k]);
                    k++;
                }

            }
            if(sum[i] > 40) {
                sum[i] = 40;
            }
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tids[i], &attr, foo, &s[i]);
            usleep(500);
            pthread_create(&tids[num], &attr, type, &s[8]);
            usleep(500);
        }
        sleep(1);
    }

    for(int i=0; i<num; i++) { 
        pthread_join(tids[i], NULL); 
    }
    sem_unlink((char*)&mutex);


    /* refresh(); */
    int ch;
    /* printw("Enter"); */
    while( (ch=wgetch(s[0].win)) != '\n') ;
    endwin();

    return 0;
}


void *foo (void *v) {
    sem_wait(&mutex);
    struct horse *s = (struct horse *)v;
    box(s->win, 0, 0);
    mvwprintw(s->win, 1, 1, "%s", s->name);
    wrefresh(s->win);
    werase(s->win);

    sem_post(&mutex);
    pthread_exit(0);
}

void *type(void *v) {
    sem_wait(&mutex);
    struct horse *s = (struct horse *)v;
    box(s->win, 0, 0);
    int ch=0;
    while( (ch=wgetch(s->win)) != '\n') { 
        switch (ch) 
        {
            case 27:
                werase(s->win);
                wrefresh(s->win);
                return NULL;
                break;
            case 127: case KEY_BACKSPACE:
                mvwprintw(s->win, y, --(x), " \b");
                mvwprintw(s->win, y-2, x, " \b");
                i--;
                break;
            default:
                temp[i]=ch;
                mvwprintw(s->win, y, x++, "%c", temp[i]);
                i++;
        }
        if(i<0) i=0;
        if(x<2) x=2;
        mvwprintw(s->win, y, x, "");
    }
    if(ch == '\n')
        i=0;
        
    werase(s->win);
    wrefresh(s->win);
    pthread_exit(0);
}
