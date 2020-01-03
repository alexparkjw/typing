#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<time.h>

#define random_range 99

typedef struct Node { 
    int data; 
    struct Node *next; 
}node; 

node* new_node(node* head, int data);
node* add_node(node* head, int data);
node* del_head(node* head, int data);
void del_body(node* prev, node* curr, int data);
int sp(int size);
void print_node(node *head); 
node* index_node(node* head, int n);
void swap(void *vp1, void *vp2, int size);
void merge(node *head, int low, int high, int dir);
void sort(node *head, int low, int high, int dir);
void bit_sort(node **head, int len, int dir);

int main(void) { 
    srand(time(NULL));
    node *head = NULL;

    int len = 10;
    for(int i=0; i<len; i++) { 
        head = add_node(head, rand()%random_range+1);
    }

    printf("random node: ");
    print_node(head); 
    printf("\n");

    printf("sorted up  : ");
    bit_sort(&head, len, 1); 
    print_node(head); 
    printf("\n");

    printf("sorted down: ");
    bit_sort(&head, len, 0); 
    print_node(head); 
    printf("\n");
        
    return 0; 
}

node* new_node(node *head, int data) {
    node *new = (node*)malloc(sizeof(node));
    new->data = data;
    new->next = NULL;
    return new;
}

node* add_node(node *head, int data) {
    if(head == NULL)
        head = new_node(head, data);
    else 
        head->next = add_node(head->next, data);
    return head;
}

node* del_head(node* head, int data) {
    if(head == NULL) return head;
    if(head->data == data) {
        return head->next;
    }
    del_body(head, head->next,  data);
    return head;
}

void del_body(node* prev, node* curr,int data) {
    if(curr == NULL) return;
    if(curr->data == data) {
        if(curr->next != NULL) 
            prev->next = curr->next;
        else 
            prev->next = NULL;
        free(curr);
        return ;
    }
    del_body(curr, curr->next,  data);
}

int sp(int size) {
    int rg=1;;
    while(size) { size/=10; rg++; }
    return rg;
}

void print_node(node *head) {
    if(head != NULL) {
        printf(" % *d ",sp(random_range), head->data);
        print_node(head->next);
    }
    else printf("\n");
}

node* index_node(node* head, int n) {
    for(int i=0; i<n; i++) 
        head = head->next;
    return head;
}

void swap(void *vp1, void *vp2, int size) {
    char buffer[size];
    memcpy(buffer, vp1, size);
    memcpy(vp1, vp2, size);
    memcpy(vp2, buffer, size);
}

void merge(node *head, int low, int high, int dir) { 
    int mid = low+(high-low)/2; 
    if (mid > low) { 
        for (int i=low; i<mid; i++) {
            node *x = index_node(head, i);          
            node *y = index_node(head, mid-low+i);
            if ( dir == (x->data > y->data) ) {
                swap(x, y, sizeof(node*)); 
            }
        }
        merge(head, low, mid, dir); 
        merge(head, mid, high, dir); 
    } 
} 

void sort(node *head, int low, int high, int dir) { 
    int mid = low+(high-low)/2; 
    if (mid > low) { 
        sort(head, low, mid, 1); 
        sort(head, mid, high, 0); 
        merge(head, low, high, dir); 
    } 
} 

void bit_sort(node **head, int len, int dir) {
    int k=2; while( (k*=2) < len) ;
    for(int i=len; i<k; i++) *head = add_node(*head, 0);
    sort(*head, 0, k, dir); 
    for(int i=len; i<k; i++) *head = del_head(*head, 0);
}








