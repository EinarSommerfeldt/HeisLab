/**
 * @file
 * @brief Queue-system for the elevator
 */

#include "hardware.h"
#include "fsm.h"
#include "elevator.h"

#ifndef QUEUE_H
#define QUEUE_H




typedef struct  {
    ElevatorOrder orders[4];

} Queue;




//Queue funksjoner------------------------------------------------------------------------------------------
/**
 * @brief Constructor for the Queue object.
 *
 * @return Pointer to the new Queue object.
 */
Queue* queue_new();

/**
 * @brief Method for adding an order to a queue.
 *
 * @warning Takes a floor not an index, starts counting at 1.
 * @param[in,out] queue Queue the order is added to.
 * @param[in] floor The floor the order came from.
 * @param[in] order Which direction was pressed.
 */
void queue_addOrder(Queue* queue, int floor, ElevatorOrder order);

/**
 * @brief Method for removing orders for a floor from a queue.
 *
 * @warning Takes a floor not an index, starts counting at 1.
 * @param[in,out] queue Queue the order is cleared from.
 * @param[in] floor The floor where orders are cleared.
 */
void queue_clearOrder(Queue* queue, int floor);

/**
 * @brief Returns the next floor the elevator should go to. 
 * Prioritizes floors in the direction the elevator is already going.
 * 
 * @param[in] queue Queue object owned by the elevator.
 * @param[in] lastFloor Last floor the elevator visited. Does not care if the elevator stopped there or not.
 * @param[in] currentDir The direction the elevator is travelling in.
 * 
 * @return Returns the next floor the elevator should go to as an int from 1-4. Returns 0 if there are no orders. 
 */
int queue_getNext(Queue* queue, int lastFloor, int currentDir);









































#endif