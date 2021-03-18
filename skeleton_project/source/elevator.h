/**
 * @file
 * @brief Elevator-modulen til heissystemet. Henter alle input fra brukeren og setter alle lys. 
 * Tar seg av bevegelseslogikk og lagrer data om heisen.
 */
#ifndef ELEVATOR_H
#define ELEVATOR_H
#include "time.h"
#include "queue.h"
#include "hardware.h"
#include "timer.h"
#include "fsm.h"
#include <stdlib.h>

/**
 * @brief Enum for hvilke tilstander heisen kan være i
 */
enum State{ 
    INIT, /**< Heisen har nettopp startet og beveger seg til en definert tilstand */
    RUNNING, /**< Heisen er i vanlig drift og beveger seg */
    STILL, /**< Heisen er i vanlig drift og står stille */
    OPEN, /**< Heisen er i vanlig drift og har døra åpen */
    OBSTRUCTED, /**< Det er en obstruksjon i døråpningen og heisen står åpen*/
    EMERGENCY /**< Stopp-knappen er trykket og heisen står stille uten å ta imot ordre */
    
};

/**
 * @brief Struct for Elevator-modulen
 */
struct Elevator{
    /*@{*/
    enum State currentState; /**< Tilstanden til heisen */
    struct Queue* p_queue; /**< Kømodulen til heisen */
    int direction; /**< Retningen heisen beveger seg i */
    int lastFloor; /**< Siste etasje heisen kjørte forbi */
    int targetFloor; /**< Etasjen heisen skal til */
    int obstruction; /**< Obstruksjonsbryteren, 1 = på, 0 = av*/
    int stopButton; /**< Stoppknappen, 1 = på, 0 = av*/
    int startTime; /**< Tiden timeren sist ble startet på, starter på systemtiden*/
    int onFloor; /**< Bool som beskriver om står på en etasje eller ikke, 1 = på, 0 = av */
    /*@}*/
};

/**
 * @brief Metode for å initialisere alle variabler i Elevator structet.
 *
 * @param[in,out] p_elev Elevator struct som skal initialiseres.
 */
void elevator_init(struct Elevator* p_elev);

/**
 * @brief Metode for hovedloopen heisen kjører under drift.
 *
 * @param[in,out] p_elev Elevator struct metoden hører til.
 */
void elevator_loop(struct Elevator* p_elev);

/**
 * @brief Metode som bestemmer heisens oppførsel basaert på hvilken tilstand den er i.
 * Denne metoden håndterer alt som har med heisens bevegelse å gjøre.
 *
 * @param[in,out] p_elev Elevator struct metoden hører til.
 */
void elevator_performState(struct Elevator* p_elev);


/**
 * @brief Metode som kalles for å hente inn bestilling fra brukeren. Kaller queue_addOrder().
 *
 * @param[in,out] p_elev Elevator struct metoden hører til.
 */
void elevator_getOrder(struct Elevator* p_elev);

/**
 * @brief Metode for å oppdatere structets variabler, inkludert @c p_queue og @c state . 
 * Dersom endringen av variablene hører sammen med at lys skrur seg av eller på blir dette gjort gjennom denne funksjonen og 
 * dens kall på elevator_reachedFloor() og elevator_onStopButton().
 *
 * @param[in,out] p_elev Elevator struct metoden hører til.
 */
void elevator_update(struct Elevator* p_elev);

/**
 * @brief Metode som kalles når heisen når en etasje. 
 * Dersom dette er en etasje heisen skal stoppe på slukkes alle lys forbundet med etasjen. I tilegg fjernes bestillinger forbundet til etasjen ved hjelp av queue_clearOrder().
 *
 * @param[in,out] p_elev Elevator struct metoden hører til.
 */
void elevator_reachedFloor(struct Elevator* p_elev, int floor);

/**
 * @brief Metode som kalles når stoppknappen er trykket inn. 
 * Fjerner alle bestillinger fra køen og slukekr tilhørende lys og setter stopp-lyset høyt.
 * I tilegg stopper metoden bevegelsen til heisen. Dette gjøres også i perform state dersom stopp-knappen er trykket, men er en ekstra forsikring.
 *
 * @param[in,out] p_elev Elevator struct metoden hører til.
 */
void elevator_onStopButton(struct Elevator* p_elev);










#endif
