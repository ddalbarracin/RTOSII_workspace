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

#define TASK_PERIOD_MS_           (50)

#define BUTTON_PERIOD_MS_         (TASK_PERIOD_MS_)
#define BUTTON_PULSE_TIMEOUT_     (200)
#define BUTTON_SHORT_TIMEOUT_     (1000)
#define BUTTON_LONG_TIMEOUT_      (2000)
#define LED_TIMER_S				  (1000)

/********************** internal data declaration ****************************/
ao_led_type_t button_type;
ao_led_stts_t button_stts;
ao_msg_t button_ao;
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

extern ao_hndlr_t ao_array[];

/********************** internal functions definition ************************/



static struct
{
    uint32_t counter;
    uint32_t led_red_counter;
    uint32_t led_green_counter;
    uint32_t led_blue_counter;

} button;

static struct
{
	uint32_t red_timer_flg;
	uint32_t green_timer_flg;
	uint32_t blue_timer_flg;

}timer_led;

static void button_init_(void)
{
  button.counter = 0;
  button.led_red_counter = 0;
  button.led_green_counter = 0;
  button.led_blue_counter = 0;
  timer_led.red_timer_flg = 0;
  timer_led.green_timer_flg = 0;
  timer_led.blue_timer_flg = 0;
}

static ao_msg_t led_process_state()
{
	ao_msg_t ret = { .type = AO_LED_TYPE_NONE,
			.stts = AO_LED_STATUS_NONE };

	if (timer_led.red_timer_flg) {

		button.led_red_counter += BUTTON_PERIOD_MS_;

		if (LED_TIMER_S <= button.led_red_counter){

			button.led_red_counter = 0;
			timer_led.red_timer_flg = 0;
			ret.type = AO_LED_TYPE_PULSE;
			ret.stts = AO_LED_STATUS_OFF;
		}

	}

	if (timer_led.green_timer_flg) {

		button.led_green_counter += BUTTON_PERIOD_MS_;

		if (LED_TIMER_S <= button.led_green_counter){

			button.led_green_counter = 0;
			timer_led.green_timer_flg = 0;
			ret.type = AO_LED_TYPE_SHORT;
			ret.stts = AO_LED_STATUS_OFF;
		}

	}
	if (timer_led.blue_timer_flg) {

		button.led_blue_counter += BUTTON_PERIOD_MS_;

		if (LED_TIMER_S <= button.led_blue_counter){

			button.led_blue_counter = 0;
			timer_led.blue_timer_flg = 0;
			ret.type = AO_LED_TYPE_LONG;
			ret.stts = AO_LED_STATUS_OFF;
		}

	}

	return(ret);

}


static ao_led_type_t button_process_state(_Bool value)
{
	ao_led_type_t ret = AO_LED_TYPE_NONE;

	if (value) {

		button.counter += BUTTON_PERIOD_MS_;

	} else {

		if (BUTTON_LONG_TIMEOUT_ <= button.counter) {

			ret = AO_LED_TYPE_LONG;
			timer_led.blue_timer_flg = 1;

		} else if (BUTTON_SHORT_TIMEOUT_ <= button.counter) {

			ret = AO_LED_TYPE_SHORT;
			timer_led.green_timer_flg = 1;

		} else if (BUTTON_PULSE_TIMEOUT_ <= button.counter) {

			ret = AO_LED_TYPE_PULSE;
			timer_led.red_timer_flg = 1;
		}

		button.counter = 0;
	}
	return ret;
}

/********************** external functions definition ************************/

void task_button(void* argument)
{
  button_init_();

  while(true)
  {
    GPIO_PinState button_state;
    button_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN);

    ao_led_type_t button_type;
    button_type = button_process_state(button_state);

    ao_msg_t button_stts;

	switch (button_type) {
		case AO_LED_TYPE_NONE:
			break;

		case AO_LED_TYPE_PULSE:
			LOGGER_INFO("button pulse - led red on");
			button_stts.type = button_type;
			button_stts.stts = AO_LED_STATUS_ON;
			ao_send_msg(&ao_array[3], button_stts);
			break;

		case AO_LED_TYPE_SHORT:
			LOGGER_INFO("button short - led green on");
			button_stts.type = button_type;
			button_stts.stts = AO_LED_STATUS_ON;
			ao_send_msg(&ao_array[3], button_stts);
			break;

		case AO_LED_TYPE_LONG:
			LOGGER_INFO("button long - led blue on");
			button_stts.type = button_type;
			button_stts.stts = AO_LED_STATUS_ON;
			ao_send_msg(&ao_array[3], button_stts);
			break;

		default:
			LOGGER_INFO("button error");
			break;

	}

	ao_msg_t led_stts;

	led_stts = led_process_state();

	switch (led_stts.type) {
		case AO_LED_TYPE_NONE:
			break;

		case AO_LED_TYPE_PULSE:

			LOGGER_INFO("button pulse - led red off");
			ao_send_msg(&ao_array[3], led_stts);
			break;

		case AO_LED_TYPE_SHORT:

			LOGGER_INFO("button short - led green off");
			ao_send_msg(&ao_array[3], led_stts);
			break;

		case AO_LED_TYPE_LONG:

			LOGGER_INFO("button long - led blue off");
			ao_send_msg(&ao_array[3], led_stts);
			break;

		default:
			LOGGER_INFO("button error");
			break;

	}

	vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  }
}

/********************** end of file ******************************************/
