#include "elevator.h"

void elevator_init(Elevator* elev) {
    elev->currentState = INIT;
    elev->queue = queue_new();
    elev->direction = 0;
    elev->lastFloor = 0;;
    elev->targetFloor = 0;
    elev->obstruction = 0;
    elev->stopButton = 0;
    elev->startTime = -3;
    elev->onFloor = 0;
}

void elevator_loop(Elevator* elev) {
    while (1) {
        elevator_update(elev);
        hardware_command_door_open(0);
        elev->targetFloor = queue_getNext(elev->queue, elev->lastFloor, elev->currentDir);
        switch (elev->currentState)
        {
        case INIT: //Will move up until it hits a floor
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            break;
        case RUNNING:
            elevator_getOrder(elev);
            if (elev->targetFloor > elev->lastFloor) {
                elev->direction = 1;
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
            } else {
                elev->direction = 0;
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
            }
            break;
        case STOP:
            hardware_command_movement(HARDWARE_MOVEMENT_STOP)
            break;
        case DOOR_OPEN:
            elevator_getOrder(elev);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP)
            hardware_command_door_open(1);
            break;
        case OBSTRUCTED:
            elevator_getOrder(elev);
            hardware_command_movement(HARDWARE_MOVEMENT_STOP)
            hardware_command_door_open(1);
            break;
        }
    }
}

void elevator_getOrder(Elevator* elev) {
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
            hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
            queue_addOrder(elev->queue, f+1, ORDER_INSIDE);
        }

        /* Orders going up */
        if(hardware_read_order(f, HARDWARE_ORDER_UP)){
            hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
            queue_addOrder(elev->queue, f+1, ORDER_UP);
        }

        /* Orders going down */
        if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
            hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
            queue_addOrder(elev->queue, f+1, ORDER_DOWN);
        }
    }

}

void elevator_update(Elevator* elev) {
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
        elevator_stop(elev)
    } else {
        hardware_command_stop_light(0);
    }

    fsm_update(elev);
}

void elevator_reachedFloor(Elevator* elev, int floor)
{
    elev->onFloor = 1;
    lastFloor = floor;
    if (targetFloor == lastFloor) {
        elev->queue->orders[floor] = ORDER_NONE;
        queue_clearOrder(elev->queue, floor);
    }
    hardware_command_floor_indicator_on(f);
    hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
    hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
    hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
}

void elevator_stop(Elevator* elev){
    hardware_command_stop_light(1);
    clear_all_order_lights();
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) {
        elev->queue->orders[f] = ORDER_NONE;
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);   
}

void elevator_setNextFloor(Elevator* elev, int floor){
    int requested_floor = floor; //get form hardware
    elev->targetFloor = requested_floor; //add queue logic later
}

void elevator_startTimer(Elevator* elev) {
    elev->startTime = clock()/CLOCKS_PER_SEC;
}


int elevator_getTimer(Elevator* elev) {
    return clock()/CLOCKS_PER_SEC - elev->startTime; 
}

