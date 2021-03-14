#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "structs.h"
#include "time.h"
#include "queue.h"
#include "hardware.h"
#include "timer.h"
#include "fsm.h"
#include <stdlib.h>



void elevator_init(struct Elevator* elev);
void elevator_loop(struct Elevator* elev);

void elevator_getOrder(struct Elevator* elev);

void elevator_update(struct Elevator* elev);

void elevator_reachedFloor(struct Elevator* elev, int floor);

void elevator_OnStopButton(struct Elevator* elev);

void elevator_setNextFloor(struct Elevator* elev, int floor);









#endif
