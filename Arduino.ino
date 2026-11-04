
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

