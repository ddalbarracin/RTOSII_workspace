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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "ao_generic.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_          (1000)
#define QUEUE_LENGTH_			 (1)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_msg_t))


#define p_ao_led_red_name  				"task_led_red"
#define p_ao_led_green_name 			"task_led_green"
#define p_ao_led_blue_name 				"task_led_blue"
#define p_ao_ui_name 					"task_led_ui"

#define p_ao_led_red_channel_name		"TASK QUEUE RED"
#define p_ao_led_blue_channel_name		"TASK QUEUE BLUE"
#define p_ao_led_green_channel_name		"TASK QUEUE GREEN"
#define p_ao_ui_channel_name 			"TASK QUEUE UI"

/********************** internal data declaration ****************************/


/********************** internal functions declaration ***********************/
static void ao_led_set(_Bool led_red, _Bool led_green, _Bool led_blue);
static void ao_task(void *argument);
static void ao_thread(ao_msg_t msg);

/********************** internal data definition *****************************/

typedef enum
{

	LED_COLOR_NONE,
	LED_COLOR_RED,
	LED_COLOR_GREEN,
	LED_COLOR_BLUE,
	LED_COLOR__N,

} led_color_t;

/********************** external data definition *****************************/

extern SemaphoreHandle_t signal_task_led_sem_bin_hndlr;

/********************** internal functions definition ************************/

ao_hndlr_t ao_array[] = {
		{
				.ao_task_cnfg.p_task_funct = ao_task,
				.ao_task_cnfg.p_task_name = p_ao_led_red_name,
				.ao_task_cnfg.task_Stack = configMINIMAL_STACK_SIZE,
				.ao_task_cnfg.pvParameters = NULL,
				.ao_task_cnfg.task_priority = tskIDLE_PRIORITY,
				.ao_task_cnfg.p_task_hndlr = NULL,
				.p_ao_thread = ao_thread,
				.ao_chnl.ao_chnl_queue = NULL,
				.ao_chnl.ao_chnl_queue_lgth = QUEUE_LENGTH_,
				.ao_chnl.ao_chnl_queue_size = QUEUE_ITEM_SIZE_,
				.ao_chnl.p_ao_chnl_name = p_ao_led_red_channel_name
		},

		{
				.ao_task_cnfg.p_task_funct = ao_task,
				.ao_task_cnfg.p_task_name = p_ao_led_green_name,
				.ao_task_cnfg.task_Stack = configMINIMAL_STACK_SIZE,
				.ao_task_cnfg.pvParameters = NULL,
				.ao_task_cnfg.task_priority = tskIDLE_PRIORITY,
				.ao_task_cnfg.p_task_hndlr = NULL,
				.p_ao_thread = ao_thread,
				.ao_chnl.ao_chnl_queue = NULL,
				.ao_chnl.ao_chnl_queue_lgth = QUEUE_LENGTH_,
				.ao_chnl.ao_chnl_queue_size = QUEUE_ITEM_SIZE_,
				.ao_chnl.p_ao_chnl_name = p_ao_led_green_channel_name

		},
		{
				.ao_task_cnfg.p_task_funct = ao_task,
				.ao_task_cnfg.p_task_name = p_ao_led_blue_name,
				.ao_task_cnfg.task_Stack = configMINIMAL_STACK_SIZE,
				.ao_task_cnfg.pvParameters = NULL,
				.ao_task_cnfg.task_priority = tskIDLE_PRIORITY,
				.ao_task_cnfg.p_task_hndlr = NULL,
				.p_ao_thread = ao_thread,
				.ao_chnl.ao_chnl_queue = NULL,
				.ao_chnl.ao_chnl_queue_lgth = QUEUE_LENGTH_,
				.ao_chnl.ao_chnl_queue_size = QUEUE_ITEM_SIZE_,
				.ao_chnl.p_ao_chnl_name = p_ao_led_blue_channel_name

		},
		{
				.ao_task_cnfg.p_task_funct = ao_task,
				.ao_task_cnfg.p_task_name = p_ao_ui_name,
				.ao_task_cnfg.task_Stack = configMINIMAL_STACK_SIZE,
				.ao_task_cnfg.pvParameters = NULL,
				.ao_task_cnfg.task_priority = tskIDLE_PRIORITY,
				.ao_task_cnfg.p_task_hndlr = NULL,
				.p_ao_thread = ao_thread,
				.ao_chnl.ao_chnl_queue = NULL,
				.ao_chnl.ao_chnl_queue_lgth = QUEUE_LENGTH_,
				.ao_chnl.ao_chnl_queue_size = QUEUE_ITEM_SIZE_,
				.ao_chnl.p_ao_chnl_name = p_ao_ui_channel_name

		}
};


