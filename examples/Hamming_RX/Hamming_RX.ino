#include "Manchester.h"

/*

  Manchester Receiver example with Hamming Code Error Correction
  
  In this example receiver will receive array of 10 bytes per transmittion

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

#define RX_PIN 4
#define LED_PIN 13

uint8_t moo = 1 , res , ddata;
#define BUFFER_SIZE 22
uint8_t buffer[BUFFER_SIZE];
uint8_t data[BUFFER_SIZE];


void printArray( uint8_t *data , uint8_t dlen )
{
    
    for(uint8_t i = 0; i < dlen ; i++) {
      if (data[i]<0x10) {Serial.print("0");}
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();   
}

void setup() 
{
  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, moo);
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_600);
  man.beginReceiveArray(BUFFER_SIZE, buffer);

  Serial.println("Starting...");
}

void loop() 
{
  if (man.receiveComplete()) 
  {
    uint8_t receivedSize = 0;

    //do something with the data in 'buffer' here before you start receiving to the same buffer again
    receivedSize = buffer[0];
    Serial.println("Received data from RF: ");
    printArray( &buffer[1] , receivedSize );

    // Now let's apply EC
    res = man.EC_decodeMessage( receivedSize , &buffer[1], &ddata, data );

    // And print the resulting EC message.
    Serial.println("EC message: ");
    printArray( data, ddata );

    man.beginReceiveArray(BUFFER_SIZE, buffer);
    moo = ++moo % 2;
    digitalWrite(LED_PIN, moo);
  }
}
