# My_stuff
## Arduino_robot
```cpp
#include <Arduino.h>
#include <HardwareSerial.h>
#include <avr/interrupt.h>
#include <string.h>
#include <Wire.h>
extern HardwareSerial Serial;

#define CYCLE 5000         // Ciclo de 20ms
//#define AV_FULL_IZD   // 1.8ms --> Adelante a full
//#define AV_FULL_DER     // 1.2ms --> 
int cnt;
uint8_t outbuf[6]; //arraytostorearduinooutput
void handler(){
  outbuf[cnt]= Wire.read();
}
void setup()
{
    delay(1);
    Wire.beginTransmission(0xF6); //deviceaddress
    Wire.write(0xF0); //1stinitialisationregister
    Wire.write(0x55); //1stinitialisationvalue
    Wire.endTransmission();
    delay(1);
    Wire.beginTransmission(0xF6);
    Wire.write(0xFB); //2ndinitialisationregister
    Wire.write(0x00); //2ndinitialisationvalue
    Wire.endTransmission();
    delay(1);
    //sendthecryptokey(zeros),in3blocksof6,6&4.
    Wire.beginTransmission(0xF6);
    Wire.write(0xF0); //cryptokeycommandregister
    Wire.write(0xAA); //sendscryptoenablenotice
    Wire.endTransmission();
    delay(1);
    Wire.beginTransmission(0xF6);
    Wire.write(0x40); //cryptokeydataaddress
    for(cnt=0;cnt<6;cnt++){
      Wire.write(0x00); //sends1stkeyblock(zeros)
    }
    Wire.endTransmission();
    Wire.beginTransmission(0xF6);
    Wire.write(0x40); //sendsmemoryaddress
    for(cnt=6;cnt<12;cnt++){
      Wire.write(0x00); //sends2ndkeyblock(zeros)
    }
    Wire.endTransmission();
    Wire.beginTransmission(0xF6);
    Wire.write(0x40); //sendsmemoryaddress
    for(cnt=12;cnt<16;cnt++){
      Wire.write(0x00); //sends3rdkeyblock(zeros)
    }
    Wire.endTransmission();
    delay(1);
    //enddeviceinit
    
    
    
}

ISR(TIMER3_COMPA_vect){    //Rutina de atenciÃ³n a la 
    digitalWrite(16,LOW);  //interrupciÃ³n. Timer3 A
}
ISR(TIMER3_COMPB_vect){    //Rutina de atenciÃ³n a la 
    digitalWrite(16,HIGH); //interrupciÃ³n. Timer3 B
    TCNT3  = 0;
}
ISR(TIMER4_COMPA_vect){    //Rutina de atenciÃ³n a la 
    digitalWrite(7,LOW);  //interrupciÃ³n. Timer4 A
}
ISR(TIMER4_COMPB_vect){    //Rutina de atenciÃ³n a la 
    digitalWrite(7,HIGH); //interrupciÃ³n. Timer4 B
    TCNT4  = 0;
}

void loop(){

   
  int cnt=0;
  cnt=0;
  Wire.beginTransmission(0x42);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x42,6);//requestdatafromnunchuck
  while(Wire.available())
  {
    Wire.onReceive(handler);//receivebyteasaninteger
    cnt++;
  }

    pinMode(16, OUTPUT);   //motor 1
    cli();
    pinMode(7,OUTPUT);     //motor 2
    cli();                 //Inab. Interr
 

    // Timer 3. Rueda IZDA

    int AV_FULL_IZD;
    int AV_FULL_DER;

    if(outbuf != 0){
      AV_FULL_IZD = 450;
      AV_FULL_DER = 300;
    }

    TCCR3A = 0;     //Reset TCCR3A. WGM31=WGM30=0
    TCCR3B = 0;     //Reset TCCR3B. WGM32=0
    TCNT3  = 0;     //Timer/Counter Register

    OCR3A = AV_FULL_IZD;  //Output A Compare Register
    OCR3B = CYCLE;        //Output B Compare Register.

    // Clk/64 (Prescaling 3). CS31 = CS30 = 1
    TCCR3B |= (1<<CS31)|(1<<CS30);   
                                     
    //Timer3 Interr. Mask Reg. Enable compare A&B 
    //Match interrupt (OCR3A&OCR3B). OCIE3A=OCR3B=1
    TIMSK3 |= (1<<OCIE3A)|(1<<OCIE3B);        


    // Timer 4. Rueda DRCA

    TCCR4A = 0;     //Reset TCCR3A. WGM31=WGM30=0
    TCCR4B = 0;     //Reset TCCR3B. WGM32=0
    TCNT4  = 0;     //Timer/Counter Register

    OCR4A = AV_FULL_DER;  //Output A Compare Register
    OCR4B = CYCLE;        //Output B Compare Register.

    // Clk/64 (Prescaling 3). CS41 = CS40 = 1
    TCCR4B |= (1<<CS31)|(1<<CS30);   
                                     
    //Timer4 Interr. Mask Reg. Enable compare A&B 
    //Match interrupt (OCR4A&OCR4B). OCIE4A=OCR4B=1
    TIMSK4 |= (1<<OCIE4A)|(1<<OCIE4B);        

    sei();         //Hab interrupts.
  
}
```
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
