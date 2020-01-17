#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define newline printf("\n")

void *search(void *, void *, size_t, int, int (*fn)(void*, void*));
int intCmp(void *, void *);
int strCmp(void *, void *);

int main() {
    char *str[] = {"apple", "banana", "orange", "kiwi", "mango", "plum"};
    int size_str = sizeof(str)/sizeof(char*);

    const char *key_str = "kiwi";
    /* char *key_str = strdup("kiwi"); */

    char **found_str = search(&key_str, str, size_str, sizeof(char*), strCmp);
    if(found_str == NULL) 
        printf("not found\n");
    else 
        printf("str[%ld] = \"%s\"\n", found_str-str, *found_str);

    return 0;
}


void *search(void *key, void *base, size_t N, int sizeType, int (*fn)(void*, void*)) {
    for(int i=0; i<N; i++) {
        void *addr = (char *)base + i*sizeType;
        if(fn(key, addr) == 0 ) 
            return addr;	
    }
    return NULL;
}

int strCmp(void *vp1, void *vp2){
    char * s1 = (char *)vp1;
    char * s2 = (char *)vp2;
    return strcmp(s1, s2);
}

int intCmp(void *elem1, void *elem2) {
    int *ip1 = (int *)elem1;
    int *ip2 = (int *)elem2;
    return *ip1-*ip2;
}
