# My_stuff
## Arduino_robot
## PCB_STM32F746ZG
https://github.com/user-attachments/assets/75a99071-52c2-4491-b9d5-b86a6104b9d2

## Practica_RTOS
```c
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
```
