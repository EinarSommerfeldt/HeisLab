/**
 * @file
 */
#include "queue.h"
#include <stdio.h>


struct Queue* queue_new()
{
    struct Queue* p_queue = malloc(sizeof(struct Queue));
    for (int i = 0; i < 4; i++ ) {
        p_queue->orders[i] = ORDER_NONE;
    }

    return p_queue;
}

void queue_addOrder(struct Queue* p_queue, int floor, enum ElevatorOrder order) {
    enum ElevatorOrder prevOrder = p_queue->orders[floor];
    if (prevOrder == order || prevOrder == ORDER_NONE || order == ORDER_INSIDE) {
        p_queue->orders[floor] = order; 
    } else {
        p_queue->orders[floor] = ORDER_BOTH;
    }
}

void queue_clearOrder(struct Queue* p_queue, int floor) {
    p_queue->orders[floor] = ORDER_NONE;
}

int queue_getNext(struct Queue* p_queue, int lastFloor, int currentDir) {
    if (lastFloor < 0) {
        return -1;
    }
    if (currentDir) { //The elevator is going up
        for (int i = lastFloor; i < 4; i++) {
            if(p_queue->orders[i] == ORDER_UP || p_queue->orders[i] == ORDER_INSIDE || p_queue->orders[i] == ORDER_BOTH) {
                return i;
            }
        }
        for (int i = 0; i < 4; i++) { 
            if(p_queue->orders[i] != ORDER_NONE) {
                return i;
            }
        }
    } else {    //The elevator is going down
        for (int i = lastFloor; i > -1; i--) { 
            if(p_queue->orders[i] == ORDER_DOWN || p_queue->orders[i] == ORDER_INSIDE || p_queue->orders[i] == ORDER_BOTH) {
                return i;
            }
        }
        for (int i = 0; i < 4; i++) { 
            if(p_queue->orders[i] != ORDER_NONE) {
                return i;
            }
        }
    }

    return -1;
}