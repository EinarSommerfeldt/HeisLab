/**
 * @file
 * @brief Queue-modulen til heissystemet. Håndterer bestillingene heisen tar inn og hvilken rekkefølge de skal betjenes
 */
#ifndef QUEUE_H
#define QUEUE_H


/**
 * @brief Enum for typen bestillinger som kan ligge i køen
 */
enum ElevatorOrder{
    ORDER_NONE, /**< Ingen bestilling */
    ORDER_UP, /**< Bestilling oppover */
    ORDER_DOWN, /**< Bestilling nedover */
    ORDER_BOTH, /**< Bestilling begge veier */
    ORDER_INSIDE /**< Bestilling fra inne i heisen */
};

/**
 * @brief Struct for Queue-modulen
 */
struct Queue{
    /*@{*/
    enum ElevatorOrder orders[4]; /**< Liste med bestillinger, et element per etajse */
    /*@}*/
};

/**
 * @brief Konstruktør for Queue-objektet.
 *
 * @return Peker til det nye objektet.
 */
struct Queue* queue_new();

/**
 * @brief Metode for å legge til en ny bestilling i køen.
 *
 * @param[in,out] p_queue Køen bestillingen legges til i.
 * @param[in] floor Etasje bestillingen kom fra.
 * @param[in] order Hva slags type bestilling det var (UP, DOWN, INSIDE).
 */
void queue_addOrder(struct Queue* p_queue, int floor, enum ElevatorOrder order);

/**
 * @brief Metode for å fjerne alle bestillinger på en etasje fra køen.
 *
 * @param[in,out] p_queue Køen bestillingene fjernes fra.
 * @param[in] floor Etasje bestillingene skal fjernes fra.
 */
void queue_clearOrder(struct Queue* p_queue, int floor);

/**
 * @brief Metode for å finne neste bestilling heisen skal betjene.
 * Prioriterer bestillinger i samme retning som heisen allerede går.
 * 
 * @param[in] p_queue Kø-objektet heisen eier.
 * @param[in] lastFloor Siste etasje heisen var innom, bryr seg ikke om heisen stoppet der eller ikke.
 * @param[in] currentDir Retningen heisen kjører i.
 * 
 * @return Den neste etasjen heisen skal kjøre til som en int på intervallet 0-3. Returnerer -1 hvis køen er tom. 
 */
int queue_getNext(struct Queue* p_queue, int lastFloor, int currentDir);



























#endif