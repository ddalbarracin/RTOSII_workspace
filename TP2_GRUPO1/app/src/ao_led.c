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

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (1000)
#define QUEUE_LENGTH_            (1)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_led_message_t))
#define delay_time               (1000000)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/



/********************** external data definition *****************************/

extern int task_cnt_;
/********************** internal functions definition ************************/
static void delete_task_(void);
static void task_(void *argument);


static void task_(void *argument) {

	QueueHandle_t hqueue_task_connetion = (QueueHandle_t) argument;

	while (true)
	{
		ao_led_message_t led_message;
		GPIO_PinState led_state;

		while (pdPASS == xQueueReceive(hqueue_task_connetion, (ao_led_message_t* )&led_message, 0))
		{
			switch (led_message){

			case AO_LED_MESSAGE_PULSE:

				led_state = GPIO_PIN_SET;
				HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, led_state);
				LOGGER_INFO("led on RED");

				vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));

				led_state = GPIO_PIN_RESET;
				HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, led_state);
				LOGGER_INFO("led off RED");

				break;

			case AO_LED_MESSAGE_SHORT:

				led_state = GPIO_PIN_SET;
				HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, led_state);
				LOGGER_INFO("led on GREEN");

				vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));

				led_state = GPIO_PIN_RESET;
				HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, led_state);
				LOGGER_INFO("led off GREEN");

				break;

			case AO_LED_MESSAGE_LONG:

				led_state = GPIO_PIN_SET;
				HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, led_state);
				LOGGER_INFO("led on BLUE");

				vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));

				led_state = GPIO_PIN_RESET;
				HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, led_state);
				LOGGER_INFO("led off BLUE");

				break;

			default:

				LOGGER_INFO("ERROR ao_led 	task_");

				break;
			}

		}

		delete_task_();
	}

}


/********************** external functions definition ************************/

bool ao_led_send(QueueHandle_t hqueue_aux, ao_led_message_t led_message) //CARGA EL MENSAJE LA COLA
{
	return (pdPASS == xQueueSend(hqueue_aux, (ao_led_message_t* )&led_message, 0));
}



bool ao_led_init(QueueHandle_t hqueue_task) //CREAA LA COLA
{

	LOGGER_INFO("Crear nueva tarea");
	if(task_cnt_ < MAX_CONNECTION_)
	{
		BaseType_t status;
		status = xTaskCreate(task_, "task_connection", 128, (void *)  hqueue_task, tskIDLE_PRIORITY + 1, NULL);
		if(pdPASS != status)
		{
			LOGGER_INFO("No es posible crear mas tareas");
			return false;
		}
		LOGGER_INFO("Nueva tarea creata");
		task_cnt_++;
		LOGGER_INFO("Cantled_messagead de procesos: %d", task_cnt_);
		return true;
	}
	else
	{
		LOGGER_INFO("Maxima cantled_messagead de tareas creadas");
		return false;
	}
}

static void delete_task_(void)
{
	LOGGER_INFO("Elimino tarea");
	task_cnt_--;
	LOGGER_INFO("Cant led_messagead de procesos: %d", task_cnt_);
	vTaskDelete(NULL);
}




/********************** end of file ******************************************/
