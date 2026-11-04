/*
 * pract1_rtos.h
 *
 *  Created on: 24 de mar. de 2025
 *      Author: dameso
 */

#ifndef PRACT1_RTOS_H_
#define PRACT1_RTOS_H_



//INT8U error;
/* Definition of peripherial addresses */

volatile int * red_LED_ptr;
volatile int * green_LED_ptr;
volatile int * HEX3_HEX0_ptr;
volatile int * HEX7_HEX4_ptr;
volatile int * KEY_ptr;
volatile int * SW_switch_ptr;
volatile char * LCD_display_ptr;
volatile short * pixel_buffer;
volatile char * character_buffer;
volatile int * keyboard_ptr;

#include <stdio.h>
#include "includes.h"
#include "AppSW\inc\alt_ucosii_simple_error_check.h"

#include "BaseSW\inc\led.h"
#include "BaseSW\inc\lcd.h"
#include "BaseSW\inc\mtl.h"
#include "AppSW\inc\init.h"

#include "AppSW\inc\tasksw.h"
#include "AppSW\inc\task1.h"
#include "AppSW\inc\task2.h"
#include "AppSW\inc\task3.h"
#include "AppSW\inc\task4.h"
#include "AppSW\inc\taskburt.h"
#include "AppSW\inc\taskhomer.h"



/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048     //palabras
OS_STK    task1_stk[TASK_STACKSIZE];	//puntero de pila
OS_STK    task2_stk[TASK_STACKSIZE];	//puntero de pila
OS_STK    task3_stk[TASK_STACKSIZE];	//puntero de pila
OS_STK    task4_stk[TASK_STACKSIZE];	//puntero de pila
OS_STK    task5_stk[TASK_STACKSIZE];	//puntero de pila
OS_STK    task6_stk[TASK_STACKSIZE];	//puntero de pila
OS_STK    task7_stk[TASK_STACKSIZE];	//puntero de pila

/* Definition of Task Priorities */
#define INHERITANCE_PRIO    0
#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3
#define TASK4_PRIORITY      4
#define TASK5_PRIORITY      5
#define TASK6_PRIORITY      6
#define TASK7_PRIORITY      7
#endif

