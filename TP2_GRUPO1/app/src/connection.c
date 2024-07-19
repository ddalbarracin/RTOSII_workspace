/*
 * Copyright (c) 2024 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provled_messageed that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provled_messageed with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVled_messageED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCled_messageENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 */

/********************** inclusions *******************************************/
#include "connection.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (5000)

/********************** internal data declaration ****************************/

extern QueueHandle_t hqueue_r;
extern QueueHandle_t hqueue_g;
extern QueueHandle_t hqueue_b;

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

int task_cnt_;

/********************** external data definition *****************************/

extern QueueHandle_t hqueue;

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

_Bool connection_new_connection(ao_led_message_t led_message) {
	QueueHandle_t hqueue_aux;

	LOGGER_INFO("Ingresa nueva conexión: %d", led_message);

	switch (led_message) {

	case AO_LED_MESSAGE_PULSE:

		hqueue_aux = hqueue_r;

		break;

	case AO_LED_MESSAGE_SHORT:

		hqueue_aux = hqueue_g;

		break;

	case AO_LED_MESSAGE_LONG:

		hqueue_aux = hqueue_b;

		break;

	default:

		hqueue_aux = NULL;

		break;
	}

	if (NULL != hqueue_aux) {

		if (true == ao_led_send(hqueue_aux, led_message)) {
			if (0 == task_cnt_) {
				ao_led_init(hqueue_aux);
			}
			LOGGER_INFO("Nueva tarea para la conexión %d", led_message);
			return true;
		} else if (ao_led_init(hqueue_aux)) {
			if (pdPASS == ao_led_send(hqueue_aux, led_message)) {
				LOGGER_INFO("Nueva tarea para la conexión %d", led_message);
				return true;
			} else {
				LOGGER_INFO(
						"Conexión %d, Error: ¿Cuándo se podría dar este error?",
						led_message);
			}
		} else {
			LOGGER_INFO("Conexión %d, Error: No se puede crear más recursos",
					led_message);
		}
		LOGGER_INFO("Conexión %d, Error: No hay más lugar en la cola",
				led_message);

	} else {
		LOGGER_INFO("ERROR: hqueue_aux = NULL");
	}

	return false;
}

/********************** end of file ******************************************/
