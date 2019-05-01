#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

struct Queue* QueueInitialize(int capacity)
{  
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0; 
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));  
  
    return queue;
}

// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* q)
{  return (q->size == q->capacity);  
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* q)
{  return (q->size == 0); }

void QueueInsert(struct Queue *q, int item)
{
    if (isFull(q))
        return;
    q->rear = (q->rear + 1)%q->capacity;
    q->array[q->rear] = item;
    q->size = q->size + 1;
}

int QueueRemove(struct Queue *q)
{
    if (isEmpty(q))
        return 0;
    int item = q->array[q->front];
    q->front = (q->front + 1)%q->capacity;
    q->size = q->size - 1;
    return item;
}



