#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 2

unsigned long shared_counter = 0;

void increment_shared_counter()
{
    shared_counter = shared_counter + 1;
}

unsigned long get_shared_counter()
{
    return shared_counter;
}

void *increment_routine(void *)
{
    for (size_t i = 0; i < 10e8; i++)
    {
        //increment_shared_counter();
        shared_counter = shared_counter + 1;
    }

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    pthread_t thread_ids[NUM_THREADS];

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&thread_ids[i], NULL, increment_routine, NULL);
        printf("Thread with TID %ld has been started\n", thread_ids[i]);
    }

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread_ids[i], NULL);
        printf("Thread with TID %ld has finished\n", thread_ids[i]);
    }

    printf ("Final counter value is %ld\n", shared_counter);

    return 0;
}