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

#ifndef AO_UI_H_
#define AO_UI_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef enum
{
  AO_LED_MESSAGE_PULSE,
  AO_LED_MESSAGE_SHORT,
  AO_LED_MESSAGE_LONG,
  AO_LED_MESSAGE_OFF_R,
  AO_LED_MESSAGE_OFF_B,
  AO_LED_MESSAGE_OFF_G,
} ao_led_message_t;

typedef struct
{
    QueueHandle_t hqueue;   // COLA PARA UI
    //QueueHandle_t hqueue1;  //COLA PARA EL ROJO
   // QueueHandle_t hqueue2; //COLA PARA EL AZUL
   // QueueHandle_t hqueue3; //COLA PARA EL VERDE
} ao_led_handle_ui_t;

typedef struct
{
    //QueueHandle_t hqueue;   // COLA PARA UI
    QueueHandle_t hqueue1;  //COLA PARA EL ROJO
   // QueueHandle_t hqueue2; //COLA PARA EL AZUL
    //QueueHandle_t hqueue3; //COLA PARA EL VERDE
} ao_led_handle_r_t;

typedef struct
{
    //QueueHandle_t hqueue;   // COLA PARA UI
    //QueueHandle_t hqueue1;  //COLA PARA EL ROJO
    //QueueHandle_t hqueue2; //COLA PARA EL AZUL
    QueueHandle_t hqueue3; //COLA PARA EL VERDE
} ao_led_handle_g_t;

typedef struct
{
    //QueueHandle_t hqueue;   // COLA PARA UI
    //QueueHandle_t hqueue1;  //COLA PARA EL ROJO
    QueueHandle_t hqueue2; //COLA PARA EL AZUL
    //QueueHandle_t hqueue3; //COLA PARA EL VERDE
} ao_led_handle_b_t;


/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

bool ao_ui_send(ao_led_handle_ui_t* hao, ao_led_message_t msg);

void ao_ui_init(ao_led_handle_ui_t* hao);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H_ */
/********************** end of file ******************************************/

