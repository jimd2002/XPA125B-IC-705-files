#include <SoftwareSerial.h>

#define CIV_RX_PIN 2
#define CIV_TX_PIN 3 // Not used in this example, but reserved for transmitting

SoftwareSerial civSerial(CIV_RX_PIN, CIV_TX_PIN);

void setup() {
  civSerial.begin(9600); // Start the CI-V communication
  Serial.begin(9600);    // Start serial communication for debugging

  Serial.println("Starting CI-V Frequency Readout");
}

void loop() {
  if (civSerial.available()) {
    byte civAddress = civSerial.read();  // Read the CI-V address byte

    Serial.print(civSerial);

    
    if (civAddress == 0xA4) {  // Check if the data is from the IC-705 (address 0xA4h)
      byte transceiverAddress = civSerial.read(); // Read the transceiver address (should be 0xE0)
      byte command = civSerial.read();            // Read the command byte
      
      if (command == 0x03) {  // Check if the command is "Read Frequency" (0x03)
        byte freqBytes[4];
        
        // Read the next 4 bytes which represent the frequency in BCD format
        for (int i = 0; i < 4; i++) {
          freqBytes[i] = civSerial.read();
        }

        // Convert BCD to frequency
        unsigned long frequency = (unsigned long)freqBytes[3] << 24 |
                                  (unsigned long)freqBytes[2] << 16 |
                                  (unsigned long)freqBytes[1] << 8  |
                                  (unsigned long)freqBytes[0];

        Serial.print("Frequency: ");
        Serial.print(frequency / 1000);
        Serial.println(" kHz");
      }
    }
  }
}