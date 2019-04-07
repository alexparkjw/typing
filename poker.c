#include<locale.h>
#include<wchar.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<time.h>

#define run 1000

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

int is_flush(deck *p) {
    wchar_t shape[4] = {0x2664, 0x2662, 0x2661, 0x2667};
    deck *curr = p;
    int res[5]={0,};
    for(int i=0; i<7; i++) {
        if(curr->num >= 39) res[4]++;
        else if(curr->num >= 26) res[3]++;
        else if(curr->num >= 13) res[2]++;
        else if(curr->num >= 0 ) res[1]++;
        else  res[0]++;
        curr = curr->next;
    }    
    for(int i=1; i<5; i++) if(res[i] >= 5) return shape[i];
    return 0;
}

int is_straight(deck *p) {
    deck *curr = p;
    int res[14]={0,};
    for(int i=0; i<7; i++) {
        res[i]=curr->num%13+1;
        if(res[i] == 1) 
            res[7+i] = 14;
        curr = curr->next;
    }
    
    for(int i=0; i<14; i++) {
        for(int j=0; j<14; j++) {
            if(res[i]==res[j]) res[j] = -1;
            if(res[i]>res[j]) swap(&res[i], &res[j], sizeof(int));
        }
    }

    for(int i=0, k=0; i<11; i++) {
        k=0;
        for(int j=i; j<4; j++) {
            if(res[j]  == res[j+1] + 1) k++;
            if(k==4) {
                if(res[i] == 14)
                    return 1;
                return res[i];
            }
        }
    }
    return 0;
}

int is_pair(deck *p, int n) {
    deck *curr = p;
    int res[13]={0,};
    for(int i=0; i<7; i++) {
        res[curr->num%13]++;
        curr = curr->next;
    }    

    if(res[0]==n) return 1;
    for(int i=12; i>=1; i--) if(res[i] == n) return i+1;
    return 0;
}

int is_onepair(deck *p) { return is_pair(p, 2); }
int is_threecard(deck *p) { return is_pair(p, 3); }
int is_fourcard(deck *p) { return is_pair(p, 4); }

int is_twopair(deck *p) {
    deck *curr = p;
    int res[13]={0,};
    for(int i=0; i<7; i++) {
        res[curr->num%13]++;
        curr = curr->next;
    }    

    int a=0, b=0, k=0;
    if(res[0] == 2) {
        k++;
        a = 1;
    }
    for(int i=12; i>=1; i--) {
        if(res[i] == 2) {
            k++;
            if(k==1) a = i+1;
            if(k==2) b = i+1;
        }
    }

    return a*100+b;
}

int is_hicard(deck *p) {
    deck *curr = p;
    int high=0;
    for(int i=0, t; i<2; i++) {
        t = curr->num%13+1;
        if(t == 1) return 1;
        if(high<t) high = t;
        curr = curr->next;
    }    
    return high;
}


int main(void) { 
    char *order[] = { "0", "A", "2", "3", "4", "5", 
        "6", "7", "8", "9", "10", "J", "Q", "K" };
    srand(time(NULL));
    char *locale = setlocale(LC_ALL, "");
    int players = 6;
    deck *card = NULL; 
    set_trump(&card);
    /* print_deck(card); */
    deck **player = (deck **)malloc(sizeof(deck*)*players);
    deck *flop = NULL, *curr = NULL;

    int res[10]={0,};
    for(int k=0; k<run; k++) {
        shuffle(&card, 1000);    
        /* print_deck(card); */
        curr = card;
        for(int i=0; i<players; i++) 
            player[i] = (deck *)malloc(sizeof(deck));

        for(int i=0; i<players; i++) {
            player[i] = NULL;
            for(int j=0; j<2; j++) {
                curr = curr->next;
                player[i] = add_deck(player[i], curr->num);
             }
        }

        flop = NULL;
        for(int i=0; i<5; i++) {
            curr = curr->next;
            flop = add_deck(flop, curr->num);
        }


        for(int i=0,on=0,to=0,th=0,fo=0,fl=0,st=0; i<players; i++) {
            printf("player #%d : ", i+1);
            print_deck(player[i]);
            player[i]->next->next = flop;
            st = is_straight(player[i]);
            fl = is_flush(player[i]);
            fo = is_fourcard(player[i]);
            th = is_threecard(player[i]);
            on = is_onepair(player[i]);
            to = is_twopair(player[i]);
            to = to?to%(on*100):0;


            if(fo > 0 && st > 0) {
                printf("Straight flush (%s)\n", order[fo]);
                res[1]++;
            }
            else if(fo > 0) { 
                printf("Four card (%s)\n", order[fo]);
                res[2]++;
            }
            else if(th > 0 && to > 0) {
                printf("Full house (%s, %s)\n", order[th], order[to]);
                res[3]++;
            }
            else if(fl > 0) {
                wprintf(L"Flush (%lc)\n", fl);
                res[4]++;
            }
            else if(st > 0) {
                printf("Straight (%s)\n", order[st]);
                res[5]++;
            }
            else if(th > 0 ) {
                printf("Three card (%s)\n", order[th]);
                res[6]++;
            }
            else if(on>0 && to>0) {
                printf("Two pair (%s %s)\n", order[on], order[to]);
                res[7]++;
            }
            else if(on>0 || to>0) {
                printf("One pair (%s)\n", on?order[on]:order[to]);
                res[8]++;
            }
            else {
                printf("high card (%s)\n", order[is_hicard(player[i])] );
                res[9]++;
            }
        }
        printf("#%d : FLop : ", k+1);
        print_deck(flop);
        printf("\n\n");
        /* int *d=(int*)malloc(sizeof(int)*5); */        
        /* combi(flop, d, 0, 4, 0, 3); */
        for(int i=0; i<players; i++) free(player[i]);
    }


    char *hand[]={"Royal flush", "Straight flush", "Four of a kind", "Full house", 
        "Flush", "Straight", "Three of kind", "Two pair", "One pair", "High card"};
    for(int i=0; i<10; i++) {
        printf("%15s = %5d / %d = %.5lf%%\n", 
        hand[i], res[i], run*players, (double)res[i]*100/(run*players));
    }

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
            wprintf(L"(%lc", 0x2667);
        else if(card->num >= 26)
            wprintf(L"(%lc", 0x2661);
        else if(card->num >= 13)
            wprintf(L"(%lc", 0x2662);
        else
            wprintf(L"(%lc", 0x2664);
        int k = card->num%13;
        if(k == 0)
            printf("%3c)", 'A');
        else if(k == 10)
            printf("%3c)", 'J');
        else if(k == 11)
            printf("%3c)", 'Q');
        else if(k == 12)
            printf("%3c)", 'K');
        else
            printf(" %2d)", k+1);
        printf("  ");
        print_deck(card->next);
    }
    else printf(" : ");
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
        *card = add_deck(*card, i);
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
