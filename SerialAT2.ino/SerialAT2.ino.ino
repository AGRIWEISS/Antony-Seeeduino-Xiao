// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1
#include <SoftwareSerial.h>


SoftwareSerial mySerial(9, 10); // RX, TX


void setup() {
  // Set console baud rate
  SerialMon.begin(9600);
  delay(3000);
//  pinMode(2, OUTPUT);
  mySerial.begin(9600);
}

void loop() {
//  digitalWrite(2, HIGH);
  delay(2000);
  
  SerialAT.begin(9600);
  delay(3000);
 
  mySerial.println("AT");
  mySerial.println("ATE");
  
  while(true) {
    if (SerialAT.available()) {
      
      SerialMon.write(SerialAT.read());
    }
    if (SerialMon.available()) {
      SerialAT.write(SerialMon.read());
    }
    //Serial.println("AT");
    //SerialMon.write(SerialAT.read());
    delay(0);
  }
}
