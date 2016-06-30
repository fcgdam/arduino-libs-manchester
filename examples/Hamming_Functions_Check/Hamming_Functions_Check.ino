/*

  This sketch tests the Hamming functions to see if they are working as they should, 
  without the need to use RF hardware.

  Random messagess and random errors are produced, "transmitted" and checked.
  
*/

#include <Manchester.h>

uint8_t data[32];
uint8_t ecdata[48];

uint8_t msgLen;

void setup() {
    Serial.setup(9600);

    randomSeed(analogRead(0));
}

// Create random messages

void createMsg(uint8_t msgLen) {
    uint8_t i;
    
    for ( i = 0 ; i < msgLen ; i++ )
        data[i] = random(256);
    
}


void loop() {
    uint8_t eclen,res;
    
    // Start a new message.
    
    // Select a random size for the message
    msgLen = random(32);
    
    // Create it
    createMsg(msgLen);
    
    // Print it on the screen
    Serial.println("Generated message: ");
    for(uint8_t i=1; i<receivedSize; i++)
      Serial.write(buffer[i]);
    
    Serial.println();    
    
    // Add Hamming EC
    eclen = mas.EC_encodeMessage( msgLen, data, ecdata );
    
    // Decide if we add noise
    if ( random(10) < 2 ) {
        
    }
    
    
    // Check and correct, if necessary the data
    
    
    // Wait for generating another message
    delay(2000);
    


}
