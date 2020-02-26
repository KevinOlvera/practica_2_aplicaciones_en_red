#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

char global_variable_1[2];
char global_variable_2[2];
sem_t sem_1, sem_2, sem_3, sem_4;
int lim_cp = 2; 

void *producer(void *args) {
    char c[1] = "";
    strcpy(c, (char *)args);
    int i = 0;
    while (i < lim_cp)
    {
        if (sem_trywait(&sem_1) == 0) {
            sprintf(global_variable_1, "%s", c);
            printf("Producer %s - Ciclo %d - Seccion S1\n", global_variable_1, i);
            sem_post(&sem_2);
            i++;
        } else if (sem_trywait(&sem_3) == 0) {
            sprintf(global_variable_2, "%s", c);
            printf("Producer %s - Ciclo %d - Seccion S2\n", global_variable_2, i);
            sem_post(&sem_4);
            i++;
        }
    }
    return EXIT_SUCCESS;
}

void *consumer(void *args) {
    char c[1] = "";
    strcpy(c, (char *)args);
    int i = 0;
    while (i < lim_cp)
    {
        if (sem_trywait(&sem_2) == 0) {
            printf("Consumer %s - Ciclo %d - Seccion S1 - Valor %s\n", c, i, global_variable_1);
            sem_post(&sem_1);
            i++;
        } else if (sem_trywait(&sem_4) == 0) {
            printf("Consumer %s - Ciclo %d - Seccion S2 - Valor %s\n", c, i, global_variable_2);
            sem_post(&sem_3);
            i++;
        }
    }
}

int main(int argc, char const *argv[])
{

    unsigned int semaphore_value;

    //Creacion e inicializacion del semaforo
    sem_init(&sem_1, 0, 1);
    sem_init(&sem_2, 0, 0);
    sem_init(&sem_3, 0, 1);
    sem_init(&sem_4, 0, 0);

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
