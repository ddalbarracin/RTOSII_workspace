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
//#include "ao_ui.h"
#include "ao_led_r.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (1000)
#define QUEUE_LENGTH_            (1)
#define QUEUE_ITEM_SIZE_         (sizeof(ao_led_message_t))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/



/********************** external data definition *****************************/


//extern QueueHandle_t hqueue2;
//extern QueueHandle_t hqueue2;
extern ao_led_handle_b_t ao_ledb;

/********************** internal functions definition ************************/




static void task_(void *argument)
{
  ao_led_handle_b_t* hao = (ao_led_handle_b_t*)argument;
  while (true)
  {
    ao_led_message_t msg;
    if (pdPASS == xQueueReceive(hao->hqueue2, &msg, portMAX_DELAY))
    {
      GPIO_PinState led_state;
      if (AO_LED_MESSAGE_LONG== msg)
      {
        led_state = GPIO_PIN_SET;
        LOGGER_INFO("led on BLUE");
      }
      else
      {
        led_state = GPIO_PIN_RESET;
        LOGGER_INFO("led off BLUE");
      }


      HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, led_state);

    }
  }
}


/********************** external functions definition ************************/

bool ao_led_send_b(ao_led_handle_b_t* hao, ao_led_message_t msg) //CARGA EL MENSAJE LA COLA
{
  return (pdPASS == xQueueSend(hao->hqueue2, (void*)&msg, 0));
}



void ao_led_B_init(ao_led_handle_b_t* hao) //CREAA LA COLA
{

	  hao->hqueue2 = xQueueCreate(QUEUE_LENGTH_, QUEUE_ITEM_SIZE_);
	   while(NULL == hao->hqueue2)
	   {
	     // error
	   }
  BaseType_t status;
  status = xTaskCreate(task_, "task_ao_led_B", 128, (void* const)hao, tskIDLE_PRIORITY, NULL);
  while (pdPASS != status)
  {
    // error
  }
}


/*void task_led(void *argument)
{
  while (true)
  {
    led_color_t color;

    if(pdTRUE == xSemaphoreTake(hsem_led, 0))
    {
      color = LED_COLOR_RED;
    }
    else
    {
      color = LED_COLOR_NONE;
    }

    switch (color)
    {
      case LED_COLOR_NONE:
        led_set_colors(false, false, false);
        break;
      case LED_COLOR_RED:
        LOGGER_INFO("led red");
        led_set_colors(true, false, false);
        break;
      case LED_COLOR_GREEN:
        LOGGER_INFO("led green");
        led_set_colors(false, true, false);
        break;
      case LED_COLOR_BLUE:
        LOGGER_INFO("led blue");
        led_set_colors(false, false, true);
        break;
      case LED_COLOR_WHITE:
        LOGGER_INFO("led white");
        led_set_colors(true, true, true);
        break;
      default:
        break;
    }

   // vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  }
}*/

/********************** end of file ******************************************/
