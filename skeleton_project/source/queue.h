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
 * @return Returnerer a pointer to the new queue object.
 */
struct Queue* queue_new();


/**
 * @brief Adds a new floor to the queue
 *
 * @param queue[in,out] Queue the floor will be added to.
 * @param elev[in] Elevator that owns the queue.
 * @param floor[in] Floor to be added.
 */
void queue_addToQueue(struct Queue* queue, struct Elevator* elev, int floor);

/**
 * @brief Adds a node in the queue in front of the input node.
 *
 * @param queue[in,out] Queue the floor will be added to.
 * @param node[in,out] Node to be inserted.
 */
void queue_insertNode(struct Queue* queue, struct Node* node);

/**
 * @brief Removes a node from the queue, @ref deleteNode(struct Node* node) 
 * takes care of the memory deallocation.
 *
 * @param queue[in,out] Queue the node will be removed from.
 * @param node[in,out] Node to be removed.
 * 
 * @return returns the @p value of the removed node.
 */
int queue_removeNode(struct Queue* queue, struct Node* node);

/**
 * @brief Checks if the queue is empty.
 *
 * @param queue[in] Queue to be checked.
 * 
 * @return 1 if the queue is empty.
 */
int queue_isEmpty(struct Queue* queue);

/**
 * @brief Returns a pointer to the head of the queue.
 *
 * @param queue[in] Queue struct.
 * 
 * @return A pointer to the first node in the queue.
 */
struct Node* queue_getHead(struct Queue* queue);

/**
 * @brief Returns a pointer to the tail of the queue.
 * @warning This node is a dummy and serves only to point to the other nodes. To get the last viable floor use node->next
 *
 * @param queue[in] Queue struct.
 * 
 * @return A pointer to the last node in the queue.
 */
struct Node* queue_getTail(struct Queue* queue);

/**
 * @brief Removes the first node in the queue and returns it's value
 *
 * @param queue[in] Queue struct.
 * 
 * @return Value of the first node.
 */
int queue_popQueue(struct Queue* queue); 

//Node funksjoner
struct Node* getPrev(struct Node* node);
struct Node* getNext(struct Node* node);
int getValue(struct Node* node);
void deleteNode(struct Node* node);








































#endif