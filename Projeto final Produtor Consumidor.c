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

// C�digo com o int�ito de simular maquinas produzindo e consumindo itens colocados em um buffer, com o uso de threads e de sem�foros

int buffer[tamanho];
int ultimo = 0;//inteiros que ser�o usados durante o c�digo para indicar as posi��es do buffer que ter�o item colocados e retirados
int primeiro  = 0;

int numero_produz = 1;  //inteiros que ser�o usados no c�digo para indicar qual � o produtor e consumidor realizando a a��o
int numero_consome = 1;
sem_t mutex; //mutualmente exclusivo
sem_t consuma; //sem�foro que indica ao consumidor se existe algo a ser consumido
sem_t produza; //sem�foro que indica ao produtor que existe espa�o no buffer para que algo seja produzido e colocado naquele lugar

void escreve(int item) {
    buffer[ultimo] = item;    /* coloca o q no buffer */
    ultimo = (ultimo + 1) % tamanho; /* aumenta o valor de ultimo, para que o pr�ximo item seja colocado no pr�ximo espa�o no buffer*/
}

int le_buffer() {// retorna o valor do item que estava no buffer, que � o que ser� consumido pelo consumidor
    int item;

    item = buffer[primeiro];   /* recupera o valor do buffer */
    primeiro = (primeiro + 1) % tamanho;   /* aumenta o valor retirado */
    return item; // retorna o valor recuperado do buffer para o consumidor
}

int getRandomNumber() { // responsavel por criar a numera��o dos itens criados pelos produtores e evitar repeti��es em excesso
    int random_number = rand() % 100; // d� para random_number um valor aleat�rio entre 0 e 100
    while (1) { //
        int bFoundEqual = 0;
        for (int i=ultimo; i>0; i--) {
            if (buffer[i] == random_number) {   // toda essa parte � feita para impedir que o novo item tenha mesmo n�mero de um j� existente, aleatorizando o n�mero novamente caso ele seja igual
                bFoundEqual = 1;
                break;
            }
        }
        if (bFoundEqual == 0){
            break;
            }

        random_number = rand() % 100;
    }

    return random_number;
}

void *produtor(void *arg) {
    int novo_item;
    int numero_proprio = numero_produz;
    numero_produz = (numero_produz + 1);

    while(1){
        sleep( 5 * (float)rand()/RAND_MAX);// faz o produtor dormir por um tempo entre 0 e 5 micro-segundos

        sem_wait(&produza);     // bloqueia produtor se o buffer estiver cheio
        sem_wait(&mutex);     // mutex

        novo_item = getRandomNumber(); //

        escreve(novo_item);       // estado critico
        printf("\n Maquina produtora %d produziu o item:  %d", numero_proprio, novo_item);

        sem_post(&mutex);     // mutex
        sem_post(&consuma);     // acorda o consumidor ao desbloquear o semaforo, dizendo que existe algo a ser consumido


    }
}


void *consumidor(void *arg) {
    int item;
    int numero_proprio = numero_consome;
    numero_consome = (numero_consome + 1);

    while(1){

       sem_wait(&consuma);     /* bloqueia se estiver vazio o buffer */
       sem_wait(&mutex);     /* mutex */

        item = le_buffer();

        sleep( 5 * (float)rand()/RAND_MAX);// faz o produtor dormir por um tempo entre 0 e 5 micro-segundos

        printf("\nConsumidor %d consumiu o item: %d", numero_proprio, item);

       sem_post(&mutex);     /* mutex */
       sem_post(&produza);
    }
    }


int main(int argc, char *argv[]) {
    pthread_t prod1, prod2, prod3, con1, con2, con3; //nomeia threads

    sem_init(&mutex, 0, 1);  // inicia os semaforos
    sem_init(&consuma, 0, 0);
    sem_init(&produza, 0, 20);

    /* Cria as threads de produtores e consumidores */
    srand(time(NULL));
    pthread_create(&prod1, NULL, produtor, NULL);

    pthread_create(&prod2, NULL, produtor, NULL);

    pthread_create(&prod3, NULL, produtor, NULL);

    pthread_create(&con1, NULL, consumidor, NULL);

    pthread_create(&con2, NULL, consumidor, NULL);

    pthread_create(&con3, NULL, consumidor, NULL);


    /* Aguarda o fim das threads */
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(prod3, NULL);
    pthread_join(con1, NULL);
    pthread_join(con2, NULL);
    pthread_join(con3, NULL);


    int sem_destroy(sem_t *produza);
    int sem_destroy(sem_t *consuma);
    int sem_destroy(sem_t *mutex);


    printf("\n\n");

    return 0;


}
