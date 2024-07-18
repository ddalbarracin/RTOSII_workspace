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
#include "ao_ui.h"
#include "ao_led_r.h"
#include "ao_led_b.h"
#include "ao_led_g.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (1000)
#define QUEUE_LENGTH_            (3)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_led_message_t))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/


/********************** external data definition *****************************/


extern QueueHandle_t hqueue;
extern ao_led_handle_r_t ao_ledr;
extern ao_led_handle_b_t ao_ledb;
extern ao_led_handle_g_t ao_ledg;
extern ao_led_handle_ui_t ao_ui;


/********************** internal functions definition ************************/


static void task_(void *argument)
{
  ao_led_handle_ui_t* hao = (ao_led_handle_ui_t*)argument;
  while (true)
  {
    ao_led_message_t msg;
    if (pdPASS == xQueueReceive(hao->hqueue, &msg, portMAX_DELAY))
    {
		    switch (msg)
		    {
		      case AO_LED_MESSAGE_OFF_R:
		    	  msg = AO_LED_MESSAGE_OFF_R;
		    	  ao_led_send_r(&ao_ledr, msg);

		        break;
		      case AO_LED_MESSAGE_OFF_G:
		     	  msg = AO_LED_MESSAGE_OFF_G;
		     	  ao_led_send_g(&ao_ledg, msg);

		        break;
		      case AO_LED_MESSAGE_OFF_B:
		      	  msg = AO_LED_MESSAGE_OFF_B;
		      	  ao_led_send_b(&ao_ledb, msg);

		      	  break;
		      case AO_LED_MESSAGE_PULSE:
		    	  msg = AO_LED_MESSAGE_PULSE;
		    	  ao_led_send_r(&ao_ledr, msg);

		        break;
		      case AO_LED_MESSAGE_SHORT:
		    	  msg = AO_LED_MESSAGE_SHORT;
		    	  ao_led_send_g(&ao_ledg, msg); //

		        break;
		      case AO_LED_MESSAGE_LONG:
		    	  msg = AO_LED_MESSAGE_LONG;
		    	  ao_led_send_b(&ao_ledb, msg); //

		        break;
		      default:
		        LOGGER_INFO("error AO_LED_MESSAGE");
		        break;
		    }

    }
  }
}


/********************** external functions definition ************************/

bool ao_ui_send(ao_led_handle_ui_t* hao, ao_led_message_t msg) //CARGA EL MENSAJE LA COLA
{
  return (pdPASS == xQueueSend(hao->hqueue, (void*)&msg, 0));
}



void ao_ui_init(ao_led_handle_ui_t* hao) //CREAA LA COLA
{



  hao->hqueue = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
  while(NULL == hao->hqueue)
  {
    // error
  }



  BaseType_t status;
  status = xTaskCreate(task_, "task_ao_led", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status)
  {
    // error
  }

}




/********************** end of file ******************************************/
