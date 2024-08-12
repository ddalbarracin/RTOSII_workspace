/*
 * arbol_prioridad.h
 *
 *  Created on: Aug 10, 2024
 *      Author: flealu
 */

#ifndef INC_ARBOL_PRIORIDAD_H_
#define INC_ARBOL_PRIORIDAD_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

// Define maximum size of the priority queue
#define MAX 10

typedef struct {
    int items[MAX];
    int size;
} PriorityQueue;

_Bool enqueue(PriorityQueue* pq, int value);
int dequeue(PriorityQueue* pq);
_Bool PriorityQueueCreate(PriorityQueue *pq);
void PriorityQueueDelete(PriorityQueue *pq);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* INC_ARBOL_PRIORIDAD_H_ */
