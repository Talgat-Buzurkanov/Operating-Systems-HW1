#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"
#include <pthread.h>
#include <semaphore.h>

void *generateRandom(void *);
void *findDivisors(void *);

struct Queue *queue;

struct number_struct
{
    int m;
    int r;
};

struct
{
    pthread_mutex_t mutex;
    int nput;
    int nval;
} put = {
    PTHREAD_MUTEX_INITIALIZER};

struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int nready;
} nready = {
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

int main(int argc, char *argv[])
{
    int t = 5, q = 10, r = 20, m = 100;

    queue = QueueInitialize(q);

    srand(time(NULL));

    struct number_struct args;
    args.m = m;
    args.r = r;

    // /////////////// Create 2 Threads ///////////////
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, generateRandom, &args);
    pthread_create(&thread2, NULL, findDivisors, &args);

    // char c;
    // while (1)
    // {

    // }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

void *
generateRandom(void *arg)
{
    struct number_struct *arg_str = (struct number_struct *)arg;
    int num = arg_str->m;
    int random_total = arg_str->r;

    int x = rand() % num;
    pthread_mutex_lock(&put.mutex);
    QueueInsert(&queue, x);

    pthread_mutex_unlock(&put.mutex);
    pthread_mutex_lock(&nready.mutex);
    if (nready.nready == 0)
        pthread_cond_signal(&nready.cond);
    nready.nready++;
    pthread_mutex_unlock(&nready.mutex);
}

void *
findDivisors(void *arg)
{

    sleep(rand() % 10);
    struct number_struct *arg_str = (struct number_struct *)arg;
    int num = arg_str->m;
    int random_total = arg_str->r;
    for (int i = 0; i < random_total; i++)
    {
        pthread_mutex_lock(&nready.mutex);
        while (nready.nready == 0)
        {
            pthread_cond_wait(&nready.cond, &nready.mutex);
        }
        nready.nready--;

        int n = QueueRemove(queue);
        pthread_t self_id;
        self_id = pthread_self();
        printf("Thread: %u, Number: %d, ", self_id, n);
        printf("Divisors: ");
        for (int i = 1; i <= n; i++)
        {
            if (n % i == 0)
                printf("%d ", i);
        }
        printf("\n");
        pthread_mutex_unlock(&nready.mutex);
    }
}
