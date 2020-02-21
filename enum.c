#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct class {
    enum { 
        Apple, Akee, Apricot, Avocado, Banana, Bilberry, Blackberry, 
        Blackcurrant, Black_sapote, Blueberry, Boysenberry, Buddhas_hand, 
        fingered_citron, Crab_apples, Currant, Cherry, Cherimoya, Custard_Apple, 
        Chico_fruit, Cloudberry, Coconut, Cranberry, Cucumber, Damson, Date, 
        Dragonfruit, Pitaya, Durian, Elderberry, Feijoa, Fig, Goji_berry, 
        Gooseberry, Grape, Raisin, Grapefruit, Guava, Honeyberry, Huckleberry, 
        Jabuticaba, Jackfruit, Jambul, Japanese_plum, Jostaberry, Jujube, 
        Juniper_berry, Kiwano, horned_melon, Kiwifruit, Kumquat, Lemon, Lime, 
        Loquat, Longan, Lychee, Mango, Mangosteen, Marionberry, Melon, Cantaloupe, 
        Honeydew, Watermelon, Miracle_fruit, Mulberry, Nectarine, Nance, Olive, 
        Orange, Blood_orange, Clementine, Mandarine, Tangerine, Papaya, Passionfruit, 
        Peach, Pear, Persimmon, Plantain, Plum, Prune, dried_plum, Pineapple, 
        Pineberry, Plumcot, Pluot, Pomegranate, Pomelo, Purple_mangosteen, Quince, 
        Raspberry, Salmonberry, Rambutan, Mamin_Chino, Redcurrant, Salal_berry, 
        Salak, Satsuma, Soursop, Star_apple, Star_fruit, Strawberry, Surinam_cherry, 
        Tamarillo, Tamarind, Ugli_fruit, White_currant, White_sapote, Yuzu 
    } index;
    void (*set)(int);
    void (*detail)();
}class; *this=NULL;

void set(int index );
void detail();

#define ELEVENTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11
#define COUNT_ARGUMENTS(...) ELEVENTH_ARGUMENT(dummy, ## __VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define CONCAT(a, b) a ## b
#define CONCAT2(a, b) CONCAT(a, b)
#define CLASS(...) CONCAT2(CLASS_, COUNT_ARGUMENTS(__VA_ARGS__))(FORMAT, ##__VA_ARGS__)

#define CLASS_0(FORMAT) class *this=NULL, *curr=NULL;
#define CLASS_1(FORMAT, ARG1)  class (ARG1); this=&(ARG1); this->set=set; this->detail=detail
#define CLASS_2(FORMAT, ARG1, ARG2) class ARG1[ARG2]; \
    for(int i=0; i<ARG2; i++) { this = &ARG1[i]; this->set=set; this->detail=detail; } 
CLASS();

char **fruits=NULL;
int len_func(FILE *fp);
char **save_func(FILE *fp, int (*fn)(FILE *));

int main(void) {
    srand(time(NULL));
    FILE *fp = fopen("./text/fruits.txt", "r");
    if(fp == NULL) printf("The file,  cannot be opened!...\n");
    fruits = save_func(fp, len_func);
    int len = len_func(fp);
    fclose(fp);

    CLASS(a);
    CLASS(b);
    CLASS(c);

    a.set(Apple);
    b.set(Banana);
    c.set(Cherry);

    this = &a;
    a.detail();
    this = &b;
    b.detail();
    this = &c;
    c.detail();

    return 0;
}

void set(int index) { this->index = index; }
void detail() { printf("%5d : %s\n\n", this->index, fruits[this->index]); }

int len_func(FILE *fp) {
    fseek(fp, 0, SEEK_SET); /* rewind(fp) */
    int len = 0;
    char buff[512];
    while(fgets(buff, sizeof(buff) - 1, fp) != NULL) { len++; }
    return len;
}

char **save_func(FILE *fp, int (*fn)(FILE *)) {
    int len = fn(fp);
    fseek(fp, 0, SEEK_SET); /* rewind(fp) */
    char buffer[512];
    char  **arr = (char**)malloc(sizeof(char*)*len);
    for (int i = 0; i<len; i++) {
        fgets(buffer, sizeof(buffer)-1 , fp);
        buffer[strcspn(buffer, "\n")] = 0;
        arr[i] = strdup(buffer);
    }
    return arr;
}

