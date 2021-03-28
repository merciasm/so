#include "./clothing.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct node {
    clothing *clothing;
    int key;
    struct node *next;

} Node;

typedef struct list {
    Node *head;
    int size;
} List;


Node *createnode(clothing *clothing, int key) {
    Node *newNode = malloc(sizeof(Node));

    if (!newNode) {
        return NULL;
    }

    newNode->clothing = clothing;
    newNode->key = key;
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
    if (list->head == NULL)
        return;

    for (; current != NULL; current = current->next) {
        printf("%s\n", current->clothing->kind);
    }
}

void add(int key, clothing *clothing, List *list) {
    Node *current = NULL;
    if (list->head == NULL) {
        list->head = createnode(clothing, key);
    } else {
        current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = createnode(clothing, key);
    }
    list->size++;
}

void delete(clothing *clothing, List *list) {
    Node *current = list->head;
    Node *previous = current;
    while (current != NULL) {
        if (current->clothing == clothing) {
            previous->next = current->next;
            if (current == list->head)
                list->head = current->next;
            list->size--;
            return;
        }
        previous = current;
        current = current->next;
    }
}

Node *getAt(int key, List *list) {
    Node *current = list->head;
    int count = 1;
    while (current != NULL) {
        if (count == key) {
            return current;
        }
        count++;
        current = current->next;
    }
    return NULL;
}

clothing *clothing_new(int id, char *kind, float price, char *size) {
    clothing *new_clothing = malloc(sizeof(clothing));
    new_clothing->code = id;
    strcpy(new_clothing->kind, kind);
    new_clothing->price = price;
    strcpy(new_clothing->size, size);
    return new_clothing;
}