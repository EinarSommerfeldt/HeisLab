#include "elevator.h"
#include <stdio.h>

void elevator_init(struct Elevator* elev) {
    elev->currentState = INIT;
    elev->queue = queue_new();
    elev->direction = 0;
    elev->lastFloor = -1;;
    elev->targetFloor = -1;
    elev->obstruction = 0;
    elev->stopButton = 0;
    elev->startTime = 0;
    elev->onFloor = 0;
}

void elevator_loop(struct Elevator* elev) {
    int iterator = 0;
    while (1) {
        
        iterator++;
        if (iterator%19 == 0) {
            printf("State: %d, timer: %d, target: %d\n", elev->currentState, elevator_getTimer(elev), elev->targetFloor);
        } 
        
        elevator_update(elev);
        
        switch (elev->currentState)
        {
        case INIT: //Will move up until it hits a floor
            hardware_command_door_open(0);
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            break;
        case RUNNING:
            hardware_command_door_open(0);
            if (elev->targetFloor > elev->lastFloor) {
                elev->direction = 1;
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            } else {
                elev->direction = 0;
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            }
            break;
        case STILL:
            hardware_command_door_open(0);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            break;
        case OPEN:
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            hardware_command_door_open(1);
            break;
        case OBSTRUCTED:
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            hardware_command_door_open(1);
            break;
        case EMERGENCY:
            if (elev->onFloor) {
                hardware_command_door_open(1);
            } else {
                hardware_command_door_open(0);
            }
            break;
        } 
    }
}

void elevator_getOrder(struct Elevator* elev) {
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
            hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
            queue_addOrder(elev->queue, f, ORDER_INSIDE);
        }

        /* Orders going up */
        if(hardware_read_order(f, HARDWARE_ORDER_UP)){
            hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
            queue_addOrder(elev->queue, f, ORDER_UP);
        }

        /* Orders going down */
        if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
            hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
            queue_addOrder(elev->queue, f, ORDER_DOWN);
        }
    }

}

void elevator_update(struct Elevator* elev) {
    //Obstruction
    elev->obstruction = hardware_read_obstruction_signal();
    
    //Logic for reaching a floor
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_floor_sensor(f)){
            elevator_reachedFloor(elev, f);
            break;
        }
        elev->onFloor = 0;
    }
    //Stopp button
    elev->stopButton = hardware_read_stop_signal();
    if (hardware_read_stop_signal()) {
        elevator_OnStopButton(elev);
    } else {
        hardware_command_stop_light(0);
    }
    
    fsm_update(elev);
    elev->targetFloor = queue_getNext(elev->queue, elev->lastFloor, elev->targetFloor, elev->direction);
    if (elev->currentState != EMERGENCY && elev->currentState != INIT) {
        elevator_getOrder(elev);
    }
}

void elevator_reachedFloor(struct Elevator* elev, int floor)
{
    elev->onFloor = 1;
    elev->lastFloor = floor;
    if (elev->targetFloor == elev->lastFloor) {
        elev->queue->orders[floor] = ORDER_NONE;
        queue_clearOrder(elev->queue, floor);
        hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
    }
    hardware_command_floor_indicator_on(floor);
    
}

void elevator_OnStopButton(struct Elevator* elev){
    hardware_command_stop_light(1);
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        for(int i = 0; i < 3; i++){
            HardwareOrder type = (HardwareOrder)i;
            hardware_command_order_light(f, type, 0);
        }
    }
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
        elev->queue->orders[f] = ORDER_NONE;
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);   
}

void elevator_setNextFloor(struct Elevator* elev, int floor){
    int requested_floor = floor; //get form hardware
    elev->targetFloor = requested_floor; //add queue logic later
}

void elevator_startTimer(struct Elevator* elev) {
    elev->startTime = clock()/CLOCKS_PER_SEC;
}


int elevator_getTimer(struct Elevator* elev) {
    return clock()/CLOCKS_PER_SEC - elev->startTime; 
}


void fsm_update(struct Elevator* elev)
{
    switch (elev->currentState)
    {
    case INIT:
        if (elev->lastFloor > -1) {
            elev->currentState = RUNNING;
            break;
        }
        break;
    case RUNNING:
        if (elev->stopButton) {
            elev->currentState = EMERGENCY;
        }  else if (elev->targetFloor == -1) {
            elev->currentState = STILL;
        } else if (elev->targetFloor == elev->lastFloor) {
            elev->currentState = OPEN;
            elevator_startTimer(elev);
        }
        break;
    case STILL:
        if (elev->stopButton) {
            elev->currentState = EMERGENCY;
        } else if (elev->targetFloor != -1) {
            elev->currentState = RUNNING;
        }
        break;
    case OPEN:
        if (elev->stopButton) {
            elev->currentState = EMERGENCY;
        } else if (elev->obstruction) {
            elev->currentState = OBSTRUCTED;
        } else if (elevator_getTimer(elev) > 2 ) {
            elev->currentState = RUNNING;
        }
        break;
    case OBSTRUCTED:
        if (elev->stopButton) {
            elev->currentState = EMERGENCY;
        } else if (!elev->obstruction) {
            elev->currentState = OPEN;
            elevator_startTimer(elev);
        }
        break;
    case EMERGENCY:
        if (!elev->stopButton && elev->onFloor) {
            elev->currentState = OPEN;
            elevator_startTimer(elev);
        } else if (!elev->stopButton) {
            elev->currentState = RUNNING;
        }
        break;
    } 
}