#include<locale.h>
#include<wchar.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<time.h>

typedef struct Deck{
    int num;
    struct Deck *next;
}deck;

deck* new_deck(deck* card, int num);
deck* add_deck(deck* card, int num);
deck* del_deck(deck* card, int num);
void del_body(deck* prev, deck* curr, int num);
int sp(int size);
void print_deck(deck *card); 
void print_array(int *arr, int len);
deck* index_deck(deck* card, int n);
void swap(void *vp1, void *vp2, int size);
void merge(deck *card, int low, int high, int dir);
void sort(deck *card, int low, int high, int dir);
void bit_sort(deck **card, int len, int dir);
char* strDup(const char* str);
int strCmp(void * vp1, void * vp2);
void set_trump(deck **card);
void shuffle(deck **card, int ea);

void combi(deck* a, int *d, int ss,int ee,int index,int r) {
    if (index == r) {
        print_array(d, r);
        return;
    }
    for (int i=ss; i<=ee && ee-i+1 >= r-index; i++) {
        deck *t = index_deck(a, i);
        d[index] = t->num;
        combi(a, d, i+1, ee, index+1, r);
    }
}

int is_paire(deck *p) {
    int arr[7];
    deck *curr = p;
    for(int i=0; i<7; i++) {
        arr[i] = curr->num;
        curr = curr->next;
    }    

}
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
        player[i] = NULL;
        for(int j=0; j<2; j++) {
            curr = curr->next;
            player[i] = add_deck(player[i], curr->num);
        }
    }
    for(int i=0; i<5; i++) {
        curr = curr->next;
        flop = add_deck(flop, curr->num);
    }

    printf(" List : \n");
    for(int i=0; i<players; i++) 
        print_deck(player[i]); 
    printf(" FLop : \n");
    print_deck(flop);



    //
    for(int i=0; i<players; i++)
        player[i]->next->next = flop;


    /* int *d=(int*)malloc(sizeof(int)*5); */        
    /* combi(flop, d, 0, 4, 0, 3); */


    for(int i=0; i<players; i++)
        free(player[i]);
    free(player);
    return 0; 
}

deck* new_deck(deck *card, int num) {
    deck *new = (deck*)malloc(sizeof(deck));
    new->num = num;
    new->next = NULL;
    return new;
}

deck* add_deck(deck *card, int num) {
    if(card == NULL) card = new_deck(card, num);
    else card->next = add_deck(card->next, num);
    return card;
}

deck* del_deck(deck* card, int num) {
    if(card == NULL) return card;
    if(card->num == num ) {
        deck *temp = card->next;
        free(card);
        return temp;
    }
    del_body(card, card->next, num);
    return card;
}

void del_body(deck* prev, deck* curr, int num) {
    if(curr == NULL) return;
    if(curr->num == num ) {
        prev->next = curr->next;
        free(curr);
        return ;
    }
    del_body(curr, curr->next, num);
}

int sp(int size) {
    int rg=1;
    while(size) { size/=10; rg++; }
    return rg;
}

void print_array(int *arr, int len) {
    for(int i=0; i<len; i++) 
        printf("%d \n", arr[i]);
    printf("\n");

}
void print_deck(deck *card) {
    /* wchar_t shape[4] = {0x2664, 0x2662, 0x2661, 0x2667}; */
    if(card != NULL) {
        if(card->num >= 39)
            wprintf(L"%lc", 0x2667);
        else if(card->num >= 26)
            wprintf(L"%lc", 0x2661);
        else if(card->num >= 13)
            wprintf(L"%lc", 0x2662);
        else
            wprintf(L"%lc", 0x2664);

        if(card->num%13 == 0)
            printf(" %2c\n", 'A');
        else if(card->num%13 == 10)
            printf(" %2c\n", 'J');
        else if(card->num%13 == 11)
            printf(" %2c\n", 'Q');
        else if(card->num%13 == 12)
            printf(" %2c\n", 'K');
        else
            printf(" %2d\n", card->num%13+1);
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
    for(int i=len; i<k; i++) *card = add_deck(*card, 0);
    sort(*card, 0, k, dir); 
    for(int i=len; i<k; i++) *card = del_deck(*card, 0);
}

char* strDup(const char* str){
    int n = strlen(str) + 1;
    char *dup = (char *)malloc(sizeof(char)*n);
        strcpy(dup, str);
    return dup;
}

int strCmp(void * vp1, void * vp2) {
    char* s1 = (char *)vp1;
    char* s2 = (char *)vp2;
    return strcmp(s1, s2);
}

void set_trump(deck **card) {
    for(int i=0; i<52; i++) {
        *card = add_deck(*card, i+1);
    }
}

void shuffle(deck **card, int ea) {
    int r1;
    for(int i=0; i<ea; i++) {
        r1 = rand()%52;
        *card = del_deck(*card, r1);
        *card = add_deck(*card, r1);
    } 
}

