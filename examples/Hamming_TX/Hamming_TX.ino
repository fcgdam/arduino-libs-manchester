
#include <Manchester.h>

/*

  Manchester Transmitter example
  
  In this example transmitter will send 10 bytes array  per transmittion

  try different speeds using this constants, your maximum possible speed will 
  depend on various factors like transmitter type, distance, microcontroller speed, ...

  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7

*/

#define TX_PIN  1  //pin where your transmitter is connected
#define LED_PIN 2 //pin for blinking LED

uint8_t moo = 1; //last led status
uint8_t data[20] = {11, '1','2', '3', '4', '5', '6', '7', '8', '9','1','2','3','4','5','6','7','8','9'};

uint8_t ecdata[32];
uint8_t eclen;

void setup() 
{
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_600);
}


uint8_t datalength=2;   //at least two data
void loop() 
{

  data[0] = datalength;

  // Given the data, add Hamming EC data
  eclen = man.EC_encodeMessage( datalength, data, ecdata );

  //Serial.println( eclen );
  man.transmitArray( eclen, ecdata);
  
  moo = ++moo % 2;
  digitalWrite(LED_PIN, moo);


  delay(1000);
  datalength++;
  if(datalength>18) datalength=2;
}
