/**
 * @file
 */
#include "queue.h"


//Queue funksjoner



struct Queue* queue_new() { 
    struct Queue* queue = malloc(sizeof(struct Queue));
    queue->tail = malloc(sizeof(struct Node));
    queue->tail->next = NULL;
    queue->tail->prev = NULL;
    queue->tail->value = 0;
    queue->head = queue->tail;
    return queue;
}

//Gjør antagelsen av at alle går inn når heisdøra åpner seg på etasjen de har trykket knappen
void queue_addFloor(struct Queue* queue, struct Elevator* elev, int floor, int direction) {
    int resolved = 0; //Is set to 1 when the floor is added to the queue
    if (queue->head == queue->tail) {
        queue_insertNode(queue, queue->tail, 4);
    } else  { //Heisen går opp fra før av
        struct Node* currentNode = queue->head;
        int highestValue = 0;
        int directionSwitched = 0; //if we are adding to the front or the back of the queue
        
        while(currentNode != queue->tail) {
            if (currentNode->value > highestValue) {
                highestValue = currentNode->value;
            } else {
                directionSwitched = 1;
            }
            if (!directionSwitched) {
                if (direction = 1) { //Heisen er på vei opp
                    if (currentNode->value < floor) {
                        queue_insertNode(queue, currentNode->next, floor);
                        resolved = 1;
                        break;
                    } else if (currentNode->value == floor) {
                        resolved = 1;
                        break;
                    } 
                }
            }
            currentNode = currentNode->next;
        }
    }
    if (!resolved) {
        queue_insertNode(queue, queue->tail, floor);
    }
}

struct Node* queue_insertNode(struct Queue* queue, struct Node* node, int value) {
    struct Node* newNode = node_new(value);
    if (node == queue->head) { //The new node becomes the head
        newNode->next = node;
        newNode->prev = NULL;
        node->prev = newNode;
        queue->head = newNode;
    } else { //The new node is placed in the middle of to existing nodes
        newNode->next = node;
        newNode->prev = node->prev;

        node->prev->next = newNode;
        node->prev = newNode;
    }

    return newNode;
}

void queue_removeNode(struct Queue* queue, struct Node* node) {
    if (node == queue->tail) {
        printf("Can't remove tail from queue");
        exit(23);
    }
    if (node == queue->head) { //Changes the head of the queue
        queue->head = node->next;
        node->next->prev = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    free(node);
}

int queue_pop(struct Queue* queue) {
    int value = queue->head->value;
    queue_removeNode(queue, queue->head);
    return value;
}

//Node funksjoner
struct Node* node_new(int value) {
    struct Node* node = malloc(sizeof(node));
    node->value = value;
    node->next = NULL;
    node->prev = NULL;

    return node;
}
