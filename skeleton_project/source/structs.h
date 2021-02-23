#ifndef STRUCTS_H
#define STRUCTS_H

enum ElevatorOrder{
    ORDER_NONE,
    ORDER_UP,
    ORDER_DOWN,
    ORDER_BOTH,
    ORDER_INSIDE
};

enum State{
    INIT,
    RUNNING,
    STILL,
    OPEN,
    OBSTRUCTED,
    EMERGENCY
} ;

struct Queue{
    enum ElevatorOrder orders[4];
};

struct Elevator{
    enum State currentState;
    struct Queue* queue;
    int direction; //1 opp 0 ned
    int lastFloor;
    int targetFloor;
    int timer;
    int obstruction;
    int stopButton;
    int startTime;
    int onFloor; //1 if elevator is on a floor 
};



#endif