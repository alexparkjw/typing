/* #include<stdio.h> */
#include<stdlib.h> 
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<semaphore.h>
#include<ncurses.h>
#include<unistd.h>

#define HEIGHT 44
#define WIDTH 120
#define SIZE_N 60
#define wwp(k) newwin(k->h,k->w,k->y,k->x)
#define ww(k) newwin(k.h,k.w,k.y,k.x)
#define rr(k) rand()%(k)

sem_t mutex;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

typedef struct Vector {
    int o, h, w, y, x;
    WINDOW *win;
    char **data;
    int len;
}vector;

char *strDup(const char *str);
int strCmp(void *vp1, void *vp2);
bool match(char *first, char *second);

void print_menu(vector *, int );
void print_basic(vector * );
int set_basic(WINDOW *,  char *, int );
int print_words(vector * );
int print_sentence(vector * );
int print_graph(vector *, int *);
int print_game(vector *);
void *rain(void *);
void *type(void *);
void option_menu(vector *, int **);
int set_option(vector *); 
int cline(FILE *);
vector * fileToData(char *, vector *, int, int, int, int );
void load_option(char *filename, int **op);
void save_option(char *filename, int **op);


int main(void) { 
    srand((unsigned)time(NULL)*getpid());
    initscr();
    // combine vector
    vector *menu=NULL, *basic=NULL, *words=NULL, *game=NULL;
    vector *sentence=NULL, *paragraph=NULL, *option=NULL;
    menu = fileToData("menu.txt", menu, 4, 4, (HEIGHT - HEIGHT/4)/2, (WIDTH - WIDTH/4)/2);
    basic = fileToData("basic.txt", menu, 4, 2, (HEIGHT - HEIGHT/4)/2, (WIDTH - WIDTH/2)/2);
    words = fileToData("words.txt", words, 6, 1, (HEIGHT - HEIGHT/6)/2, (WIDTH - WIDTH/1)/2);
    game = fileToData("words.txt", game, 1, 1, (HEIGHT - HEIGHT/1)/2, (WIDTH - WIDTH/1)/2);
    sentence = fileToData("short.txt", sentence, 6, 1, (HEIGHT - HEIGHT/6)/2, (WIDTH - WIDTH/1)/2);
    paragraph = fileToData("paragraph.txt", paragraph, 2, 1, (HEIGHT - HEIGHT/2)/2, (WIDTH - WIDTH/1)/2);
    option = fileToData("option.txt", option, 4, 4, (HEIGHT - HEIGHT/4)/2, (WIDTH - WIDTH/4)/2);

    int *op[]= { &basic->o, &words->o, &sentence->o, &paragraph->o , &game->o};
    load_option("option.conf", op);

    // WINDOW 
    clear();
    cbreak();
    noecho();
    keypad(menu->win, TRUE);

    int c = 0, l=0, highlight = 1;
    while(1) {
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
                    print_game(game);
                    /* cbreak(); */
                    werase(game->win);
                    wrefresh(game->win);
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

    return 0;
}


bool match(char *first, char * second)  { 
    if (*first == '\0' && *second == '\0') 
        return true; 
    if (*first == '*' && *(first+1) != '\0' && *second == '\0') 
        return false; 
    if (*first == '?' || *first == *second) 
        return match(first+1, second+1); 
    if (*first == '*') 
        return match(first+1, second) || match(first, second+1); 
    return false; 
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

int print_game(vector *game ) {
    box(game->win, 0, 0);
    wrefresh(game->win);

    /* sem_open((char*)&mutex, 0, 1); */
    /* sem_init(&mutex, 0, 1); */

    vector *s[6], *kb;
    for(int i=0; i<6; i++) {
        s[i] = fileToData("words.txt", s[i], 16, 10, 1, 1);
    }

    kb = fileToData("words.txt", kb, 10, 10, 1, 1);
    pthread_t tids_key;
    pthread_attr_t attr_key;
    pthread_attr_init(&attr_key);
    pthread_create(&tids_key, &attr_key, type, s[0]);
    pthread_join(tids_key, NULL);

    /* pthread_t tids[6]; */
    /* pthread_attr_t attr[6]; */

    /* for(int i=0; i<6; i++) { */
        /* pthread_attr_init(&attr[i]); */
        /* pthread_create(&tids[i], &attr[i], rain, s[i]); */
        /* usleep(1100000); */
        /* sleep(2); */
        /* pthread_join(tids[i], NULL); */
    /* } */

    /* pthread_join(tids, NULL); */
    /* sem_unlink((char*)&mutex); */

    /* pthread_join(tids, NULL); */
    /* sem_unlink((char*)&mutex); */
    /* sem_destroy(&mutex); */

    int ch;
    while( (ch=wgetch(game->win)) != '\n') ;

    werase(game->win);
    wrefresh(game->win);
    return 0;
}

void *type(void *v) {
    /* sem_wait(&mutex); */
    vector *game = (vector *)v;
    box(game->win, 0, 0);
    char temp[150];
    int  ch, ii=0, y=30, x=30;
    while( (ch=wgetch(game->win)) ) {
        switch (ch) 
        {
            case 27:
                werase(game->win);
                wrefresh(game->win);
                pthread_exit(0);
                break;
            case 127: case 8: case KEY_BACKSPACE:
                mvwprintw(game->win, y, --x, " \b");
                ii--;
                break;
            default:
                temp[ii]=ch;
                mvwprintw(game->win, y, x++, "%c", temp[ii]);
                ii++;
        }
        if(ii<0) ii=0;
        if(x<30) x=30;
    }
    temp[ii]='\0';
    
    werase(game->win);
    wrefresh(game->win);

    /* sem_post(&mutex); */
    pthread_exit(0);
}

void *rain(void *w) {
    /* sem_wait(&mutex); */
    /* pthread_mutex_lock(&mutex1); */
    vector *s = (vector *)w;
    s->x = rand()%(WIDTH-20)+1;
    int rnum=rand()%s->len;
    char intput[100];
    while(s->y < HEIGHT - s->h - 1) {
        werase(s->win);
        wrefresh(s->win);
        s->w = strlen(s->data[rnum])+2;
        s->win=wwp(s);;
        /* box(s->win, 0, 0); */
        mvwprintw(s->win, 1, 1, "%s", s->data[rnum]);
        wrefresh(s->win);
        s->y++;
        usleep(300000);

    }
    werase(s->win);
    wrefresh(s->win);
    s->y = 1;

    /* sem_post(&mutex); */
    /* pthread_mutex_unlock(&mutex1); */
    pthread_exit(0);
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

char * strDup(const char * str){
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

int cline(FILE *fp) {
    int count = 0;
    for(char tmp = getc(fp); tmp !=EOF; tmp =getc(fp))
        if(tmp == '\n')
            count++;
    rewind(fp);
    return count;
} 

vector * fileToData(char *filename, vector *v, int h, int w, int y, int x) {
    v = (vector*)malloc(sizeof(vector));
    FILE *fp=fopen(filename, "r");
    if(fp == NULL) {
        printf("%s file cannot open \n", filename);
        return NULL;
    }

    v->len=cline(fp);
    v->data=(char**)malloc(sizeof(char*)*(v->len));

    char temp[150];
    for(int i=0; i < v->len; i++) {
        fscanf(fp, "%[^\n] ", temp);
        v->data[i] = strDup(temp);
    }
    v->h = HEIGHT/h;
    v->w = WIDTH/w;
    v->y = y;
    v->x = x;
    /* v->y = (HEIGHT - v->h)/2; */
    /* v->x = (WIDTH - v->w)/2; */
    /* (HEIGHT - v->h)/2, (WIDTH - v->w)/2; */
    v->win = newwin(v->h, v->w, v->y, v->x);

    fclose(fp);
    return v;
}

void load_option(char *filename, int **op) {
    FILE *fp=fopen(filename, "r");
    if(fp == NULL) {
        printf("%s file cannot open \n", filename);
        return ;
    }
    for(int i=0; i<5; i++ )
        fscanf(fp, " %d", op[i]);

    fclose(fp);
}

void save_option(char *filename, int **op) {
    FILE *fp=fopen(filename, "w+");
    if(fp == NULL) {
        printf("%s file cannot open \n", filename);
        return ;
    }
    for(int i=0; i<5; i++ )
        fprintf(fp, "%d\n", *op[i]);

    fclose(fp);
}

