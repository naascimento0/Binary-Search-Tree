#ifndef _NODE_H_
#define _NODE_H_

typedef void* data_type;

typedef struct Element
{
    data_type value;
    struct Element *prev;
    struct Element *next;
} Element;

Element *element_construct(data_type value, Element *prev, Element *next);

void element_destroy(Element *n);

#endif
