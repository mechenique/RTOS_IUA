/*
FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
All rights reserved

VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

This file is part of the FreeRTOS distribution.

FreeRTOS is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License (version 2) as published by the
Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

***************************************************************************
>>!   NOTE: The modification to the GPL is included to allow you to     !<<
>>!   distribute a combined work that includes FreeRTOS without being   !<<
>>!   obliged to provide the source code for proprietary components     !<<
>>!   outside of the FreeRTOS kernel.                                   !<<
***************************************************************************

FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  Full license text is available on the following
link: http://www.freertos.org/a00114.html

***************************************************************************
*                                                                       *
*    FreeRTOS provides completely free yet professionally developed,    *
*    robust, strictly quality controlled, supported, and cross          *
*    platform software that is more than just the market leader, it     *
*    is the industry's de facto standard.                               *
*                                                                       *
*    Help yourself get started quickly while simultaneously helping     *
*    to support the FreeRTOS project by purchasing a FreeRTOS           *
*    tutorial book, reference manual, or both:                          *
*    http://www.FreeRTOS.org/Documentation                              *
*                                                                       *
***************************************************************************

http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
the FAQ page "My application does not run, what could be wrong?".  Have you
defined configASSERT()?

http://www.FreeRTOS.org/support - In return for receiving this top quality
embedded software for free we request you assist our global community by
participating in the support forum.

http://www.FreeRTOS.org/training - Investing in training allows your team to
be as productive as possible as early as possible.  Now you can receive
FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
Ltd, and the world's leading authority on the world's leading RTOS.

http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
including FreeRTOS+Trace - an indispensable productivity tool, a DOS
compatible FAT file system, and our tiny thread aware UDP/IP stack.

http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
licenses offer ticketed support, indemnification and commercial middleware.

http://www.SafeRTOS.com - High Integrity Systems also provide a safety
engineered and independently SIL3 certified version for use in safety and
mission critical applications that require provable dependability.

1 tab == 4 spaces!
*/

/*
* main() creates all the demo application tasks, then starts the scheduler.
* The web documentation provides more details of the standard demo application
* tasks, which provide no particular functionality but do provide a good
* example of how to use the FreeRTOS API.
*
* In addition to the standard demo tasks, the following tasks and tests are
* defined and/or created within this file:
*
* "Check" task - This only executes every five seconds but has a high priority
* to ensure it gets processor time.  Its main function is to check that all the
* standard demo tasks are still operational.  While no errors have been
* discovered the check task will print out "OK" and the current simulated tick
* time.  If an error is discovered in the execution of a task then the check
* task will print out an appropriate error message.
*
*/


/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* Priorities at which the tasks are created. */
#define mainCHECK_TASK_PRIORITY      ( configMAX_PRIORITIES - 2 )
#define mainQUEUE_POLL_PRIORITY      ( tskIDLE_PRIORITY + 1 )
#define mainSEM_TEST_PRIORITY      ( tskIDLE_PRIORITY + 1 )
#define mainBLOCK_Q_PRIORITY      ( tskIDLE_PRIORITY + 2 )
#define mainCREATOR_TASK_PRIORITY    ( tskIDLE_PRIORITY + 3 )
#define mainINTEGER_TASK_PRIORITY    ( tskIDLE_PRIORITY )
#define mainGEN_QUEUE_TASK_PRIORITY    ( tskIDLE_PRIORITY )
#define mainFLOP_TASK_PRIORITY      ( tskIDLE_PRIORITY )
#define mainQUEUE_OVERWRITE_PRIORITY  ( tskIDLE_PRIORITY )

#define mainTIMER_TEST_PERIOD      ( 50 )

/* Custom Tasks for assignments */
void vTask5( void *pvParameters );
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
void vTask3( void *pvParameters );
void vTask4( void *pvParameters );

QueueHandle_t xQueue;

struct datos_task
{
      /* TaskFunction_t pvTask Es un function pointer, no un array de char!
      TaskFunction_t es un typedef que equivale a definir void (*pvTask)(void *)
      */
  TaskFunction_t pvTask;
  char sDescr[10];
  int iStack;
  char cMessage[50];
  int iPriority;  
  TaskHandle_t HTarea;
  int iDelay;
  int iCounter;
};
struct datos_task config_tarea[6];
/*-----------------------------------------------------------*/

int main ( void )
{
  
  //struct datos_task *p = &config_tarea;

  /*
  Hay que inicializar el puntero de funcion de la siguiente manera donde vTask5
  es el nombre de la funcion definida en el prototipo de la funcion
  
  void vTask5( void *pvParameters );
  */
  config_tarea[1].pvTask = &vTask1;
  config_tarea[1].iDelay=250;
  sprintf(config_tarea[1].sDescr,"Task1");
  config_tarea[1].iPriority=1;
  sprintf(config_tarea[1].cMessage,"Init - T1"); 

  config_tarea[2].pvTask = &vTask2;
  config_tarea[2].iDelay=250;
  sprintf(config_tarea[2].sDescr,"Task2");
  config_tarea[2].iPriority=1;
  sprintf(config_tarea[2].cMessage,"Init - T2"); 

  config_tarea[3].pvTask = &vTask3;
  config_tarea[3].iDelay=250;
  sprintf(config_tarea[3].sDescr,"Task3");
  config_tarea[3].iPriority=1;
  sprintf(config_tarea[3].cMessage,"Init - T3"); 
 
 
  config_tarea[4].pvTask = &vTask4;
  config_tarea[4].iDelay=250;
  sprintf(config_tarea[4].sDescr,"Task4");
  config_tarea[4].iPriority=2;
  sprintf(config_tarea[4].cMessage,"Init - T4"); 

  config_tarea[5].pvTask = &vTask5;
  config_tarea[5].iDelay=250;
  sprintf(config_tarea[5].sDescr,"Task5");
  config_tarea[5].iPriority=1; 
  
  xTaskCreate(config_tarea[5].pvTask ,config_tarea[5].sDescr, config_tarea[5].iStack, NULL, config_tarea[5].iPriority, &config_tarea[5].HTarea );
  /* Start the scheduler itself. */
  vTaskStartScheduler();

  /* Should never get here unless there was not enough heap space to create
  the idle and other system tasks. */
  return 0;
}
/*-----------------------------------------------------------*/

