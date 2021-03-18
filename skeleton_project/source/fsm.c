#include "fsm.h"

void fsm_update(struct Elevator* p_elev)
{
    switch (p_elev->currentState)
    {
    case INIT:
        if (p_elev->lastFloor > -1) {
            p_elev->currentState = RUNNING;
            break;
        }
        break;
    case RUNNING:
        if (p_elev->stopButton) {
            p_elev->currentState = EMERGENCY;
        }  else if (p_elev->targetFloor == -1) {
            p_elev->currentState = STILL;
        } else if (p_elev->targetFloor == p_elev->lastFloor && p_elev->onFloor == 1) {
            p_elev->currentState = OPEN;
            timer_start(&p_elev->startTime);
        }
        break;
    case STILL:
        if (p_elev->stopButton) {
            p_elev->currentState = EMERGENCY;
        } else if (p_elev->targetFloor == p_elev->lastFloor && p_elev->onFloor == 1) {
            p_elev->currentState = OPEN;
        } else if (p_elev->targetFloor != -1) {
            p_elev->currentState = RUNNING;
        }
        break;
    case OPEN:
        if (p_elev->stopButton) {
            p_elev->currentState = EMERGENCY;
        } else if (p_elev->obstruction) {
            p_elev->currentState = OBSTRUCTED;
        } else if (timer_get(p_elev->startTime) > 2 ) {
            p_elev->currentState = RUNNING;
        }
        break;
    case OBSTRUCTED:
        if (p_elev->stopButton) {
            p_elev->currentState = EMERGENCY;
        } else if (!p_elev->obstruction) {
            p_elev->currentState = OPEN;
            timer_start(&p_elev->startTime);
        }
        break;
    case EMERGENCY:
        if (!p_elev->stopButton && p_elev->onFloor) {
            p_elev->currentState = OPEN;
            timer_start(&p_elev->startTime);
        } else if (!p_elev->stopButton) {
            p_elev->currentState = STILL;
        }
        break;
    } 
}