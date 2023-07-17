#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct Queue Queue;

#include "list.h"

// cria uma queue
Queue *queue_construct();

// insere um item na queue 
void queue_enqueue(Queue *queue, data_type value);

// remove o elemento mais antigo da pilha e o retorna
data_type queue_dequeue(Queue *queue);

// retorna 1 se a queue está vazia e 0 caso contrário
int queue_empty(Queue *queue);

// libera o espaco alocado para a queue
void queue_destroy(Queue *queue);

#endif
