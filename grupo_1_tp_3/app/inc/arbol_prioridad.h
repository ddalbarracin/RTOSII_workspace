/*
 * arbol_prioridad.h
 *
 *  Created on: Aug 10, 2024
 *      Author: flealu
 */

#ifndef INC_ARBOL_PRIORIDAD_H_
#define INC_ARBOL_PRIORIDAD_H_

#include "cmsis_os.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define maximum size of the priority queue
#define MAX 10

typedef struct {
    int items[MAX];
    int size;
} PriorityQueue;

_Bool enqueue(PriorityQueue* pq, int value);
int dequeue(PriorityQueue* pq);
_Bool PriorityQueueCreate(PriorityQueue *pq);


#endif /* INC_ARBOL_PRIORIDAD_H_ */
