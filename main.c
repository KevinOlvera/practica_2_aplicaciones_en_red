#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define lim_cp 1    //# Consumos Producciones
#define lim_sp 16       //# Semaforos
#define lim_sc 8        //# Secciones
#define lim_th 10       //# Hilos

char global_variable[lim_sc];
int num[10]={0,0,0,0,0,0,0,0,0,0};

sem_t sem[lim_sp];

void *producer(void *args) {
    char c = *(char *)args;
    int i = 0;
    while (i < lim_cp)
    {
        int k, flag = 0;

        while (flag == 0) {
            for (int j = 0, k = 0; k < lim_sc/2; j = j+2, k++)
                if (sem_trywait(&sem[j]) == 0) {
                    global_variable[k] = c;
                    printf("Producer %c - Ciclo %d - Seccion %d\n", global_variable[k], i+1, k+1);
                    sem_post(&sem[j+1]);
                    flag = 1;
                    break;
                }
        }

        while (flag == 1) {
            for (int j = lim_sp/2, k = lim_sc/2; k < lim_sc; j = j+2, k++)
                if (sem_trywait(&sem[j]) == 0) {
                    global_variable[k] = c-16;
                    printf("Producer %c - Ciclo %d - Seccion %d\n", global_variable[k], i+1, k+1);
                    sem_post(&sem[j+1]);
                    i++;
                    flag = 0;
                    break;
                }
        }

    }
    return EXIT_SUCCESS;
}

void *consumer(void *args) {
    char c = *(char *)args;
    int i = 0;
    
    while (i < lim_cp)
    {
        int k = 0, flag = 0;
        while (flag == 0) {
            for (int j = 0, k = 0; k < lim_sc/2; j = j+2, k++)
                if (sem_trywait(&sem[j+1]) == 0) {
                    printf("Consumer %c - Ciclo %d\t- Seccion %d - Valor %c\n", c, i+1, k+1, global_variable[k]);
                    switch(global_variable[k]){
                    	case 'A':{
                    		FILE *fichero;
                    		fichero = fopen("A.txt","a+");
                    		//fputc('A',fichero);
                    		fprintf(fichero,"%d A\n",num[0]);
                    		num[0]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case 'B':{
                    		FILE *fichero;
                    		fichero = fopen("B.txt","a+");
                    		//fputc('B',fichero);
                    		fprintf(fichero,"%d B\n",num[1]);
                    		num[1]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case 'C':{
                    		FILE *fichero;
                    		fichero = fopen("C.txt","a+");
                    		//fputc('C',fichero);
                    		fprintf(fichero,"%d C\n",num[2]);
                    		num[2]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case 'D':{
                    		FILE *fichero;
                    		fichero = fopen("D.txt","a+");
                    		//fputc('D',fichero);
                    		fprintf(fichero,"%d D\n",num[3]);
                    		num[3]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case 'E':{
                    		FILE *fichero;
                    		fichero = fopen("E.txt","a+");
                    		//fputc('E',fichero);
                    		fprintf(fichero,"%d E\n",num[4]);
                    		num[4]++;
                    		fclose(fichero);
                    		break;
                    	}
                    }

                    sem_post(&sem[j]);
                    flag = 1;
                    break;
                }
        }

        while (flag == 1) {
            for (int j = lim_sp/2, k = lim_sc/2; k < lim_sc; j = j+2, k++)
                if (sem_trywait(&sem[j+1]) == 0) {
                    printf("Consumer %c - Ciclo %d\t- Seccion %d - Valor %c\n", c, i+1, k+1, global_variable[k]);
                    switch(global_variable[k]){
                    	case '1':{
                    		FILE *fichero;
                    		fichero = fopen("1.txt","a+");
                    		//fputc('1',fichero);
                    		fprintf(fichero,"%d 1\n",num[5]);
                    		num[5]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case '2':{
                    		FILE *fichero;
                    		fichero = fopen("2.txt","a+");
                    		//fputc('2',fichero);
                    		fprintf(fichero,"%d 2\n",num[6]);
                    		num[6]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case '3':{
                    		FILE *fichero;
                    		fichero = fopen("3.txt","a+");
                    		//fputc('3',fichero);
                    		fprintf(fichero,"%d 3\n",num[7]);
                    		num[7]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case '4':{
                    		FILE *fichero;
                    		fichero = fopen("4.txt","a+");
                    		//fputc('4',fichero);
                    		fprintf(fichero,"%d 4\n",num[8]);
                    		num[8]++;
                    		fclose(fichero);
                    		break;
                    	}
                    	case '5':{
                    		FILE *fichero;
                    		fichero = fopen("5.txt","a+");
                    		//fputc('5',fichero);
                    		fprintf(fichero,"%d 5\n",num[9]);
                    		num[9]++;
                    		fclose(fichero);
                    		break;
                    	}
                    }
                    sem_post(&sem[j]);
                    i++;
                    flag = 0;
                    break;
                }
        }
    }
}

int main(int argc, char const *argv[])
{
    for (int i = 0; i < lim_sp; i++)
        if (i%2 == 0)
            sem_init(&sem[i], 0, 1);
        else
            sem_init(&sem[i], 0, 0);

    //Creacion de hilos
    pthread_t threads[lim_th];

    if (0 != pthread_create(&threads[0], NULL, producer, (void *)"A"))
        return -1;

    if (0 != pthread_create(&threads[1], NULL, producer, (void *)"B"))
        return -1;
            
    if (0 != pthread_create(&threads[2], NULL, producer, (void *)"C"))
        return -1;

    if (0 != pthread_create(&threads[3], NULL, producer, (void *)"D"))
        return -1;

    if (0 != pthread_create(&threads[4], NULL, producer, (void *)"E"))
        return -1;

    if (0 != pthread_create(&threads[5], NULL, consumer, (void *)"1"))
        return -1;

    if (0 != pthread_create(&threads[6], NULL, consumer, (void *)"2"))
        return -1;
            
    if (0 != pthread_create(&threads[7], NULL, consumer, (void *)"3"))
        return -1;

    if (0 != pthread_create(&threads[8], NULL, consumer, (void *)"4"))
        return -1;

    if (0 != pthread_create(&threads[9], NULL, consumer, (void *)"5"))
        return -1;

    for (int i = 0; i < lim_th; i++)
        pthread_join(threads[i], NULL);

    return 0;
}