void vTask5( void *pvParameters )
{
  
  
  eTaskState var;
  char strRTOSstate [250];
  char strTaskState[][25]={
  "Running" ,	
	"Ready",			
	"Blocked",		
	"Suspended",		
	"Deleted",		
	"Invalid"
  };
  TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 1500 );

  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();
  xQueue = xQueueCreate (50,50);
  if (xQueue !=NULL)
  {
  xTaskCreate(config_tarea[1].pvTask ,config_tarea[1].sDescr, config_tarea[1].iStack, config_tarea[1].cMessage, config_tarea[1].iPriority, &config_tarea[1].HTarea ); 
  xTaskCreate(config_tarea[2].pvTask ,config_tarea[2].sDescr, config_tarea[2].iStack, config_tarea[2].cMessage, config_tarea[2].iPriority, &config_tarea[2].HTarea ); 
  xTaskCreate(config_tarea[3].pvTask ,config_tarea[3].sDescr, config_tarea[3].iStack, config_tarea[3].cMessage, config_tarea[3].iPriority, &config_tarea[3].HTarea ); 
  xTaskCreate(config_tarea[4].pvTask ,config_tarea[4].sDescr, config_tarea[4].iStack, config_tarea[4].cMessage, config_tarea[4].iPriority, &config_tarea[4].HTarea ); 
  
  }
  for( ;; )
  {
    /* Place this task in the blocked state until it is time to run again. */
  

  var =  eTaskGetState(config_tarea[1].HTarea);
    printf("La tarea 1 esta en estado %s\n",strTaskState[var]);
  var =  eTaskGetState(config_tarea[2].HTarea);
    printf("La tarea 2 esta en estado %s\n",strTaskState[var]);
  var =  eTaskGetState(config_tarea[3].HTarea);
    printf("La tarea 3 esta en estado %s\n",strTaskState[var]);
  var =  eTaskGetState(config_tarea[4].HTarea);
    printf("La tarea 4 esta en estado %s\n",strTaskState[var]);
  var =  eTaskGetState(config_tarea[5].HTarea);
    printf("La tarea 5 esta en estado %s\n",strTaskState[var]);
    vTaskList(strRTOSstate);
    printf("%s\n",strRTOSstate);

    vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );
    fflush( stdout );
    

  }
}

void vTask1( void *pvParameters )
{
  
  char *lValuetosend;
  BaseType_t xStatus;
  lValuetosend = (char*) pvParameters;
  TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100 );

  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();
  //sprintf(lValuetosend,"hola");
  for( ;; )
  {
    /* Place this task in the blocked state until it is time to run again. */
    vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );
    xStatus=xQueueSendToBack (xQueue,(void *)lValuetosend,0);
    if (xStatus!=pdPASS) printf("Could not send to queue. \r\n");
    //printf("This is task 1\r\n");
    sprintf(config_tarea[1].cMessage,"T1 - Counter: %i\r\n",config_tarea[1].iCounter);
    config_tarea[1].iCounter++;
    fflush( stdout );
  }
}
void vTask2( void *pvParameters )
{
char *lValuetosend;
  BaseType_t xStatus;
  lValuetosend = (char*) pvParameters;
  TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100 );

  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();
  //sprintf(lValuetosend,"hola");
  for( ;; )
  {
    /* Place this task in the blocked state until it is time to run again. */
    vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );
    xStatus=xQueueSendToBack (xQueue,(void *)lValuetosend,0);
    if (xStatus!=pdPASS) printf("Could not send to queue. \r\n");
    //printf("This is task 2\r\n");
    sprintf(config_tarea[2].cMessage,"T2 - Counter: %i\r\n",config_tarea[2].iCounter);
    config_tarea[2].iCounter++;
    fflush( stdout );
  }
}
void vTask3( void *pvParameters )
{
 char *lValuetosend;
  BaseType_t xStatus;
  lValuetosend = (char*) pvParameters;
  TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100 );

  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();
  //sprintf(lValuetosend,"hola");
  for( ;; )
  {
    /* Place this task in the blocked state until it is time to run again. */
    vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );
    xStatus=xQueueSendToBack (xQueue,(void *)lValuetosend,0);
    if (xStatus!=pdPASS) printf("Could not send to queue. \r\n");
    //printf("This is task 3\r\n");
    sprintf(config_tarea[3].cMessage,"T3 - Counter: %i\r\n",config_tarea[3].iCounter);
    config_tarea[3].iCounter++;
    fflush( stdout );
  }
}

void vTask4( void *pvParameters )
{
  char lReceivedValue[20];
  //char RxMessage[20];
  BaseType_t xStatus;

  //TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );

  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  //xNextWakeTime = xTaskGetTickCount();

  for( ;; )
  {
    if (uxQueueMessagesWaiting (xQueue)!=0)
   {
    printf("Queue should have been empty!\r\n"); 
   }
   xStatus = xQueueReceive (xQueue, (void *)lReceivedValue, xCycleFrequency);
   if (xStatus==pdPASS)
   {
     printf ("Received: %s",lReceivedValue); }
    /* Place this task in the blocked state until it is time to run again. */
    //vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );
    //printf("This is task 4\n");
    fflush( stdout );
  }
}
