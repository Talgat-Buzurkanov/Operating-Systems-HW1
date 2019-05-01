#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[])
{

    // default parameter values//
    int p_value = 20, n_value = 4, q_value = 3;
    double d_value = 10, g_value = 100;

    // getopt implementation//
    int index;
    int opt;
    while((opt=getopt(argc,argv,"p:n:q:d:g:")) !=-1)
    {
        switch(opt)
        {
            case 'p':
                p_value = atoi(optarg);
                break;
            case 'n':
                n_value = atoi(optarg);
                break;
            case 'q':
                q_value = atoi(optarg);
                break;
            case 'd':
                d_value = atof(optarg);
                break;
            case 'g':
                g_value = atof(optarg);
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                break;

        }
    }

    for (index = optind; index < argc; index++){
        printf("Non-option argument: %s\n",argv[index]);
    }

    // printing the parameter values, default unless getopt is not used//
    printf("NUM_PAYERS        : %d\n",p_value);
    printf("NUM_DESKS         : %d\n",n_value);
    printf("QUEUE_SIZE        : %d\n",q_value);
    printf("DURATION_RATE     : %f\n",d_value);
    printf("GENERATION_RATE   : %f\n",g_value);




    struct TaxPayer{
        int id;
        double duration;
    };


    struct TaxPayer payer = {0,0};
    payer.duration = d_value; 

    pthread_t tid[p_value];
    int err;

    pthread_mutex_t lock;

    // exponential distribution function//
    /* example code borrowed from
     https://stackoverflow.com/questions/34558230/generating-random-numbers-of-exponential-distribution */
    double exp_distr(double lambda){
        double x;
        x = rand()/(RAND_MAX+1.0);
        return -log(1-x)/lambda;
    }

    // thread generating function//
    /* example code borrowed from
     https://www.thegeekstuff.com/2012/04/create-threads-in-linux/?utm_source=feedburner */
    void *generatePayerThreads(void *arg)
    {


        double gen_sleep_time = exp_distr(g_value);

        sleep(gen_sleep_time);


        return NULL;
    }

    // main thread generation//
    // creates a thread and prints TaxPayer <0,1,2,..,(p_value-1)> //
    while (payer.id < p_value)
    {
        double dur_sleep_time = exp_distr(d_value);

        pthread_mutex_lock(&lock);

        err = pthread_create(&(tid[payer.id]), NULL, &generatePayerThreads, NULL);
        if (err !=0)
            printf("can't create thread :[%s]\n", strerror(err));
        else 
            printf("TaxPayer %d arrived\n",payer.id++);
            sleep(dur_sleep_time);

        pthread_mutex_unlock(&lock);

        }



    struct Information{
        int deskNo;
        struct TaxPayer payer;
    };

    struct Information information;
    information.deskNo = 0;



    struct Desk{
        int desk;
        int queue;
    };

    struct Desk desk_thread;
    desk_thread.queue = q_value; 
    desk_thread.queue = 0;

    int err1;
    pthread_t tid1[n_value];

    void *generateDeskThreads(void *arg)
    {

        

        return NULL;
    }



    while (desk_thread.desk < n_value)
    {
        pthread_mutex_lock(&lock);

        err1 = pthread_create(&(tid1[desk_thread.desk]), NULL, &generateDeskThreads, NULL);
        if (err1 !=0)
            printf("can't create thread :[%s]\n", strerror(err1));
        else 
            printf("Desk %d opened\n",desk_thread.desk++);
            

        pthread_mutex_unlock(&lock);
    }




    // Queue implementation //
    /* example code borrowed from
     https://stackoverflow.com/questions/37218411/how-do-i-implement-a-queue-  with-a-dynamically-allocated-array */
    struct PayerQueue {
      struct TaxPayer *payer;     // data type you're working with
      int maxSize;  // the physical array size
      int currentSize; // number of items in queue
      int frontIndex;  // location to pop from
      int rearIndex;  // location to push to
    };

    struct PayerQueue q;
    q.payer = malloc( sizeof(struct TaxPayer) * p_value );
    if ( q.payer )
    {
      q.maxSize = p_value;
      q.currentSize = q.frontIndex = q.rearIndex = 0;
    }

    int push(struct PayerQueue q, struct TaxPayer payer)
    {
      if ( q.currentSize == q.maxSize )
      {
        // queue full, handle as appropriate
        return 0;
      }
      else
      {
        q.payer[q.rearIndex] = payer;
        q.currentSize++;
        q.rearIndex = ( q.rearIndex + 1 ) % q.maxSize;
      }
      return 1;
    }

    int pop( struct PayerQueue q, struct TaxPayer *payer )
    {
      if ( q.currentSize == 0 )
      {
        // queue is empty, handle as appropriate
        return 0;
      }
      else
      {
        *payer = q.payer[q.frontIndex];
        q.currentSize--;
        q.frontIndex = ( q.frontIndex + 1 ) % q.maxSize;
      }

      return 1;
    }




    return 0;


}
