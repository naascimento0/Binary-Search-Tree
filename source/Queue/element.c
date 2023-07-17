#include <stdio.h>
#include <stdlib.h>
#include "element.h"

Element *element_construct(data_type value, Element *prev, Element *next)
{
	Element *n = (Element *)malloc(sizeof(Element));
    n->value = value;
    n->next = next;
    n->prev = prev;
    return n;
}

void element_destroy(Element *n)
{
    free(n);
}
