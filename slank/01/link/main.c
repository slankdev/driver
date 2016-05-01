#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;

struct node {
    uint8_t *addr;
    size_t   len;
    struct node *next;
};
/* struct slank_device { */
/*     struct node *head; */
/* }; */




struct node *add_node(size_t size)
{
    struct node *node = malloc(sizeof(struct node));
    node->len  = size;
    node->addr = malloc(node->len);
    node->next = NULL;

    return node;
}


struct node* get_tail(struct node *node)
{
    if (!node) return NULL;

    if (node->next)
        return get_tail(node->next);
    else
        return node;
}


void add_tail(struct node* head, size_t size)
{
    if (head) {
        struct node* tail = get_tail(head);
        tail->next = add_node(size);
    } else {
        head = add_node(size);
    }
}


void rm_tail(struct node *head)
{
    if (!head) return;

    if (head->next) {
        rm_tail(head->next);
    } else {
        free(head->addr);
        free(head);
        head = NULL;
    }
}   






// size_t depth = get_depth(head, 0);
size_t get_depth(struct node* head, size_t count)
{  
    if (!head) return 0;

    count++;
    if (head->next) return get_depth(head->next, count);
    else return count;
}




int main()
{
    struct node *head;
    add_tail(head, 10);
    add_tail(head, 10);
    /* head = add_node(100); */
    /* if (get_tail(head)) get_tail(head)->next = add_node(10); */
    printf("depth is %zd \n", get_depth(head,0));

    
    return 1;


}