/*
 * @func:
 * 		ao_led_set
 *
 * @brief:
 * 		Set colors of Leds
 *
 * @params:
 * 		led_red: bool value to set or not set
 * 		led_blue: bool value to set or not set
 * 		led_green: bool value to set or not set
 *
 * @return:
 * 		None
 */
static void ao_led_set(_Bool led_red, _Bool led_green, _Bool led_blue)
{

  HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, led_red ? GPIO_PIN_SET: GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, led_green ? GPIO_PIN_SET: GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, led_blue ? GPIO_PIN_SET: GPIO_PIN_RESET);

  return;

}

/*
 * @func:
 * 		task_ao
 *
 * @brief:
 * 		Execution Thread of AO
 *
 * @params:
 * 		argument: casting to ao_hndlr_t
 *
 * @return:
 * 		None
 */
static void ao_task(void *argument)
{

	ao_hndlr_t *ao_hndlr = (ao_hndlr_t *)argument;

	while (true) {
		ao_msg_t msg;

		if (pdPASS == xQueueReceive(ao_hndlr->ao_chnl.ao_chnl_queue, &msg, portMAX_DELAY)){

			ao_hndlr->p_ao_thread(msg);
		}

	}
}


/*
 * @func:
 * 		ao_thread
 *
 * @brief:
 * 		Execution Thread of AO
 *
 * @params:
 * 		ao_msg_t: 	message whose ao receive
 *
 * @return:
 * 		None
 */
static void ao_thread(ao_msg_t msg){

	if(msg.type){

		switch (msg.type) {

			case AO_LED_TYPE_PULSE:
				LOGGER_INFO("led red type pulse");
				ao_send_msg(&ao_array[0], msg);
				break;

			case AO_LED_TYPE_SHORT:
				LOGGER_INFO("led green type short");
				ao_send_msg(&ao_array[1], msg);
				break;

			case AO_LED_TYPE_LONG:
				LOGGER_INFO("led blue type long");
				ao_send_msg(&ao_array[2], msg);
				break;
			default:
				break;

		}
	}else{

		switch (msg.stts) {

			case AO_LED_STATUS_ON:
				LOGGER_INFO("led on");
				ao_led_set(AO_LED_STATUS_ON,AO_LED_STATUS_ON,AO_LED_STATUS_ON);
				break;

			case AO_LED_STATUS_OFF:
				LOGGER_INFO("led off");
				ao_led_set(AO_LED_STATUS_OFF,AO_LED_STATUS_OFF,AO_LED_STATUS_OFF);
				break;

			default:
				break;

		}
	}
}

/********************** external functions definition ************************/
/*
 * @func:
 * 		ao_send_msg
 *
 * @brief:
 * 		make something
 *
 * @params:
 * 		ao_hndlr_t: handler to Active Object
 * 		ao_msg_t: 	message whose ao receive
 *
 * @return:
 * 		None
 */
_Bool ao_send_msg(ao_hndlr_t* ao_hndlr, ao_msg_t msg){

	return (pdPASS == xQueueSend(ao_hndlr->ao_chnl.ao_chnl_queue, (void*)&msg, 0));
}


/*
 * @func:
 * 		ao_init
 *
 * @brief:
 * 		make something
 *
 * @params:
 * 		ao_hndlr_t: handler to Active Object
 *
 * @return:
 * 		None
 */
void ao_init(ao_hndlr_t *ao_hndlr){

	BaseType_t ret;

	ao_hndlr->ao_chnl.ao_chnl_queue = xQueueCreate(ao_hndlr->ao_chnl.ao_chnl_queue_lgth, ao_hndlr->ao_chnl.ao_chnl_queue_size);

	configASSERT(NULL != ao_hndlr->ao_chnl.ao_chnl_queue);

	vQueueAddToRegistry(ao_hndlr->ao_chnl.ao_chnl_queue, ao_hndlr->ao_chnl.p_ao_chnl_name);

	ret = xTaskCreate(ao_hndlr->ao_task_cnfg.p_task_funct,
					  ao_hndlr->ao_task_cnfg.p_task_name,
					  ao_hndlr->ao_task_cnfg.task_Stack,
					  (void *) ao_hndlr,
					  ao_hndlr->ao_task_cnfg.task_priority,
					  ao_hndlr->ao_task_cnfg.p_task_hndlr);

	configASSERT(ret == pdPASS);

	return;

}



/********************** end of file ******************************************/
