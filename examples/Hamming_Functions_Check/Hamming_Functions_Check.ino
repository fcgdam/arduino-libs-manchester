/*

  This sketch tests the Hamming functions to see if they are working as they should, 
  without the need to use RF hardware.

  Random messagess and random errors are produced, "transmitted" and checked.
  
*/

#include <Manchester.h>

uint8_t data[32];
uint8_t dataout[32];
uint8_t ecdata[48];

uint8_t msgLen;

void setup() {
    Serial.begin(9600);

    randomSeed(analogRead(0));
}

// Create random messages

void createMsg(uint8_t msgLen) {
    uint8_t i;
    
    for ( i = 0 ; i < msgLen ; i++ )
        data[i] = random(256);
    
}

void printArray( uint8_t *data , uint8_t dlen )
{
    
    for(uint8_t i = 0; i < dlen ; i++) {
      if (data[i]<0x10) {Serial.print("0");}
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();   
}

void flipRandomBit(uint8_t *ec_data , uint8_t dlen) {

  int rnpo = random(dlen);  // Select one random message byte
  int rndb = random( 8 );     // Select one random bit
  uint8_t mask = 1 << rndb ;  // Create a mask

  ec_data[ rnpo ] ^= mask;       // and flip it with XOR.
    
}


void loop() {
    uint8_t eclen,res , ddata;
    
    // Start a new message.
    Serial.println("--------------------------------------------------------------------------------------------------------------");
    // Select a random size for the message
    msgLen = 2 + random(8);  // At least 2 bytes of data....
    
    // Create it
    createMsg(msgLen);
    
    // Print it on the screen
    Serial.println("Generated message: ");
    printArray( data , msgLen );
    
    // Add Hamming EC
    eclen = man.EC_encodeMessage( msgLen, data, ecdata );

    // Print it on the screen
    Serial.println("Message with parity: ");
    printArray( ecdata , eclen ); 
    
    // Decide if we add noise
    if ( random(10) < 5 ) {
        int k, nb;
        // Let's decide how many bit's we will flip
        nb = random( 16 );
        Serial.println("Introducing noise: ");
        Serial.print(" Flipping " );
        Serial.print( nb );
        Serial.println(" bits.");

        for ( k = 0 ; k < nb ; k++ ) {
          flipRandomBit(ecdata, eclen );
        }
        // Print it on the screen
        Serial.println("Message with possible errors: ");
        printArray( ecdata , eclen ); 
    }
    
    Serial.println("EC processing: ");
    // Check and correct, if necessary the data
    res = man.EC_decodeMessage( eclen, ecdata, &ddata, dataout );
    Serial.print("Result: ");
    Serial.println( res , HEX );

    
    // Print it on the screen
    Serial.println("Message with parity: ");
    printArray( dataout , ddata ); 

    switch ( res ) {
      case NO_ERROR:
          Serial.println("Message received OK.");
          break;
      case ERROR_IN_PARITY:
          Serial.println("Message received with PARITY errors but corrected.");
          break;
      case UNCORRECTABLE:
          Serial.println("Message received with UNCORRECTABLE errors.");
          break;
    }
    
    // Wait for generating another message
    delay(2000);
    


}
