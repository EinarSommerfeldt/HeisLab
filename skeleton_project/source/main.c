#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "elevator.h"
#include "queue.h"
#include "time.h"


int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    struct Elevator elev;
    elevator_init(&elev);

    elevator_loop(&elev);
    
    return 0;
}
