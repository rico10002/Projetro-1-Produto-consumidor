#include<stdio.h>
#include<semaphore.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define tamanho 20

int buffer[tamanho];
int ultimo = 0;
int primeiro  = 0;
sem_t m;
sem_t c;
sem_t p;

void escreve(int q) {
    buffer[ultimo] = q;    /* coloca o q no buffer */
    ultimo = (ultimo + 1) % tamanho; /* aumenta o valor do item */
}

int le() {// retorna o valor do item que estava no buffer, que é o que será consumido pelo consumidor
    int w;

    w = buffer[primeiro];   /* recupera o valor do buffer */
    primeiro = (primeiro + 1) % tamanho;   /* aumenta o valor retirado */
    return w;
}


void *produtor1(void *arg) {
    int i = 101;
    while(1){
        sleep(4);

        sem_wait(&p);     // bloqueia produtor se o buffer estiver cheio
        sem_wait(&m);     // mutex
        escreve(i);       // estado critico
        printf("\n Maquina produtora 1 produziu o item:  %d", i);
        i++;
        sem_post(&m);     // mutex
        sem_post(&c);     // acorda o consumidor ao desbloquear o semaforo

    }
}

void *produtor2(void *arg) {
    int i = 201;
    while(1){
        sleep(2);

        sem_wait(&p);     // bloqueia produtor se o buffer estiver cheio
        sem_wait(&m);     // mutex
        escreve(i);       // estado critico
        printf("\n Maquina produtora 2 produziu o item:  %d", i);
        i++;
        sem_post(&m);     // mutex
        sem_post(&c);     // acorda o consumidor ao desbloquear o semaforo


    }
}


void *consumidor1(void *arg) {
    int i, item;
    while(1){

       sem_wait(&c);     /* bloqueia se estiver vazio o buffer */
       sem_wait(&m);     /* mutex */

        item = le();

        sleep(6);  /* dorme */
        printf("\nConsumidor 1 processou o item %d", item);



       sem_post(&m);     /* mutex */
       sem_post(&p);
    }
    }



    void *consumidor2(void *arg) {
    int i, item;
    while(1){

       sem_wait(&c);     /* bloqueia se estiver vazio o buffer */
       sem_wait(&m);     /* mutex */

        item = le();

        sleep(12);  /* dorme */
        printf("\nConsumidor 2 processou o item %d", item);


       sem_post(&m);     /* mutex */
       sem_post(&p);
    }
    }


    void *consumidor3(void *arg) {
    int i, item;
    while(1){

       sem_wait(&c);     /* bloqueia se estiver vazio o buffer */
       sem_wait(&m);     /* mutex */

        item = le();

        sleep(2);  /* dorme */
        printf("\nConsumidor 3 processou o item %d", item);


       sem_post(&m);     /* mutex */
       sem_post(&p);
    }
    }



int main(int argc, char *argv[]) {
    int i;
    pthread_t prod1, prod2, con1, con2, con3; //nomeia threads

    sem_init(&m, 0, 1);  // inicia os semaforos
    sem_init(&c, 0, 0);
    sem_init(&p, 0, 10);

    /* Cria as threads de produtores e consumidores */
    pthread_create(&prod1, NULL, produtor1, NULL);
    pthread_create(&prod2, NULL, produtor2, NULL);
    pthread_create(&con1, NULL, consumidor1, NULL);
    pthread_create(&con2, NULL, consumidor2, NULL);
    pthread_create(&con3, NULL, consumidor3, NULL);


    /* Aguarda o fim das threads */
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(con1, NULL);
    pthread_join(con2, NULL);
    pthread_join(con3, NULL);


    printf("\n\n");

    return 0;


}
