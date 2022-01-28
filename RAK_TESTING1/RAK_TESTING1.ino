// Set serial for debug console (to the Serial Monitor, speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1


void setup() {
  // Set console baud rate
  SerialMon.begin(9600);
  pinMode(4,OUTPUT);
  delay(3000);
  digitalWrite(4, HIGH);
    delay(2000);
}

void loop() {
  for(int x=0; x<5; x++){
    
    SerialAT.begin(9600);
    delay(3000);
    if (SerialAT.available()){
      Serial.print("test1");
    }
    Serial1.print("AT");
    Serial1.println('AT');
    Serial1.println("ATE");
    
    while(true) {
      if (SerialAT.available()) {
        
        SerialMon.write(SerialAT.read());
      }
      if (SerialMon.available()) {
        SerialAT.write(SerialMon.read());
      }
      else{
        break;    
      }
  }
  
  
    //Serial.println("AT");
    //SerialMon.write(SerialAT.read());
    delay(0);
  }
}
