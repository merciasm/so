#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "./clothing.c"

typedef struct node {
    clothing *clothing;
    int id;
    struct node *next;

} Node;

typedef struct list {
    Node *head;
    int size;
} List;


Node *createnode(clothing *clothing, int id) {
    Node *newNode = malloc(sizeof(Node));

    if(newNode == NULL){
        fprintf(stderr, "Error: not enough memory.\n");
        return NULL;
    }

    newNode->clothing = clothing;
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

List *makelist() {
    List *list = malloc(sizeof(List));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

void display(List *list) {

    Node *current = list->head;

    while (current != NULL)
    {
        printf("%s\n", current->clothing->kind);
        current = current->next;
    }
}

void add(int id, clothing *clothing, List *list) {
    Node *current = NULL;
    if (list->head == NULL) {
        list->head = createnode(clothing, id);
    } else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = createnode(clothing, id);
    }
    list->size++;
}

void delete(clothing *clothing, List *list) {  
    Node *tmp = list->head;
    Node *previous = tmp;
    while (tmp != NULL) {
        if (tmp->clothing == clothing) {    
            previous->next = tmp->next;
            if (tmp == list->head)
                list->head = tmp->next;
            list->size--;
            return;
        }
        previous = tmp;
        tmp = tmp->next;
    }
}

Node *getAt(int id, List *list) { 
    Node *current = list->head;
    int count = 1;
    while (current != NULL) {
        if (count == id) {
            return current;
        }
        count++;
        current = current->next;
    }
    return NULL;
}