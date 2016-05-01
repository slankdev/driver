#include <stdio.h>
#include <string.h>
#include <stdlib.h>





typedef unsigned char uint8_t;

struct node {
    uint8_t *data;
    size_t   len;
    struct node *next;
};


struct slank_dev {
    struct node *head;
};


static struct node *alloc_node(size_t size)
{
    struct node *node = malloc(sizeof(struct node));
    node->len  = size;
    node->data = malloc(node->len);
    node->next = NULL;
    return node;
}

static struct node* get_tail(struct slank_dev* dev)
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


static void add_tail(struct slank_dev* dev, size_t len)
{
    struct node* tail = get_tail(dev);
    if (tail) {
        tail->next = alloc_node(len);
    } else {
        dev->head = alloc_node(len);
    }
}


static void rm_head(struct slank_dev* dev)
{
    if (dev->head) {
        struct node* next = dev->head->next;
        free(dev->head->data);
        free(dev->head);
        dev->head = next;
    }
    return;
}




void slank_open(struct slank_dev* dev)
{
    dev->head = NULL;
}

void slank_close(struct slank_dev* dev)
{
    while (1) {
        if (dev->head == NULL)
            break;
        rm_head(dev);
    }
}

ssize_t slank_read(struct slank_dev* dev, void* buf, size_t nbyte)
{
    struct node* n = dev->head;
    if (n) {
        if (nbyte > n->len)
            nbyte = n->len;

        memcpy(buf, n->data, nbyte);
        rm_head(dev);
    } else {
        nbyte = 0;
    }
    return nbyte;
}

ssize_t slank_write(struct slank_dev* dev, const void* buf, size_t nbyte)
{
    add_tail(dev, nbyte);
    struct node* n = get_tail(dev);
    memcpy(n->data, buf, nbyte);
    return nbyte;
}

void slank_info(struct slank_dev* dev)
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
    uint8_t buf1[10];
    uint8_t buf2[11];
    struct slank_dev dev;

    slank_open(&dev);

    res = slank_write(&dev, buf1, sizeof buf1);
    res = slank_write(&dev, buf2, sizeof buf2);
    res = slank_write(&dev, buf2, sizeof buf2);
    res = slank_write(&dev, buf2, sizeof buf2);
    res = slank_write(&dev, buf2, sizeof buf2);
    
    res = slank_read(&dev, buf1, sizeof buf1);
    res = slank_read(&dev, buf1, sizeof buf1);
    res = slank_read(&dev, buf1, sizeof buf1);

    slank_info(&dev);
    slank_close(&dev);
    return 0;
}
