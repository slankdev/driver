#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define kmalloc(A, B) malloc(A)
#define kfree free

#define copy_to_user(dst, src, count) memcpy(dst, src, count)
#define copy_from_user(dst, src, count) memcpy(dst, src, count)




typedef unsigned char uint8_t;

struct node {
    uint8_t *data;
    size_t   len;
    struct node *next;
};

struct slank_dev {
    struct node *head;
};

struct slank_dev *slank_devices;


static struct node *alloc_node(size_t size)
{
    struct node *node = kmalloc(sizeof(struct node), GFP_KERNEL);
    node->len  = size;
    node->data = kmalloc(node->len, GFP_KERNEL);
    node->next = NULL;
    return node;
}

static struct node* get_tail(void)
{
    struct node* n = slank_devices->head;
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


static void add_tail(size_t len)
{
    struct node* tail = get_tail();
    if (tail) {
        tail->next = alloc_node(len);
    } else {
        slank_devices->head = alloc_node(len);
    }
}


static void rm_head(void)
{
    if (slank_devices->head) {
        struct node* next = slank_devices->head->next;
        kfree(slank_devices->head->data);
        kfree(slank_devices->head);
        slank_devices->head = next;
    }
    return;
}



void slank_open()
{
    slank_devices->head = NULL;
}

void slank_close()
{
    while (1) {
        if (slank_devices->head == NULL)
            break;
        rm_head();
    }
}

ssize_t slank_read(void* buf, size_t nbyte)
{
    struct node* n = slank_devices->head;
    if (n) {
        if (nbyte > n->len)
            nbyte = n->len;

        copy_to_user(buf, n->data, nbyte);
        rm_head();
    } else {
        nbyte = 0;
    }
    return nbyte;
}

ssize_t slank_write(const void* buf, size_t nbyte)
{
    add_tail(nbyte);
    struct node* n = get_tail();
    copy_from_user(n->data, buf, nbyte);
    return nbyte;
}

void slank_info()
{
    struct node* h = slank_devices->head;
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
    slank_devices = (struct slank_dev*)kmalloc(sizeof(struct slank_dev), GFP_KERNEL);

    slank_open();

    res = slank_write(buf1, sizeof buf1);
    res = slank_write(buf2, sizeof buf2);
    res = slank_write(buf2, sizeof buf2);
    res = slank_write(buf2, sizeof buf2);
    res = slank_write(buf2, sizeof buf2);
    
    res = slank_read(buf1, sizeof buf1);
    res = slank_read(buf1, sizeof buf1);

    slank_info();
    slank_close();
    kfree(slank_devices);
    return 0;
}
