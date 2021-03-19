#include "timer.h"


void timer_start(int* p_startTime) {
    *p_startTime = time(NULL);
}


int timer_get(int startTime) {
    return time(NULL) - startTime; 
}