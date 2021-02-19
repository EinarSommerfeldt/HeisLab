/**
 * @file
 * @brief Queue-system for the elevator
 */

#include "hardware.h"
#include "fsm.h"

#ifndef QUEUE_H
#define QUEUE_H


struct Node {
    int value;
    struct Node* next;
    struct Node* prev;
};

struct Queue {
    struct Node* head;
    struct Node* tail; 
};



//Queue funksjoner------------------------------------------------------------------------------------------
/**
 * @brief Constructor for the queue object.
 *
 * @return Pointer to the new queue object.
 */
struct Queue* queue_new();


/**
 * @brief Adds a new floor to the queue
 *
 * @param queue[in,out] Queue the floor will be added to.
 * @param elev[in] Elevator that owns the queue.
 * @param floor[in] Floor to be added.
 * @param direction[in] Which direction the order is.
 */
void queue_addFloor(struct Queue* queue, struct Elevator* elev, int floor, int direction);

/**
 * @brief Adds a node in the queue in front of the input node.
 *
 * @param queue[in,out] Queue the floor will be added to.
 * @param node[in,out] Node to be inserted.
 * @param value[in] Value for the new node.
 * 
 * @return pointer to the new node 
 */
struct Node* queue_insertNode(struct Queue* queue, struct Node* node, int value);

/**
 * @brief Removes a node from the queue and frees the node from memory.
 *
 * @param queue[in,out] Queue the node will be removed from.
 * @param node[in,out] Node to be removed.
 * 
 */
void queue_removeNode(struct Queue* queue, struct Node* node);


/**
 * @brief Removes the first node in the queue and returns it's value
 *
 * @param queue[in] Queue struct.
 * 
 * @return Value of the first node.
 */
int queue_pop(struct Queue* queue); 

//Node funksjoner
struct Node* node_new(int value);








































#endif