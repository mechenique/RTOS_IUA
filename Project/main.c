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


/*
* Prototypes for the standard FreeRTOS callback/hook functions implemented
* within this file.
*/
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );

/* Task function to check demo status. */
static void prvCheckTask( void *pvParameters );

/* The variable into which error messages are latched. */
static char *pcStatusMessage = "OK";

/* Custom Tasks for assignments */
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );
#define task1StackSize 1000
#define task2StackSize 100
#define task1Priority 1
#define task2Priority 1
/*-----------------------------------------------------------*/

int main ( void )
{
  /* Start the check task as described at the top of this file. */
  xTaskCreate( prvCheckTask, "Check", configMINIMAL_STACK_SIZE, NULL, \
                                              mainCHECK_TASK_PRIORITY, NULL );

  /* Create Task 1 and 2. */
  xTaskCreate( vTask1, "Task1", task1StackSize, NULL, task1Priority, NULL );
  xTaskCreate( vTask2, "Task2", task2StackSize, NULL, task2Priority, NULL );

  /* Create the standard demo tasks. */
  vStartIntegerMathTasks( mainINTEGER_TASK_PRIORITY );
  vStartMathTasks( mainFLOP_TASK_PRIORITY );
  vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
  vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
  vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
  vStartDynamicPriorityTasks();
  vCreateBlockTimeTasks();
  vStartGenericQueueTasks( mainGEN_QUEUE_TASK_PRIORITY );
  vStartQueuePeekTasks();
  vStartCountingSemaphoreTasks();
  vStartRecursiveMutexTasks();

  vCreateAbortDelayTasks();
  vStartEventGroupTasks();
  vStartInterruptSemaphoreTasks();
  vStartQueueSetTasks();
  vStartQueueSetPollingTask();
  vStartQueueOverwriteTask( mainQUEUE_OVERWRITE_PRIORITY );
  vStartTaskNotifyTask();

  #if( configUSE_PREEMPTION != 0  )
  {
    /* Don't expect these tasks to pass when preemption is not used. */
    vStartTimerDemoTask( mainTIMER_TEST_PERIOD );
  }
  #endif

  /* The suicide tasks must be created last as they need to know how many
  tasks were running prior to their creation.  This then allows them to
  ascertain whether or not the correct/expected number of tasks are running at
  any given time. */
  vCreateSuicidalTasks( mainCREATOR_TASK_PRIORITY );

  /* Start the scheduler itself. */
  vTaskStartScheduler();

  /* Should never get here unless there was not enough heap space to create
  the idle and other system tasks. */
  return 0;
}
/*-----------------------------------------------------------*/

void vTask1( void *pvParameters )
{
  TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );

  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();

  for( ;; )
  {
    /* Place this task in the blocked state until it is time to run again. */
    vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );

    printf("This is task 1\n");
    fflush( stdout );
  }
}

void vTask2( void *pvParameters )
{
  TickType_t xNextWakeTime;
  const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );

  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  /* Initialise xNextWakeTime - this only needs to be done once. */
  xNextWakeTime = xTaskGetTickCount();

  for( ;; )
  {
    /* Place this task in the blocked state until it is time to run again. */
    vTaskDelayUntil( &xNextWakeTime, xCycleFrequency );

    printf("This is task 2\n");
    fflush( stdout );
  }
}
