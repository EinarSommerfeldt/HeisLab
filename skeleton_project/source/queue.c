/**
 * @file
 */
#include "queue.h"


//Queue funksjoner


Queue* queue_new()
{
    Queue* queue = malloc(sizeof(Queue));
    for (int i{0}; i < 4; i++ ) {
        queue->orders[i] = ORDER_NONE;
    }

    return queue;
}

void queue_addOrder(Queue* queue, int floor, ElevatorOrder order) {
    ElevatorOrder prevOrder = queue->orders[floor-1];
    if (prevOrder == order || prevOrder == ORDER_NONE || order ==ORDER_INSIDE) {
        queue->orders[floor-1] = order; 
    } else {
        queue->orders[floor-1] = ORDER_BOTH;
    }
}

void queue_clearOrder(Queue* queue, int floor) {
    queue->orders[floor-1] = ORDER_NONE;
}

int queue_getNext(Queue* queue, int latestFloor , int currentDir) {
    if (currentDir) { //The elevator is going up
        for (int i{latestFloor-1}; i < 4; i++) {
            if(queue->orders[i] == ORDER_UP || queue->orders[i] == ORDER_INSIDE || queue->orders[i] == ORDER_BOTH) {
                return i+1;
            }
        }
        for (int i{latestFloor-1}; i>-1; i--) {
            if (queue->orders[i] != ORDER_NONE) {
                return i+1;
            }
        }
    } else {    //The elevator is going down
        for (int i{latestFloor-1}; i > -1; i--) {
            if(queue->orders[i] == ORDER_DOWN || queue->orders[i] == ORDER_INSIDE || queue->orders[i] == ORDER_BOTH) {
                return i+1;
            }
        }
        for (int i{latestFloor-1}; i < 4; i++) {
            if(queue->orders[i] != ORDER_NONE) {
                return i+1;
            }
        }
    }
    return 0; //Returns 0 if there are no orders on any floors.
}