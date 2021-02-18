#include "queue.h"


//Queue funksjoner
void addToQueue(struct Queue* queue, struct Elevator* elev, int floor) {
    if (isEmpty(queue)) {
        
    }
}



int isEmpty(struct Queue* queue);
struct Node* getHead(struct Queue* queue);
int popQueue(struct Queue* queue);

//Node funksjoner
struct Node* getPrev(struct Node* node);
struct Node* getNext(struct Node* node);
int getValue(struct Node* node);
void deleteNode(struct Node* node);