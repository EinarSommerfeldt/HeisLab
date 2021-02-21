#include "fsm.h"

void fsm_update(Elevator* elev)
{
    switch (elev->currentState)
    {
    case INIT:
        if (elev->lastFloor != 0) {
            elev->currentState = RUNNING
            break;
        }
        break;
    case RUNNING:
        if (elev->stopButton) {
            elev->currentState = STOP;
            break;
        } else if (elev->onFloor && elev->lastFloor == targetFloor) {
            elev->currentState = DOOR_OPEN;
            break;
        }
        break;
    case STOP:
        if (onFloor) {
            elev->currentState = DOOR_OPEN;
            elev_startTimer(elev);
            break;
        } else if (!elev->stopButton) {
            elev->currentState = RUNNING;
            break;
        }
        break;
    case DOOR_OPEN: 
        if (elev->obstructed) {
            elev->currentState = OBSTRUCTED;
            break;
        } else if (elevator_getTimer(elev) > 3 ) {
            elev->currentState = RUNNING;
            break,
        }
        break;
    case OBSTRUCTED:
        if (!elev->obstructed) {
            elev->currentState = DOOR_OPEN;
            elev_startTimer(elev);
            break;
        }
        break;
    }
}