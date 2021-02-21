#include "hardware.h"
#include "time.h"
#include "fsm.h"
#include "queue.h"

#ifndef ELEVATOR_H
#define ELEAVTOR_H

typedef enum {
    ORDER_NONE,
    ORDER_UP,
    ORDER_DOWN,
    ORDER_BOTH,
    ORDER_INSIDE
} ElevatorOrder;

typedef enum{
    INIT,
    RUNNING,
    STOP,
    OPEN,
    OBSTRUCTED
} State;


typedef struct {
    State currentState;
    Queue* queue;
    int direction; //1 opp 0 ned
    int lastFloor;
    int targetFloor;
    int timer;
    int doorButton;
    int obstruction;
    int stopButton;
    int startTime;
    int onFloor; //1 if elevator is on a floor 
} Elevator;

void elevator_loop(Elevator* elev);
void elevator_getOrder(Elevator* elev)
void elevator_configure(Elevator* elev);
void elevator_update(Elevator* elev);
void elevator_reachedFloor(Elevator* elev, int floor);

void elevator_setNextFloor(Elevator* elev, int floor);
void elevator_stop(Elevator* elev);
void elevator_startTimer(Elevator* elev);
int elevator_getTimer(Elevator* elev);






















#endif