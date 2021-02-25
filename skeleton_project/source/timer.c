#include "timer.h"


void timer_start(int* startTime) {
    *startTime = time(NULL);
}


int timer_get(int* startTime) {
    return time(NULL) - *startTime; 
}