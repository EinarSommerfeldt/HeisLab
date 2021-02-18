#include "fsm.h"

void stopElev(){
    hardware_command_movement(HARDWARE_MOVEMENT_STOP); 
    //check if hardware_sal is included in hardware.h
}

void setNextFloor(struct Elevator* elev, int floor){
    int requested_floor = floor; //get form hardware
    elev->targetFloor = requested_floor; //add queue logic later
}

void startTimer(struct Elevator* elev) {
    elev->startTime = clock()/CLOCKS_PER_SEC;
}


int getTimer(struct Elevator* elev) {
    return clock()/CLOCKS_PER_SEC - elev->startTime; 
}
