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
/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (50)

#define BUTTON_PERIOD_MS_         (TASK_PERIOD_MS_)
#define BUTTON_PULSE_TIMEOUT_     (200)
#define BUTTON_SHORT_TIMEOUT_     (1000)
#define BUTTON_LONG_TIMEOUT_      (2000)
#define delay_1s                  (1000)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/


extern ao_led_handle_ui_t ao_ui; //STRUCT QUE CONTIENE LA COLA

/********************** internal functions definition ************************/
typedef uint32_t tick_t;
typedef bool bool_t;

tick_t delay_r=0;
bool_t flag_r=false;
tick_t delay_g=0;
bool_t flag_g=false;
tick_t delay_b=0;
bool_t flag_b=false;
typedef enum
{
  BUTTON_TYPE_NONE,
  BUTTON_TYPE_PULSE,
  BUTTON_TYPE_SHORT,
  BUTTON_TYPE_LONG,
  BUTTON_TYPE__N,
} button_type_t;

static struct
{
    uint32_t counter;
} button;

static void button_init_(void)
{
  button.counter = 0;
}

static button_type_t button_process_state_(bool value)
{
  button_type_t ret = BUTTON_TYPE_NONE;
  if(value)
  {
    button.counter += BUTTON_PERIOD_MS_;
  }
  else
  {
    if(BUTTON_LONG_TIMEOUT_ <= button.counter)
    {
      ret = BUTTON_TYPE_LONG;
    }
    else if(BUTTON_SHORT_TIMEOUT_ <= button.counter)
    {
      ret = BUTTON_TYPE_SHORT;
    }
    else if(BUTTON_PULSE_TIMEOUT_ <= button.counter)
    {
      ret = BUTTON_TYPE_PULSE;
    }
    button.counter = 0;
  }
  return ret;
}

/********************** external functions definition ************************/

void task_button(void* argument)
{
  button_init_();

  TickType_t time = xTaskGetTickCount();

  while(true)
  {
    GPIO_PinState button_state;
    button_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);

    button_type_t button_type;
    button_type = button_process_state_(button_state);

    ao_led_message_t msg; //ENUM

    switch (button_type)
    {
      case BUTTON_TYPE_NONE:
        break;
      case BUTTON_TYPE_PULSE:
        LOGGER_INFO("button pulse");

        msg = AO_LED_MESSAGE_PULSE;
        ao_ui_send(&ao_ui, msg); //
        flag_r=true;
        delay_r=0; //se asigna el valor cada 1mms
        break;
      case BUTTON_TYPE_SHORT:
        LOGGER_INFO("button short");
        msg = AO_LED_MESSAGE_SHORT;
        ao_ui_send(&ao_ui, msg); //
        flag_g=true;
        delay_g=0; //se asigna el valor cada 1mms
        break;
      case BUTTON_TYPE_LONG:
        LOGGER_INFO("button long");
        msg = AO_LED_MESSAGE_LONG;
        ao_ui_send(&ao_ui, msg); //
        flag_b=true;
        delay_b=0; //se asigna el valor cada 1mms
        break;
      default:
        LOGGER_INFO("button error");
        break;
    }

		if (flag_r)
		{
			if (delay_r == delay_1s)
			{

				msg = AO_LED_MESSAGE_OFF_R;
				ao_ui_send(&ao_ui, msg); //
				flag_r = false;
			}
		}

		if (flag_g)
		{
			if (delay_g == delay_1s)
			{

				msg = AO_LED_MESSAGE_OFF_G;
				ao_ui_send(&ao_ui, msg); //
				flag_g = false;
			}
		}

		if (flag_b)
		{
			if (delay_b == delay_1s)
			{

				msg = AO_LED_MESSAGE_OFF_B;
				ao_ui_send(&ao_ui, msg); //
				flag_b = false;
			}
		}

   // vTaskDelayUntil((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));

    vTaskDelayUntil(&time, (TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
    delay_r+=TASK_PERIOD_MS_;
    delay_b+=TASK_PERIOD_MS_;
    delay_g+=TASK_PERIOD_MS_;
  }
}

/********************** end of file ******************************************/
