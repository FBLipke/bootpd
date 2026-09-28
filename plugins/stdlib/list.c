/**
 * Simple Singly-Linked List
 * For use in freestanding 16-bit environments
 */

#include "include/stdlib.h"

/* Forward declare structs (defined in stdlib.h) */
struct list_node;
struct list;

/* Initialize list */
void list_init(struct list *l)
{
    if (!l)
        return;
    l->head = NULL;
    l->tail = NULL;
    l->count = 0;
}

/* Add item to end of list */
void list_add(struct list *l, void *data)
{
    if (!l)
        return;

    struct list_node *node = (struct list_node *)malloc(sizeof(struct list_node));
    if (!node)
        return;

    node->data = data;
    node->next = NULL;

    if (l->tail)
    {
        l->tail->next = node;
        l->tail = node;
    }
    else
    {
        l->head = l->tail = node;
    }
    l->count++;
}

/* Get item at index */
void *list_get(struct list *l, int index)
{
    if (!l || index < 0 || index >= l->count)
        return NULL;

    struct list_node *node = l->head;
    int i = 0;
    while (node && i < index)
    {
        node = node->next;
        i++;
    }
    return node ? node->data : NULL;
}

/* Get count */
int list_count(struct list *l)
{
    return l ? l->count : 0;
}

/* Remove item at index */
void list_remove(struct list *l, int index)
{
    if (!l || index < 0 || index >= l->count)
        return;

    struct list_node *node = l->head;
    struct list_node *prev = NULL;
    int i = 0;

    while (node && i < index)
    {
        prev = node;
        node = node->next;
        i++;
    }

    if (!node)
        return;

    if (prev)
    {
        prev->next = node->next;
    }
    else
    {
        l->head = node->next;
    }

    if (!node->next)
    {
        l->tail = prev;
    }

    free(node);
    l->count--;
}

/* Free all nodes (data NOT freed) */
void list_clear(struct list *l)
{
    if (!l)
        return;

    struct list_node *node = l->head;
    while (node)
    {
        struct list_node *next = node->next;
        free(node);
        node = next;
    }
    l->head = l->tail = NULL;
    l->count = 0;
}

/* Iterate through list */
void *list_iterate(struct list *l, int *state)
{
    if (!l || !state)
        return NULL;

    static struct list_node *current = NULL;

    if (*state == 0)
    {
        current = l->head;
    }
    else if (*state == 1)
    {
        current = current ? current->next : NULL;
    }

    *state = 1;
    return current ? current->data : NULL;
}
