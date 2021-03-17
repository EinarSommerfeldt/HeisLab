/**
 * @file
 * @brief Elevator-modulen til heissystemet. Henter alle input fra brukeren og setter alle lys. 
 * Tar seg av bevegelseslogikk og lagrer data om heisen.
 */
#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "structs.h"
#include "time.h"
#include "queue.h"
#include "hardware.h"
#include "timer.h"
#include "fsm.h"
#include <stdlib.h>


/**
 * @brief Metode for å initialisere alle variabler i Elevator structet.
 *
 * @param[in,out] elev Elevator struct som skal initialiseres.
 */
void elevator_init(struct Elevator* elev);

/**
 * @brief Metode for hovedloopen heisen kjører under drift.
 *
 * @param[in,out] elev Elevator struct metoden hører til.
 */
void elevator_loop(struct Elevator* elev);

/**
 * @brief Metode for heisens oppførsel basaert på hvilken tilstand den er i.
 *
 * @param[in,out] elev Elevator struct metoden hører til.
 */
void elevator_performState(struct Elevator* elev);


/**
 * @brief Metode som kalles for å hente inn bestilling fra brukeren. Kaller queue_addOrder().
 *
 * @param[in,out] elev Elevator struct metoden hører til.
 */
void elevator_getOrder(struct Elevator* elev);

/**
 * @brief Metode for å oppdatere structets variabler, inkludert @c queue og @c state . 
 * Dersom endringen av variablene hører sammen med at lys skrur seg av eller på blir dette gjort gjennom denne funksjonen og 
 * dens kall på elevator_reachedFloor() og elevator_onStopButton().
 *
 * @param[in,out] elev Elevator struct metoden hører til.
 */
void elevator_update(struct Elevator* elev);

/**
 * @brief Metode som kalles når heisen når en etasje. 
 * Dersom dette er en etasje heisen skal stoppe på slukkes alle lys forbundet med etasjen. I tilegg fjernes bestillinger forbundet til etasjen ved hjelp av queue_clearOrder().
 *
 * @param[in,out] elev Elevator struct metoden hører til.
 */
void elevator_reachedFloor(struct Elevator* elev, int floor);

/**
 * @brief Metode som kalles når stoppknappen er trykket inn. 
 * Fjerner alle bestillinger fra køen og slukekr tilhørende lys og setter stopp-lyset høyt.
 * I tilegg stopper metoden bevegelsen til heisen. Dette gjøres også i perform state dersom stopp-knappen er trykket, men er en ekstra forsikring.
 *
 * @param[in,out] elev Elevator struct metoden hører til.
 */
void elevator_onStopButton(struct Elevator* elev);










#endif
