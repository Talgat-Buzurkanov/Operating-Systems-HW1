struct Queue
{
    int front, rear, size;
    int capacity;
    int* array;
};


struct Queue* QueueInitialize(int);
int isFull(struct Queue* );
int isEmpty(struct Queue* );
void  QueueInsert(struct Queue *, int);
int QueueRemove(struct Queue *);
//void QueueDestroy(struct Queue *);
