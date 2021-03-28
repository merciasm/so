#include <stdio.h>
#include "./linkedList.c"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 8

pthread_mutex_t mutex_client = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_volunteer = PTHREAD_MUTEX_INITIALIZER;

List *available_clothings;
List *clothings_to_repair;
int num;

void *clients_function(void *arg);

void *volunteers_function(void *arg);

int main(void) {
    pthread_t clients[NUM_THREADS];
    pthread_t volunteers[NUM_THREADS];
    time_t t;
    srand((unsigned) time(&t));

    available_clothings = makelist();
    clothings_to_repair = makelist();

    clothing *new_clothing = clothing_new(1, "Bermuda", 25, "P");
    num++;
    clothing *new_clothing1 = clothing_new(2, "Vestido", 50, "G");
    num++;
    clothing *new_clothing2 = clothing_new(3, "Camiseta", 30, "P");
    num++;
    clothing *new_clothing3 = clothing_new(4, "Vestido", 55, "GG");
    num++;
    clothing *new_clothing4 = clothing_new(5, "Camiseta", 25, "XG");
    num++;

    add(1, new_clothing, available_clothings);
    add(2, new_clothing1, available_clothings);
    add(3, new_clothing2, available_clothings);
    add(4, new_clothing3, available_clothings);
    add(5, new_clothing4, available_clothings);

    int i;
    for (i = 1; i <= NUM_THREADS; i++) {
        pthread_create(&volunteers[i], NULL, volunteers_function, &i);
        sleep(5);
        pthread_create(&clients[i], NULL, clients_function, &i);
        sleep(5);
    }

    int x;
    for (x = 1; x <= NUM_THREADS; x++) {
        pthread_join(volunteers[x], NULL);
        pthread_join(clients[x], NULL);
    }

    pthread_mutex_destroy(&mutex_client);
    pthread_mutex_destroy(&mutex_volunteer);
    printf("-----------------------\n");
    display(clothings_to_repair);
    printf("-----------------------\n");
    display(available_clothings);

    return 0;
}


void *volunteers_function(void *arg) {
    int choose_random_action = (rand() % 3) + 1;
    printf("%i \n", choose_random_action);
    char kind_random[20];
    int index = available_clothings->size;
    int i;
    Node *node;

    switch (choose_random_action) {
        case 1:
            sprintf(kind_random, "Modelo %d", num);
            clothing *new_clothing = clothing_new(index, kind_random, 762, "Tamanho");
            num++;
            pthread_mutex_lock(&mutex_volunteer);
            add(index, new_clothing, available_clothings);
            printf("Voluntario %d adiciona roupa %s\n", *(int *) arg, new_clothing->kind);
            pthread_mutex_unlock(&mutex_volunteer);
            sleep(2);
            break;
        case 2:
            if (clothings_to_repair->size > 0) {
                for (i = 1; i <= clothings_to_repair->size; i++) {
                    node = getAt(i, clothings_to_repair);
                    pthread_mutex_lock(&mutex_volunteer);
                    add(index, node->clothing, available_clothings);
                    delete(node->clothing, clothings_to_repair);
                    pthread_mutex_unlock(&mutex_volunteer);
                    printf("Voluntario %d liberou roupa %s do reparo\n", *(int *) arg, node->clothing->kind);
                }
            } else {
                printf("Bazar %i sem roupas para reparo\n", *(int *) arg);
            }
            sleep(2);
            break;
        case 3:
            if (available_clothings->size > 0) {
                int rand_index = (rand() % available_clothings->size) + 1;
                node = getAt(rand_index, available_clothings);
                pthread_mutex_lock(&mutex_volunteer);
                printf("Voluntario %i retirou roupa %s\n", *(int *) arg, node->clothing->kind);
                delete(node->clothing, available_clothings);
                pthread_mutex_unlock(&mutex_volunteer);
            }
            sleep(2);
            break;
        default:
            printf("Falha na Thread voluntários! ");
            pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

void *clients_function(void *arg) {
    Node *node;
    int ran_sleep = (rand() % 10) + 1;

    if (available_clothings->size > 0) {
        int rand_index = (rand() % available_clothings->size) + 1;
        pthread_mutex_lock(&mutex_client);
        node = getAt(rand_index, available_clothings);
        printf("Cliente %d compra roupa %s\n", *(int *) arg, node->clothing->kind);
        delete(node->clothing, available_clothings);
        pthread_mutex_unlock(&mutex_client);
        sleep(ran_sleep);
        if (node != NULL) {
            pthread_mutex_lock(&mutex_client);
            add(node->key, node->clothing, clothings_to_repair);
            printf("Cliente %d doa roupa %s\n", *(int *) arg, node->clothing->kind);
            pthread_mutex_unlock(&mutex_client);
            sleep(ran_sleep);
        }
    } else {
        printf("Sem roupas disponiveis\n");
    }

    pthread_exit(NULL);
}