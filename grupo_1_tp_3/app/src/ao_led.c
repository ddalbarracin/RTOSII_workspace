/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
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
#include "ao_led.h"
#include "arbol_prioridad.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (5000)
#define QUEUE_LENGTH_            (10)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_led_message_t))
#define delay_time               (1000000)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/
ao_handle_led_t ao_led;
PriorityQueue xpq = {{0},0};



/********************** internaSemaphoreHandle_t sem_hndlr_r,sem_hndlr_g,sem_hndlr_b;	l functions definition ************************/
static void task_(void *argument);
int ao_led_receive(PriorityQueue *pq);

static void task_(void *argument) {

	PriorityQueue *pq = (PriorityQueue*) argument;
	GPIO_PinState led_state;
	int item = 0;

	while (true) {

		item = ao_led_receive(pq);

		switch (item) {

			case AO_LED_MESSAGE_PULSE:

				led_state = GPIO_PIN_SET;
				HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, led_state);
				LOGGER_INFO("###led on RED");
				vTaskDelay(TASK_PERIOD_MS_ / portTICK_PERIOD_MS);

				led_state = GPIO_PIN_RESET;
				HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, led_state);
				LOGGER_INFO("###led off RED");

				break;

			case AO_LED_MESSAGE_SHORT:

				led_state = GPIO_PIN_SET;
				HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, led_state);
				LOGGER_INFO("###led on GREEN");
				vTaskDelay(TASK_PERIOD_MS_ / portTICK_PERIOD_MS);

				led_state = GPIO_PIN_RESET;
				HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, led_state);
				LOGGER_INFO("###led off GREEN");

				break;

			case AO_LED_MESSAGE_LONG:

				led_state = GPIO_PIN_SET;
				HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, led_state);
				LOGGER_INFO("###led on BLUE");
				vTaskDelay(TASK_PERIOD_MS_ / portTICK_PERIOD_MS);

				led_state = GPIO_PIN_RESET;
				HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, led_state);
				LOGGER_INFO("###led off BLUE");

				break;

			default:

				LOGGER_INFO("ERROR ao_led 	task_");

				break;
		}

	}

}

/********************** external functions definition ************************/

_Bool ao_led_send(ao_handle_led_t *hao, ao_led_message_t led_message) //CARGA EL MENSAJE LA COLA
{
	_Bool stts = false;

	if (true == enqueue(hao->pq, (int) led_message)) {
		stts = true;
		LOGGER_INFO("		ao_led_send: Enviado %d", led_message);
	}

	return (stts);
}

int ao_led_receive(PriorityQueue *pq) //CARGA EL MENSAJE LA COLA
{
	int item = 0;
	item = dequeue(pq);

	if (item == -1) {

		LOGGER_INFO("		ao_led_receive: Empty Queue");

	}

	return (item);
}

_Bool ao_led_init(ao_handle_led_t *ao_led)  //CREAA LA COLA
{
	ao_led->pq = (&xpq);

	if (false == PriorityQueueCreate(ao_led->pq)) {

		LOGGER_INFO("Priority queue create error!");

		while (1) {
			//error
		}

	}

	BaseType_t status;
	status = xTaskCreate(task_, "task_ao_led", 128, (void*) ao_led->pq,
	tskIDLE_PRIORITY + 2, NULL);
	if (pdPASS != status) {
		LOGGER_INFO("No es posible crear tarea");
		return false;
	}

	return true;
}

/********************** end of file ******************************************/
