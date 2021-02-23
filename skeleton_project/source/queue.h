/**
 * @file
 * @brief Queue-system for the elevator
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "structs.h"



//Queue funksjoner------------------------------------------------------------------------------------------
/**
 * @brief Constructor for the Queue object.
 *
 * @return Pointer to the new Queue object.
 */
struct Queue* queue_new();

/**
 * @brief Method for adding an order to a queue.
 *
 * @param[in,out] queue Queue the order is added to.
 * @param[in] floor The floor the order came from.
 * @param[in] order Which direction was pressed.
 */
void queue_addOrder(struct Queue* queue, int floor, enum ElevatorOrder order);

/**
 * @brief Method for removing orders for a floor from a queue.
 *
 * @param[in,out] queue Queue the order is cleared from.
 * @param[in] floor The floor where orders are cleared.
 */
void queue_clearOrder(struct Queue* queue, int floor);

/**
 * @brief Returns the next floor the elevator should go to. 
 * Prioritizes floors in the direction the elevator is already going.
 * 
 * @param[in] queue Queue object owned by the elevator.
 * @param[in] lastFloor Last floor the elevator visited. Does not care if the elevator stopped there or not.
 * @param[in] currentDir The direction the elevator is travelling in.
 * 
 * @return Returns the next floor the elevator should go to as an int from 0-3. Returns -1 if there are no orders. 
 */
int queue_getNext(struct Queue* queue, int lastFloor, int targetFloor, int currentDir);



























#endif