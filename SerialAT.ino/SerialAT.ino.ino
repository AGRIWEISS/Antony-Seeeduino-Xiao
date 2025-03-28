// Seeduino Xiao Soil Moisture Sensor with 0-2V soil moisture sensor
// Chilumba Machona - Agrisave Technology Pty Ltd 
// January 8th 2022
//Author: Chilumba, Antony.

// Arduino Low Power - Version: Latest
#include <ArduinoLowPower.h>

// Scheduler - Version: Latest
#include <Scheduler.h>

// ArduinoJson - Version: 5.13.4
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

// ArduinoRS485 - Version: Latest
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

// SD card libraries
#include <SPI.h>
#include <SD.h>

// Set serial for AT commands (to the module)
//#define SerialAT Serial1

// connection from Modbus to SeeeduinoXiao pin
#define MAX485_DE      8 
#define MAX485_RE_NEG  8 
#define MAX485_DI      6
#define MAX485_RO      7

//String at, atsend;

float temp ; // Define the moisture value 
float moisture;  // Define the temp value
const int CS_pin = 2;
String filename = "data.csv";
File myFile;

void setup() {
   // Set console baud rate
  Serial.begin(9600);
  
  RS485.setPins(MAX485_DI, MAX485_DE, MAX485_RE_NEG );
  Serial.begin(9600);
  while (!Serial);

  // Initialise SD card
Serial.print("Initializing SD card...");
  if (!SD.begin(CS_pin)) {
  Serial.println("initialization failed! try running the CardInfo sketch to debug");
  while (1);
}
  // Open a file
  myFile = SD.open(filename, FILE_WRITE);
  myFile.close();

  Serial.println("initialization done.");
  Serial.println("Modbus Temperature Humidity Sensor");
  // start the Modbus RTU client
  if (!ModbusRTUClient.begin(9600)) {
    Serial.println("Failed to start Modbus RTU Client!");
    while (1);
  }
  Serial.println("Client started");
}


void loop() {

 // SerialAT.begin(9600);
 // Serial1.println("AT+DEVEUI=?"); // Sending AT command to the RAK3172
  //Serial1.println("AT"); // Sending AT command to the RAK3172
  //Serial1.println("ATE"); // Sending Echo AT command to the RAK3172
  //Serial1.println("AT+JOIN=1:0:10:8"); // Sending RAK JOIN AT command to the RAK3172

  //Serial1.println("AT+SEND=3:12341234"); // Sending RAK SEND DATA AT command to the RAK3172
  
 // String returned_result = ""; // String to store the returned values 
//while (Serial1.available())
//{
   //returned_result = returned_result + Serial1.read();
//}
//Serial.println(returned_result); // Sending the response to the console
   
    // Reopen Modbus if SD was initialised
 if (!ModbusRTUClient.begin(9600)) {
 Serial.println("Failed to start Modbus RTU Client!");
 while (1);
}

  // send a Holding registers read request to (slave) id 1, for 2 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 0x00, 2))
  {
    Serial.print("failed to read registers! ");
    Serial.println(ModbusRTUClient.lastError());
  }
  else
  {
  short output_value = ModbusRTUClient.read();
  short output_value2 = ModbusRTUClient.read();

 // To get the temperature in Celsius and the humidity reading as
    // a percentage, divide the raw value by 10.0.
    temp = output_value / 100.0;
    moisture = output_value2 / 100.0;
    
  Serial.print("Mositure : "); // Print Moisture Label
  Serial.print(moisture); // Print Moisture Value 
  Serial.println("%"); // Print In % place holder 

  Serial.print("Temp: "); // Print Temp label
  Serial.print(temp); // Print Temp Value
  Serial.println("°C"); // Print °C place holder
  }

  String dataString = String(temp) + "," + String(moisture);
  saveData(dataString);
  delay(2000);
}

void saveData(String dataString) {
  //Reopen SD if Modbus was initialised
  if (!SD.begin(CS_pin)) {
  Serial.println("SD initialization failed!");
  while (1);
}
   //check the SD card is still there
  if (SD.exists(filename)) {
  myFile = SD.open(filename, FILE_WRITE);
  if (myFile) {
     Serial.print("Writing to " + filename + "... " +dataString);
      myFile.println(dataString);
      myFile.close();
      Serial.println(" done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening " + filename);
    }
  }
  else {
    Serial.println("Error writing to " + filename);
  }
}
