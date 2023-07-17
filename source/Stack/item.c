
#include <stdlib.h>
#include "item.h"

Item *item_construct(data_type value, Item *next)
{
	Item *n = (Item *)malloc(sizeof(Item));
    n->value = value;
    n->next = next;
    return n;
}

void item_destroy(Item *n)
{
    free(n);
}
