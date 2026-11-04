
#include <Wire.h>

uint8_t outbuf[6];		// array to store arduino output
int cnt = 0;
int ledPin = 13;

void print_C();
void print_asm();
char nunchuk_decode_byte (char x);
void nunchuck_init ();


void setup ()
{
	Serial.begin (9600);
	Wire.begin ();
	nunchuck_init ();
	Serial.print ("Finished setup\n");
}

void nunchuck_init () {
	delay(1);
	Wire.beginTransmission(0x52); 	// device address
	Wire.write(0xF0); 				// 1st initialisation register
	Wire.write(0x55); 				// 1st initialisation value
	Wire.endTransmission();
	delay(1);
	Wire.beginTransmission(0x52);
	Wire.write(0xFB); 				// 2nd initialisation register
	Wire.write(0x00); 				// 2nd initialisation value
	Wire.endTransmission();
	delay(1);
	
	// send the crypto key (zeros), in 3 blocks of 6, 6 & 4.
	Wire.beginTransmission(0x52);
	Wire.write(0xF0);				// crypto key command register
	Wire.write(0xAA); 				// sends crypto enable notice
	Wire.endTransmission();
	delay(1);
	Wire.beginTransmission(0x52);
	Wire.write(0x40); 				// crypto key data address
	for (cnt = 0; cnt < 6; cnt++) {
		Wire.write(0x00); 			// sends 1st key block (zeros)
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x52);
	Wire.write(0x40); 				// sends memory address
	for (cnt = 6; cnt < 12; cnt++) {
		Wire.write(0x00); 			// sends 2nd key block (zeros)
	}
	Wire.endTransmission();
	Wire.beginTransmission(0x52);
	Wire.write(0x40); 				// sends memory address
	for (cnt = 12; cnt < 16; cnt++) {
		Wire.write(0x00); 			// sends 3rd key block (zeros)
	}
	Wire.endTransmission();
	delay(1);
	// end device init


}

void send_zero (){
	Wire.beginTransmission (0x52);	// transmit to device 0x52
	Wire.write (0x00);		// sends one byte
	Wire.endTransmission ();	// stop transmitting
}

void loop (){
	Wire.requestFrom (0x52, 6);	// request data from nunchuck
	while (Wire.available ())
	{
		outbuf[cnt] = nunchuk_decode_byte (Wire.read ());	// receive byte as an integer
		digitalWrite (ledPin, HIGH);	// sets the LED on
		cnt++;
	}

	// If we recieved the 6 bytes, then go print them
	if (cnt >= 5)
	{
		print_C (); //Datos procesados en C
		print_asm (); //Datos procesados en ensamblador
	}

	cnt = 0;
	send_zero (); // send the request for next bytes
	delay (100);
}

void print_asm (){
	unsigned char joy_x_axis = outbuf[0];
	unsigned char joy_y_axis = outbuf[1];
	int accel_x_axis; //  outbuf[2]
	int accel_y_axis; //  outbuf[3]
	int accel_z_axis; //  outbuf[4]

	unsigned char z_button = 0;
	unsigned char c_button = 0;

	asm volatile(
	"clr %B2; "			"\n\t"
	"clr %B3;"			"\n\t"
	"clr %B4;"			"\n\t"



	: "=&r" (z_button), "=&r" (c_button), "=&r" (accel_x_axis), "=&r" (accel_y_axis), "=&r" (accel_z_axis)
	: "r" (outbuf[2]), "r" (outbuf[3]), "r" (outbuf[4]), "r" (outbuf[5])
	: "r16"
	);
	
	Serial.print ("ASM: ");
	Serial.print (joy_x_axis, DEC);
	Serial.print (" ");
	Serial.print (joy_y_axis, DEC);
	Serial.print (" ");
	Serial.print (accel_x_axis, DEC);
	Serial.print (" ");
	Serial.print (accel_y_axis, DEC);
	Serial.print (" ");
	Serial.print (accel_z_axis, DEC);
	Serial.print (" ");
	Serial.print (z_button, DEC);
	Serial.print (" ");
	Serial.print (c_button, DEC);
	Serial.print (" ");
	Serial.print ("\r\n");
}

char nunchuk_decode_byte (char x)
{
	x = (x ^ 0x17) + 0x17;
	return x;
}

void print_C (){
	unsigned char joy_x_axis = outbuf[0];
	unsigned char joy_y_axis = outbuf[1];
	int accel_x_axis = outbuf[2] <<2;
	int accel_y_axis = outbuf[3] <<2;
	int accel_z_axis = outbuf[4] <<2;

	int z_button = 0;
	int c_button = 0;

	z_button=  (outbuf[5] & 0x1)!=0;
	c_button=  (outbuf[5] & 0x2)!=0;

	accel_x_axis=accel_x_axis+((outbuf[5] >> 2)&0x3);
	accel_y_axis=accel_y_axis+((outbuf[5] >> 4)&0x3);
	accel_z_axis=accel_z_axis+((outbuf[5] >> 6)&0x3);

	Serial.print ("C:   ");
	Serial.print (joy_x_axis, DEC);
	Serial.print (" ");
	Serial.print (joy_y_axis, DEC);
	Serial.print (" ");
	Serial.print (accel_x_axis, DEC);
	Serial.print (" ");
	Serial.print (accel_y_axis, DEC);
	Serial.print (" ");
	Serial.print (accel_z_axis, DEC);
	Serial.print (" ");
	Serial.print (z_button, DEC);
	Serial.print (" ");
	Serial.print (c_button, DEC);
	Serial.print (" ");
	Serial.print ("\r\n");
}
