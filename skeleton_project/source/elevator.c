#include "elevator.h"


void elevator_init(struct Elevator* p_elev) {
    p_elev->currentState = INIT;
    p_elev->p_queue = queue_new();
    p_elev->direction = 0;
    p_elev->lastFloor = -1;;
    p_elev->targetFloor = -1;
    p_elev->obstruction = 0;
    p_elev->stopButton = 0;
    p_elev->startTime = 0;
    p_elev->onFloor = 0;
}

void elevator_loop(struct Elevator* p_elev) {
    int iterator = 0;
    while (1) {
        elevator_update(p_elev);
        elevator_performState(p_elev);
    }
}

void elevator_performState(struct Elevator* p_elev)
{
    switch (p_elev->currentState)
        {
        case INIT: //Will move up until it hits a floor
            hardware_command_door_open(0);
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            break;
        case RUNNING:
            hardware_command_door_open(0);
            if (p_elev->targetFloor > p_elev->lastFloor) {
                p_elev->direction = 1;
                if (!hardware_read_floor_sensor(3)) { //Elevator should not go out of bounds
                    hardware_command_movement(HARDWARE_MOVEMENT_UP);
                }
            } else {
                p_elev->direction = 0;
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
            if (p_elev->onFloor) {
                hardware_command_door_open(1);
            } else {
                hardware_command_door_open(0);
            }
            break;
        } 
}

void elevator_getOrder(struct Elevator* p_elev) {
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_order(f, HARDWARE_ORDER_INSIDE)){
            hardware_command_order_light(f, HARDWARE_ORDER_INSIDE, 1);
            queue_addOrder(p_elev->p_queue, f, ORDER_INSIDE);
        }

        /* Orders going up */
        if(hardware_read_order(f, HARDWARE_ORDER_UP)){
            hardware_command_order_light(f, HARDWARE_ORDER_UP, 1);
            queue_addOrder(p_elev->p_queue, f, ORDER_UP);
        }

        /* Orders going down */
        if(hardware_read_order(f, HARDWARE_ORDER_DOWN)){
            hardware_command_order_light(f, HARDWARE_ORDER_DOWN, 1);
            queue_addOrder(p_elev->p_queue, f, ORDER_DOWN);
        }
    }

}

void elevator_update(struct Elevator* p_elev) {
    //Obstruction
    p_elev->obstruction = hardware_read_obstruction_signal();
    
    //Logic for reaching a floor
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){
        if(hardware_read_floor_sensor(f)){
            elevator_reachedFloor(p_elev, f);
            break;
        }
        p_elev->onFloor = 0;
    }
    //Stop button
    p_elev->stopButton = hardware_read_stop_signal();
    if (hardware_read_stop_signal()) {
        elevator_onStopButton(p_elev);
    } else {
        hardware_command_stop_light(0);
    }
    //updates state machine before taking new orders incase new state i emergency
    fsm_update(p_elev);

    
    if (p_elev->currentState != EMERGENCY && p_elev->currentState != INIT) {
        elevator_getOrder(p_elev);
    }

    p_elev->targetFloor = queue_getNext(p_elev->p_queue, p_elev->lastFloor, p_elev->direction);  
}

void elevator_reachedFloor(struct Elevator* p_elev, int floor)
{
    p_elev->onFloor = 1;
    p_elev->lastFloor = floor;
    if (p_elev->targetFloor == p_elev->lastFloor) { //clears all lights if the floor is stoppped on
        p_elev->p_queue->orders[floor] = ORDER_NONE;
        queue_clearOrder(p_elev->p_queue, floor);
        hardware_command_order_light(floor, HARDWARE_ORDER_DOWN, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_UP, 0);
        hardware_command_order_light(floor, HARDWARE_ORDER_INSIDE, 0);
    }
    hardware_command_floor_indicator_on(floor);
    
}

void elevator_onStopButton(struct Elevator* p_elev){
    hardware_command_stop_light(1);
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++){ //clears all order lights
        for(int i = 0; i < 3; i++){
            HardwareOrder type = (HardwareOrder)i;
            hardware_command_order_light(f, type, 0);
        }
    }
    for(int f = 0; f < HARDWARE_NUMBER_OF_FLOORS; f++) { //clears all orders
        p_elev->p_queue->orders[f] = ORDER_NONE;
    }
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);   
}



