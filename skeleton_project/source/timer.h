/**
 * @file
 * @brief Timer-modulen til heissystemet. 
 */
#ifndef TIMER_H
#define TIMER_H
#include "time.h"

/**
 * @brief Metode for å starte timeren, setter verdien til pekeren lik systemtiden
 *
 * @param[out] startTime peker til variabelen som settes lik systemtiden
 */
void timer_start(int* startTime);

/**
 * @brief Metode for å få tiden det har gått siden timeren ble startet.
 *
 * @param[in] startTime Tiden timeren ble startet på.
 */
int timer_get(int startTime);



#endif 