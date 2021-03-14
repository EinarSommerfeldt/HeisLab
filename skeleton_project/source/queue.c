/**
 * @file
 */
#include "queue.h"
#include "structs.h"
#include <stdio.h>


struct Queue* queue_new()
{
    struct Queue* queue = malloc(sizeof(struct Queue));
    for (int i = 0; i < 4; i++ ) {
        queue->orders[i] = ORDER_NONE;
    }

    return queue;
}

void queue_addOrder(struct Queue* queue, int floor, enum ElevatorOrder order) {
    enum ElevatorOrder prevOrder = queue->orders[floor];
    if (prevOrder == order || prevOrder == ORDER_NONE || order == ORDER_INSIDE) {
        queue->orders[floor] = order; 
    } else {
        queue->orders[floor] = ORDER_BOTH;
    }
}

void queue_clearOrder(struct Queue* queue, int floor) {
    queue->orders[floor] = ORDER_NONE;
}

int queue_getNext(struct Queue* queue, int lastFloor , int targetFloor, int currentDir) {
    if (lastFloor < 0) {
        return -1;
    }
    if (currentDir) { //The elevator is going up
        for (int i = lastFloor; i < 4; i++) {
            if(queue->orders[i] == ORDER_UP || queue->orders[i] == ORDER_INSIDE || queue->orders[i] == ORDER_BOTH) {
                return i;
            }
        }
        for (int i = 0; i < 4; i++) { 
            if(queue->orders[i] != ORDER_NONE) {
                return i;
            }
        }
    } else {    //The elevator is going down
        for (int i = lastFloor; i > -1; i--) { 
            if(queue->orders[i] == ORDER_DOWN || queue->orders[i] == ORDER_INSIDE || queue->orders[i] == ORDER_BOTH) {
                return i;
            }
        }
        for (int i = 0; i < 4; i++) { 
            if(queue->orders[i] != ORDER_NONE) {
                return i;
            }
        }
    }

    return -1;
}