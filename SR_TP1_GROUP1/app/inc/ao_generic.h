/*
 * Copyright (c) 2023 Daniel Daivid Albarracin <danield.albarracin@gmail.com>.
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

#ifndef TASK_AO_H_
#define TASK_AO_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
typedef struct
{
    void * p_task_funct;
    const char *p_task_name;
    const configSTACK_DEPTH_TYPE task_Stack;
    void * const pvParameters;
    UBaseType_t task_priority;
    TaskHandle_t * const p_task_hndlr;

} ao_task_t;

typedef enum
{
	AO_LED_STATUS_NONE,
	AO_LED_STATUS_ON,
	AO_LED_STATUS_OFF,

} ao_led_stts_t;

typedef enum
{
  AO_LED_TYPE_NONE,
  AO_LED_TYPE_PULSE,
  AO_LED_TYPE_SHORT,
  AO_LED_TYPE_LONG,

} ao_led_type_t;

typedef struct{

	ao_led_type_t type;
	ao_led_stts_t stts;

}ao_msg_t;

typedef struct{

	QueueHandle_t ao_chnl_queue;
	uint32_t ao_chnl_queue_lgth;
	uint32_t ao_chnl_queue_size;
	const char *p_ao_chnl_name;

}ao_chnl_t;

typedef struct
{
	ao_task_t ao_task_cnfg;
	void (*p_ao_thread)(ao_msg_t);
	ao_chnl_t ao_chnl;

}ao_hndlr_t;



/********************** external data declaration ****************************/
_Bool ao_send_msg(ao_hndlr_t *ao_hndlr, ao_msg_t msg);
void ao_init(ao_hndlr_t* ao_hndlr);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_AO_H_ */
/********************** end of file ******************************************/

