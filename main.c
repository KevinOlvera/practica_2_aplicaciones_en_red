#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

//char global_variable_1[4];
//char global_variable_2[4];

char global_variable_1;
char global_variable_2;

sem_t sem[4];

int lim_cp = 1;

int error(char *err);



void *producer(void *args) {
    char c = *(char *)args;
    int i = 0;
    while (i < lim_cp)
    {
        if (sem_trywait(&sem[0]) == 0) {
            global_variable_1 = c;
            printf("Producer %c - Ciclo %d - Seccion S1\n", global_variable_1, i);
            sem_post(&sem[1]);
            i++;
        } else if (sem_trywait(&sem[2]) == 0) {
            global_variable_2 = c;
            printf("Producer %c - Ciclo %d - Seccion S2\n", global_variable_2, i);
            sem_post(&sem[3]);
            i++;
        }
    }
    return EXIT_SUCCESS;
}

void *consumer(void *args) {
    char c = *(char *)args;
    int i = 0;
    while (i < lim_cp)
    {
        if (sem_trywait(&sem[1]) == 0) {
            printf("Consumer %c - Ciclo %d - Seccion S1 - Valor %c\n", c, i, global_variable_1);
            sem_post(&sem[0]);
            i++;
        } else if (sem_trywait(&sem[3]) == 0) {
            printf("Consumer %c - Ciclo %d - Seccion S2 - Valor %c\n", c, i, global_variable_2);
            sem_post(&sem[2]);
            i++;
        }
    }
}




int main(int argc, char const *argv[])
{
    for (int i = 0; i < 4; i++)
        if (i%2 == 0)
            sem_init(&sem[i], 0, 1);
        else
            sem_init(&sem[i], 0, 0);

    //Creacion de hilos
    pthread_t threads[8];

    if (0 != pthread_create(&threads[0], NULL, producer, (void *)"A"))
            return -1;

    if (0 != pthread_create(&threads[1], NULL, producer, (void *)"B"))
            return -1;
            
    if (0 != pthread_create(&threads[2], NULL, producer, (void *)"C"))
            return -1;

    if (0 != pthread_create(&threads[3], NULL, producer, (void *)"D"))
            return -1;

    if (0 != pthread_create(&threads[4], NULL, consumer, (void *)"1"))
            return -1;

    if (0 != pthread_create(&threads[5], NULL, consumer, (void *)"2"))
            return -1;
            
    if (0 != pthread_create(&threads[6], NULL, consumer, (void *)"3"))
            return -1;

    if (0 != pthread_create(&threads[7], NULL, consumer, (void *)"4"))
            return -1;

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
    pthread_join(threads[4], NULL);
    pthread_join(threads[5], NULL);
    pthread_join(threads[6], NULL);
    pthread_join(threads[7], NULL);








    return 0;
}

int error(char *err) {
    perror(err);
    return -1;
}