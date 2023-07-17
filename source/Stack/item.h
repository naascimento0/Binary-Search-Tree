
#ifndef _ITEM_H_
#define _ITEM_H_

typedef void* data_type;

typedef struct Item
{
    data_type value;
    struct Item *next;
} Item;

Item *item_construct(data_type value, Item *next);
void item_destroy(Item *n);

#endif
