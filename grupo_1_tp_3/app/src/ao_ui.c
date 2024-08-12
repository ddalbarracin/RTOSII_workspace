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
#include "ao_ui.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (1000)
#define QUEUE_LENGTH_            (10)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_led_message_t))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/


/********************** external data definition *****************************/


//extern QueueHandle_t hqueue;
ao_handle_ui_t ao_ui;
extern ao_handle_led_t ao_led;

QueueHandle_t hqueue_r;



/********************** internal functions definition ************************/


static void task_(void *argument)
{
	ao_handle_ui_t* hao = (ao_handle_ui_t*)argument;


	while (true)
	{
		ao_led_message_t msg;
		if (pdPASS == xQueueReceive(hao->hqueue, &msg, portMAX_DELAY))
		{

			LOGGER_INFO("ao_ui xQueueReceive from button %d", msg);

			if( (AO_LED_MESSAGE_PULSE == msg) || (AO_LED_MESSAGE_SHORT == msg) || (AO_LED_MESSAGE_LONG  == msg)){

				ao_led_send(&ao_led, msg);

			} else { LOGGER_INFO("ERROR TIPO DE MSG"); }
		}
	}
}


/********************** external functions definition ************************/

_Bool ao_ui_send(ao_handle_ui_t* hao, ao_led_message_t msg) //CARGA EL MENSAJE LA COLA
{
	_Bool stts = false;

	if(pdPASS == xQueueSend(hao->hqueue, (ao_led_message_t* )&msg, 0)){
		stts = true;
		LOGGER_INFO("		ao_ui_send: Enviado %d", msg);
	}

	return (stts);

}



void ao_ui_init(ao_handle_ui_t* hao) //CREAA LA COLA
{



	hao->hqueue = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
	while(NULL == hao->hqueue)
	{
		// error
	}



	vQueueAddToRegistry(hao->hqueue, "Queue-UI");

	BaseType_t status;
	status = xTaskCreate(task_, "task_ao_ui", 128, (void* const)hao, tskIDLE_PRIORITY+2, NULL);
	while (pdPASS != status)
	{
		// error
	}

}




/********************** end of file ******************************************/
