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
        if (iterator%100 == 0) {
            printf("State: %d, timer: %d, target: %d\n", elev->currentState, timer_get(elev->startTime), elev->targetFloor);

        }  
        
        elevator_update(elev);
        elevator_performState(elev);
    }
}

void elevator_performState(struct Elevator* elev)
{
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
                if (!hardware_read_floor_sensor(3)) { //Elevator should not go out of bounds
                    hardware_command_movement(HARDWARE_MOVEMENT_UP);
                }
            } else {
                elev->direction = 0;
                if (!hardware_read_floor_sensor(0)) { //Elevator should not go out of bounds
                    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                }
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
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            if (elev->onFloor) {
                hardware_command_door_open(1);
            } else {
                hardware_command_door_open(0);
            }
            break;
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
    //Stop button
    elev->stopButton = hardware_read_stop_signal();
    if (hardware_read_stop_signal()) {
        elevator_onStopButton(elev);
    } else {
        hardware_command_stop_light(0);
    }
    //updates state machine before taking new orders incase new state i emergency
    fsm_update(elev);

    
    if (elev->currentState != EMERGENCY && elev->currentState != INIT) {
        elevator_getOrder(elev);
    }

    elev->targetFloor = queue_getNext(elev->queue, elev->lastFloor, elev->direction);  
}

void elevator_reachedFloor(struct Elevator* elev, int floor)
{
    elev->onFloor = 1;
    elev->lastFloor = floor;
    if (elev->targetFloor == elev->lastFloor) { //clears all lights if the floor is stoppped on
        elev->queue->orders[floor] = ORDER_NONE;
        queue_clearOrder(elev->queue, floor);
        hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
    }
    hardware_command_floor_indicator_on(floor);
    
}

void elevator_onStopButton(struct Elevator* elev){
    hardware_command_stop_light(1);
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){ //clears all order lights
        for(int i = 0; i < 3; i++){
            HardwareOrder type = (HardwareOrder)i;
            hardware_command_order_light(f, type, 0);
        }
    }
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) { //clears all orders
        elev->queue->orders[f] = ORDER_NONE;
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);   
}



