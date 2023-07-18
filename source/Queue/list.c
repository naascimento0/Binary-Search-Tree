#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List *list_construct(){
    List *list = calloc(1, sizeof(List));
    list->last = list->head = NULL;
    return list;
}

int list_size(List *l){
    return l->size;
}

void list_push_front(List *l, data_type data){
    Element *new = element_construct(data, NULL, l->head);
    if(l->head != NULL)
        l->head->prev = new;
    else
        l->last = new;
    
    l->head = new;
    l->size++;
}

void list_push_back(List *l, data_type data){
    Element *new = element_construct(data, l->last, NULL);

    if(l->last != NULL)
        l->last->next = new;
    else
        l->head = new;

    l->last = new;
    l->size++;
}

void list_print(List *l, void (*print_fn)(data_type)){
    printf("[");

    Element *aux = l->head;
    while(aux != NULL){
        print_fn(aux->value);
        aux = aux->next;

        if(aux != NULL)
            printf(", ");
    }

    printf("]");
}

void list_print_reverse(List *l, void (*print_fn)(data_type)){
    printf("[");

    if(l->last != NULL){

        Element *aux = l->last;
        while(aux != NULL){
            print_fn(aux->value);
            aux = aux->prev;

            if(aux != NULL)
                printf(", ");
        }
    }

    printf("]");

}

void list_destroy(List *l){
    while(l->head != NULL){
        Element *aux = l->head;
        l->head = l->head->next;
        element_destroy(aux);
    }

    free(l);
}

data_type list_pop_front(List *l){
    if(l->head == NULL)
	    exit(printf("Error: list is empty!\n"));

    Element *aux = l->head;
    data_type value = aux->value;

    l->head = aux->next;
    if(l->head != NULL)
    	l->head->prev = NULL;

    element_destroy(aux);
    l->size--;

    if(l->size <= 1)
    	l->last = l->head;

    return value;
}


