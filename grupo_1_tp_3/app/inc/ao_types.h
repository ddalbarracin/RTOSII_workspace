/*
 * ao_types.h
 *
 *  Created on: Jul 19, 2024
 *      Author: ddalbarracin
 */

#ifndef INC_AO_TYPES_H_
#define INC_AO_TYPES_H_
/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "arbol_prioridad.h"
/********************** typedef **********************************************/

typedef enum
{

	AO_LED_MESSAGE_PULSE,
	AO_LED_MESSAGE_SHORT,
	AO_LED_MESSAGE_LONG



} ao_led_message_t;

typedef struct
{
	QueueHandle_t hqueue;   // COLA PARA UI
} ao_handle_ui_t;

typedef struct
{
	PriorityQueue* pq;   // COLA PARA UI
} ao_handle_led_t;


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* INC_AO_TYPES_H_ */
