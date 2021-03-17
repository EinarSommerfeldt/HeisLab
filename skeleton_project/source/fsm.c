#include "fsm.h"

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
        } else if (elev->targetFloor == elev->lastFloor && elev->onFloor == 1) {
            elev->currentState = OPEN;
            timer_start(&elev->startTime);
        }
        break;
    case STILL:
        if (elev->stopButton) {
            elev->currentState = EMERGENCY;
        } else if (elev->targetFloor == elev->lastFloor && elev->onFloor == 1) {
            elev->currentState = OPEN;
        } else if (elev->targetFloor != -1) {
            elev->currentState = RUNNING;
        }
        break;
    case OPEN:
        if (elev->stopButton) {
            elev->currentState = EMERGENCY;
        } else if (elev->obstruction) {
            elev->currentState = OBSTRUCTED;
        } else if (timer_get(elev->startTime) > 2 ) {
            elev->currentState = RUNNING;
        }
        break;
    case OBSTRUCTED:
        if (elev->stopButton) {
            elev->currentState = EMERGENCY;
        } else if (!elev->obstruction) {
            elev->currentState = OPEN;
            timer_start(&elev->startTime);
        }
        break;
    case EMERGENCY:
        if (!elev->stopButton && elev->onFloor) {
            elev->currentState = OPEN;
            timer_start(&elev->startTime);
        } else if (!elev->stopButton) {
            elev->currentState = STILL;
        }
        break;
    } 
}