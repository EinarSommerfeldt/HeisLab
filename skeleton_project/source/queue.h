#include "hardware.h"
#include "fsm.h"

#ifndef QUEUE_H
#define QUEUE_H


struct Node {
    int value;
    struct Node* next;
    struct Node* prev;
};

struct Queue {
    struct Node* head;
    struct Node* const tail; //dummy node
};



//Queue funksjoner
void addToQueue(struct Queue* queue, struct Elevator* elev, int floor);
void insertNode(struct Queue* queue, int value);
int removeNode(struct Queue* queue, struct Node* node);
int isEmpty(struct Queue* queue);
struct Node* getHead(struct Queue* queue);
int popQueue(struct Queue* queue); //returner neste element i køen og sletter det

//Node funksjoner
struct Node* getPrev(struct Node* node);
struct Node* getNext(struct Node* node);
int getValue(struct Node* node);
void deleteNode(struct Node* node);








































#endif