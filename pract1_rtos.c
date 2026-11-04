/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/

#include "pract1_rtos.h"

INT8U error;
volatile int * red_LED_ptr = (int *) RED_LEDS_BASE;
volatile int * green_LED_ptr = (int *) GREEN_LEDS_BASE;
volatile int * HEX3_HEX0_ptr = (int *) HEX3_HEX0_BASE;
volatile int * HEX7_HEX4_ptr = (int *) HEX7_HEX4_BASE;
volatile int * KEY_ptr = (int *) PUSHBUTTONS_BASE;
volatile int * SW_switch_ptr = (int *) SWITCHES_BASE;
volatile char * LCD_display_ptr = (int *) CHAR_LCD_BASE;
volatile short * pixel_buffer = (int *) SRAM_BASE;
volatile char * character_buffer = (int *) MTL_CHAR_BUFFER_AVALON_CHAR_BUFFER_SLAVE_BASE;
volatile int * keyboard_ptr = (int *) PS2_KEY_BASE;


/* The main function creates two task and starts multi-tasking */
int main(void)
{
  App_Init();
  UCOS_Utilities();

  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1], //dir pila
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSTaskCreateExt(task3,
                    NULL,
                    (void *)&task3_stk[TASK_STACKSIZE-1],
                    TASK3_PRIORITY,
                    TASK3_PRIORITY,
                    task3_stk,
                    TASK_STACKSIZE,
                    NULL,
                    0);
  OSTaskCreateExt(task4,
                    NULL,
                    (void *)&task4_stk[TASK_STACKSIZE-1],
                    TASK4_PRIORITY,
                    TASK4_PRIORITY,
                    task4_stk,
                    TASK_STACKSIZE,
                    NULL,
                    0);

  error = OSTaskCreateExt(TaskBurt,
                      NULL,
                      (void *)&task5_stk[TASK_STACKSIZE-1],
					  TASK5_PRIORITY,
					  TASK5_PRIORITY,
                      task5_stk,
                      TASK_STACKSIZE,
                      NULL,
                      0);

  alt_ucosii_check_return_code(error);

  error = OSTaskCreateExt(TaskHomer,
                      NULL,
                      (void *)&task6_stk[TASK_STACKSIZE-1],
					  TASK6_PRIORITY,
					  TASK6_PRIORITY,
                      task6_stk,
                      TASK_STACKSIZE,
                      NULL,
                      0);
  alt_ucosii_check_return_code(error);

  OSTaskCreateExt(TaskSW,
		  	  	  	  NULL,
					  (void *)&task7_stk[TASK_STACKSIZE-1],
					  TASK7_PRIORITY,
					  TASK7_PRIORITY,
                      task7_stk,
                      TASK_STACKSIZE,
                      NULL,
                      0);


  OSStart();

  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
