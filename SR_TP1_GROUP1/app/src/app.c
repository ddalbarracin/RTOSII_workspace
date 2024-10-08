/*
 * Copyright (c) 2023 Daniel David Albarracin <danield.albarracin@gmail.com>.
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
 * @author : Daniel David Albarracin <danield.albarracin@gmail.com>
 */

/********************** inclusions *******************************************/

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "dwt.h"
#include "board.h"

#include "task_button.h"
#include "ao_generic.h"

/********************** macros and definitions *******************************/


/********************** internal data declaration ****************************/
const char *pinit = " App Init";
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration *****************************/

SemaphoreHandle_t signal_task_button_sem_bin_hndlr;
SemaphoreHandle_t signal_task_led_sem_bin_hndlr;

/********************** external functions definition ************************/
void app_init(void)
{
	extern ao_hndlr_t ao_array[];

	BaseType_t ret;

	ao_init(&ao_array[0]);
	ao_init(&ao_array[1]);
	ao_init(&ao_array[2]);
	ao_init(&ao_array[3]);

	ret = xTaskCreate(task_button,
					  "task_button",
					  configMINIMAL_STACK_SIZE,
					  NULL,
					  tskIDLE_PRIORITY,
					  NULL);

	configASSERT(ret == pdPASS);




	LOGGER_INFO(pinit);

	cycle_counter_init();
}

/********************** end of file ******************************************/
