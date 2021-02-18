#include "hardware.h"
#include "time.h"

enum State{
    UP,
    DOWN,
    STOP,
};

struct Elevator{
    enum State currentState;
    int lastFloor;
    int targetFloor;
    int timer;
    int open_door;
    int obstruction;
    int stop_button;
    int startTime;
};



void setNextFloor(struct Elevator* elev, int floor);


void stopElev();
void startTimer(struct Elevator* elev);
int getTimer(struct Elevator* elev);