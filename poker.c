#include<locale.h>
#include<wchar.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<time.h>

typedef struct Deck{
    wchar_t suit;
    int num;
    struct Deck *next;
}deck;

deck* new_deck(deck* card, wchar_t suit, int num);
deck* add_deck(deck* card, wchar_t suit, int num);
deck* del_deck(deck* card, wchar_t suit, int num);
void del_body(deck* prev, deck* curr, wchar_t suit, int num);
int sp(int size);
void print_deck(deck *card); 
deck* index_deck(deck* card, int n);
void swap(void *vp1, void *vp2, int size);
void merge(deck *card, int low, int high, int dir);
void sort(deck *card, int low, int high, int dir);
void bit_sort(deck **card, int len, int dir);
char* strDup(const char* str);
int strCmp(void * vp1, void * vp2);
void set_trump(deck **card);
void shuffle(deck **card, int ea);

int main(void) { 
    srand(time(NULL));
    char *locale = setlocale(LC_ALL, "");
    deck *card = NULL;

    set_trump(&card);
    shuffle(&card, 1000);    
    /* print_deck(card); */

    int players = 6;
    deck **player = (deck **)malloc(sizeof(deck*)*players);
    deck *flop;
    deck *curr = card;
    for(int i=0; i<players; i++) {
        player[i] = (deck *)malloc(sizeof(deck));
        for(int j=0; j<2; j++) {
            curr = curr->next;
            player[i] = add_deck(player[i], curr->suit, curr->num);
        }
    }
    for(int i=0; i<3; i++) {
        curr = curr->next;
        flop = add_deck(flop, curr->suit, curr->num);
    }

    printf(" list : \n");
    for(int i=0; i<players; i++) 
        print_deck(player[i]); 
    print_deck(flop);
        

    for(int i=0; i<players; i++)
        free(player[i]);
    free(player);
    return 0; 
}

deck* new_deck(deck *card, wchar_t suit, int num) {
    deck *new = (deck*)malloc(sizeof(deck));
    new->num = num;
    new->suit = suit;
    new->next = NULL;
    return new;
}

deck* add_deck(deck *card, wchar_t suit, int num) {
    if(card == NULL) card = new_deck(card, suit, num);
    else card->next = add_deck(card->next, suit, num);
    return card;
}

deck* del_deck(deck* card, wchar_t suit, int num) {
    if(card == NULL) return card;
    if(card->num == num && card->suit == suit) {
        deck *temp = card->next;
        free(card);
        return temp;
    }
    del_body(card, card->next, suit, num);
    return card;
}

void del_body(deck* prev, deck* curr, wchar_t suit, int num) {
    if(curr == NULL) return;
    if(curr->num == num && curr->suit == suit) {
        prev->next = curr->next;
        free(curr);
        return ;
    }
    del_body(curr, curr->next, suit, num);
}

int sp(int size) {
    int rg=1;
    while(size) { size/=10; rg++; }
    return rg;
}

void print_deck(deck *card) {
    if(card != NULL) {
        wprintf(L"%lc", card->suit);
        printf(" %2d\n", card->num);
        print_deck(card->next);
    }
    else printf("\n\n");
}

deck* index_deck(deck* card, int n) {
    for(int i=0; i<n; i++) 
        card = card->next;
    return card;
}

void swap(void *vp1, void *vp2, int size) {
    char buffer[size];
    memcpy(buffer, vp1, size);
    memcpy(vp1, vp2, size);
    memcpy(vp2, buffer, size);
}

void merge(deck *card, int low, int high, int dir) { 
    int mid = low+(high-low)/2; 
    if (mid > low) { 
        for (int i=low; i<mid; i++) {
            deck *x = index_deck(card, i);          
            deck *y = index_deck(card, mid-low+i);
            if ( dir == (x->num > y->num) ) {
                swap(x, y, sizeof(deck*)); 
            }
        }
        merge(card, low, mid, dir); 
        merge(card, mid, high, dir); 
    } 
} 

void sort(deck *card, int low, int high, int dir) { 
    int mid = low+(high-low)/2; 
    if (mid > low) { 
        sort(card, low, mid, 1); 
        sort(card, mid, high, 0); 
        merge(card, low, high, dir); 
    } 
} 

void bit_sort(deck **card, int len, int dir) {
    int k=2; while( (k*=2) < len) ;
    for(int i=len; i<k; i++) *card = add_deck(*card, 0x2668, 0);
    sort(*card, 0, k, dir); 
    for(int i=len; i<k; i++) *card = del_deck(*card, 0x2668, 0);
}

char* strDup(const char* str){
    int n = strlen(str) + 1;
    char *dup = (char *)malloc(sizeof(char)*n);
    if (dup)
        strcpy(dup, str);
    return dup;
}

int strCmp(void * vp1, void * vp2) {
    char* s1 = (char *)vp1;
    char* s2 = (char *)vp2;
    return strcmp(s1, s2);
}

void set_trump(deck **card) {
    wchar_t s_name[4] = {0x2664, 0x2662, 0x2661, 0x2667};
    for(int i=0; i<4; i++) {
        for(int j=0; j<13; j++) {
            *card = add_deck(*card, s_name[i], j%13+1);
        }
    }
}

void shuffle(deck **card, int ea) {
    wchar_t s_name[4] = {0x2664, 0x2662, 0x2661, 0x2667};
    int r1, r2;
    for(int i=0; i<ea; i++) {
        r1 = rand()%4;
        r2 = rand()%13+1;
        *card = del_deck(*card, s_name[r1], r2);
        *card = add_deck(*card, s_name[r1], r2);
    } 
}

