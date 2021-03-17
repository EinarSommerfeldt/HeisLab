/**
 * @file
 * @brief Queue-modulen til heissystemet. Håndterer bestillingene heisen tar inn og hvilken rekkefølge de skal betjenes
 */
#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "structs.h"



//Queue funksjoner------------------------------------------------------------------------------------------
/**
 * @brief Konstruktør for Kø-objektet.
 *
 * @return Peker til det nye objektet.
 */
struct Queue* queue_new();

/**
 * @brief Metode for å legge til en ny bestilling i køen.
 *
 * @param[in,out] queue Køen bestillingen legges til i.
 * @param[in] floor Etasje bestillingen kom fra.
 * @param[in] order Hva slags type bestilling det var (UP, DOWN, INSIDE).
 */
void queue_addOrder(struct Queue* queue, int floor, enum ElevatorOrder order);

/**
 * @brief Metode for å fjerne alle bestillinger på en etasje fra køen.
 *
 * @param[in,out] queue Køen bestillingene fjernes fra.
 * @param[in] floor Etasje bestillingene skal fjernes fra.
 */
void queue_clearOrder(struct Queue* queue, int floor);

/**
 * @brief Metode for å finne neste bestilling heisen skal betjene.
 * Prioriterer bestillinger i samme retning som heisen allerede går.
 * 
 * @param[in] queue Kø-objektet heisen eier.
 * @param[in] lastFloor Siste etasje heisen var innom, bryr seg ikke om heisen stoppet der eller ikke.
 * @param[in] currentDir Retningen heisen kjører i.
 * 
 * @return Den neste etasjen heisen skal kjøre til som en int på intervallet 0-3. Returnerer -1 hvis køen er tom. 
 */
int queue_getNext(struct Queue* queue, int lastFloor, int currentDir);



























#endif