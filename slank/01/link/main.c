#include <stdio.h>
#include <string.h>
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

static struct node *alloc_node(size_t size);
static struct node* get_tail(struct slank_device* dev);
static void add_tail(struct slank_device* dev, size_t len);
static void rm_head(struct slank_device* dev);


void print_device(struct slank_device* dev);
size_t get_depth(struct slank_device* dev);

void slank_open(struct slank_device* dev)
{
    dev->head = NULL;
}

void slank_close(struct slank_device* dev)
{
    while (1) {
        if (dev->head == NULL)
            break;
        rm_head(dev);
    }
}

ssize_t slank_read(struct slank_device* dev, void* buf, size_t nbyte)
{
    struct node* n = dev->head;
    if (n) {
        if (nbyte > n->len)
            nbyte = n->len;

        memcpy(buf, n->addr, nbyte);
        rm_head(dev);
    } else {
        nbyte = 0;
    }
    return nbyte;
}

ssize_t slank_write(struct slank_device* dev, const void* buf, size_t nbyte)
{
    add_tail(dev, nbyte);
    struct node* n = get_tail(dev);
    memcpy(n->addr, buf, nbyte);
    return nbyte;
}

void slank_info(struct slank_device* dev)
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


int main()
{
    size_t res;
    struct slank_device dev;

    slank_open(&dev);

    uint8_t buf1[10];
    memset(buf1, 0xaa, sizeof buf1);
    uint8_t buf2[11];
    memset(buf2, 0xbb, sizeof buf2);

    res = slank_write(&dev, buf1, sizeof buf1);
    res = slank_write(&dev, buf2, sizeof buf2);
    res = slank_write(&dev, buf2, sizeof buf2);
    res = slank_write(&dev, buf2, sizeof buf2);
    res = slank_write(&dev, buf2, sizeof buf2);
    
    res = slank_read(&dev, buf1, sizeof buf1);
    for (size_t i=0; i<res; i++)
        printf("%02x ", buf1[i]);
    printf("\n");

    res = slank_read(&dev, buf1, sizeof buf1);
    for (size_t i=0; i<res; i++)
        printf("%02x ", buf1[i]);
    printf("\n");

    res = slank_read(&dev, buf1, sizeof buf1);
    for (size_t i=0; i<res; i++)
        printf("%02x ", buf1[i]);
    printf("\n");

    slank_info(&dev);

    slank_close(&dev);
    return 0;
}




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

