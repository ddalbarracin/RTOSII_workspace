
#include "arbol_prioridad.h"

SemaphoreHandle_t Sem_priority_Queue_hndlr;


// Define swap function to swap two integers
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Define heapifyUp function to maintain heap property
// during insertion
// por cada valor ingresado verifica si es de mayor prioridad
// y hace el intercambio con swap
void heapifyUp(PriorityQueue *pq, int index)
{
	if (index && pq->items[(index - 1) / 2] > pq->items[index]) {

		swap(&pq->items[(index - 1) / 2], &pq->items[index]);
		heapifyUp(pq, (index - 1) / 2); //recursividad

	}
}

// Define enqueue function to add an item to the queue
_Bool enqueue(PriorityQueue *pq, int value) {

	taskENTER_CRITICAL();
	{

		if (pq->size == MAX) {
			return false;
		}

		pq->items[pq->size++] = value;

		heapifyUp(pq, pq->size - 1);

	}
	taskEXIT_CRITICAL();
	xSemaphoreGive(Sem_priority_Queue_hndlr);

	return (true);

}


// Define heapifyDown function to maintain heap property
// during deletion
// al sacar el elemento items[0] reacomoda el arbol
void heapifyDown(PriorityQueue *pq, int index)
{
	int smallest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left < pq->size && pq->items[left] < pq->items[smallest])
		smallest = left;

	if (right < pq->size && pq->items[right] < pq->items[smallest])
		smallest = right;

	if (smallest != index) {
		swap(&pq->items[index], &pq->items[smallest]);
		heapifyDown(pq, smallest);
	}

}


// Define dequeue function to remove an item from the queue
// saca el lemento de mayor prioridadque esta en items[0]
int dequeue(PriorityQueue *pq)
{
	int item = 0;
	xSemaphoreTake(Sem_priority_Queue_hndlr, portMAX_DELAY);
	{
		taskENTER_CRITICAL();
		{

			if (!pq->size) {
				item = -1;
			}

			item = pq->items[0];
			pq->items[0] = pq->items[--pq->size];
			heapifyDown(pq, 0);

		}
		taskEXIT_CRITICAL();
	}

	LOGGER_INFO("		dequeue: %d", item);

	return (item);
}


// Define peek function to get the top item from the queue
int peek(PriorityQueue *pq) {
	if (!pq->size) {
		return -1;
	}
	return pq->items[0];
}


_Bool PriorityQueueCreate(PriorityQueue *pq) {


	Sem_priority_Queue_hndlr = xSemaphoreCreateCounting(MAX, 0);
	configASSERT(NULL != Sem_priority_Queue_hndlr);
	vQueueAddToRegistry(Sem_priority_Queue_hndlr, "Sem_priority_Queue_hndlr");

	return (true);
}


