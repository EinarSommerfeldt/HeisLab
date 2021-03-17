/**
 * @file
 * @brief Fsm-modulen til heissystemet. Inneholder logikken for å endre tilstanden til heisen
 */
#ifndef FSM_H
#define FSM_H
#include "elevator.h"


/**
 * @brief Metode for å oppdatere tilstanden @c state til heisen. 
 *
 * @param[in,out] elev Elevator struct metoden oppdaterer tilstanden til.
 */
void fsm_update(struct Elevator* elev); 



#endif