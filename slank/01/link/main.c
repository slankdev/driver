#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;

struct node {
    uint8_t *addr;
    size_t   len;
    struct node *next;
};

struct slank_device {
    struct node *head;
};



static struct node *alloc_node(size_t size)
{
    struct node *node = malloc(sizeof(struct node));
    node->len  = size;
    node->addr = malloc(node->len);
    node->next = NULL;
    return node;
}

static struct node* get_tail(struct slank_device* dev)
{
    struct node* n = dev->head;
    while (1) {
        if (n) {
            if (n->next)
                n = n->next;
            else 
                break;
        } else {
            break;
        }
    }
    return n;
}

void init_device(struct slank_device* dev)
{
    dev->head = NULL;
}

void print_device(struct slank_device* dev)
{
    struct node* h = dev->head;
    printf("device[");
    while (1) {
        if (h) {
            printf("%d -> ", h->len);
            h = h->next;
        } else {
            printf("null]\n");
            break;
        }
    }
}

size_t get_depth(struct slank_device* dev)
{  
    size_t i;
    struct node* h = dev->head;
    for (i=0; ; i++) {
        if (h)
            h = h->next;
        else
            break;
    }
    return i;
}

void add_tail(struct slank_device* dev, size_t len)
{
    struct node* tail = get_tail(dev);
    if (tail) {
        tail->next = alloc_node(len);
    } else {
        dev->head = alloc_node(len);
    }
}


void rm_head(struct slank_device* dev)
{
    if (dev->head) {
        struct node* next = dev->head->next;
        free(dev->head->addr);
        free(dev->head);
        dev->head = next;
    }
    return;
}


int main()
{
    struct slank_device dev;
    init_device(&dev);

    add_tail(&dev, 10);
    add_tail(&dev, 30);
    rm_head(&dev);
    add_tail(&dev, 20);

    print_device(&dev);
    printf("depth is %zd \n", get_depth(&dev));
    return 0;
}



