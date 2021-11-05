/*
 * FreeRTOS Kernel V10.4.4
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>
#include "GPIO.h"
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1
#define configUSE_TIME_SLICING      0
#define configQUEUE_REGISTRY_SIZE 	0
#define configUSE_EDF_SCHEDULER 	1


#define configUSE_APPLICATION_TASK_TAG  	  1
#define configUSE_APPLICATION_TASK_ITEM_VALUE 1

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 				0
#define configMAX_CO_ROUTINE_PRIORITIES 	( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           1
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE() (T1TC)

/* Trace Hook Macros */

extern volatile float CPULoad;
extern volatile unsigned int TotalSystemTime;
extern volatile unsigned int Task1TimeIn,Task1TimeOut,Task1TimeTotal;
extern volatile unsigned int Task2TimeIn,Task2TimeOut,Task2TimeTotal;

/* 
 * Macro called after a task has been selected to run. 
 * pxCurrentTCB contains the task about to enter the Running state.
 */
#define traceTASK_SWITCHED_IN() do \
								{ \
									if((int)pxCurrentTCB->pxTaskTag ==0)\
									{ \
											GPIO_write(PORT_0,PIN2,PIN_IS_HIGH); \
									} \
									else if((int)pxCurrentTCB->pxTaskTag ==1) \
									{ \
											GPIO_write(PORT_0,PIN3,PIN_IS_HIGH); \
											Task1TimeIn = T1TC; \
									} \
									else if((int)pxCurrentTCB->pxTaskTag ==2) \
									{ \
											GPIO_write(PORT_0,PIN4,PIN_IS_HIGH); \
											Task2TimeIn = T1TC; \
									} \
								}while(0)
																
		
/* 
 * Macro called before a new task is selected to run. 
 * pxCurrentTCB contains the task about to leave the Running state.
 */																
#define traceTASK_SWITCHED_OUT() do \
								 { \
										if((int)pxCurrentTCB->pxTaskTag ==0) \
										{ \
												GPIO_write(PORT_0,PIN2,PIN_IS_LOW); \
										} \
										else if((int)pxCurrentTCB->pxTaskTag ==1) \
										{ \
												GPIO_write(PORT_0,PIN3,PIN_IS_LOW); \
												Task1TimeOut = T1TC; \
												Task1TimeTotal += (Task1TimeOut - Task1TimeIn); \
										} \
										else if((int)pxCurrentTCB->pxTaskTag ==2) \
										{ \
												GPIO_write(PORT_0,PIN4,PIN_IS_LOW); \
												Task2TimeOut = T1TC; \
												Task2TimeTotal += (Task2TimeOut - Task2TimeIn); \
										} \
										TotalSystemTime = T1TC; \
										CPULoad = (Task1TimeTotal + Task2TimeTotal)/(float)TotalSystemTime; \
								 }while(0)
															 
#endif /* FREERTOS_CONFIG_H */