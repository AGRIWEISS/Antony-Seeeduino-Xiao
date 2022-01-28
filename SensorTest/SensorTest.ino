// Arduino Low Power - Version: Latest
#include <ArduinoLowPower.h>

// Scheduler - Version: Latest
#include <Scheduler.h>

// PubSubClient - Version: Latest


// ArduinoJson - Version: 5.13.4
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

// ArduinoRS485 - Version: Latest
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

// SD card libraries
#include <SPI.h>
#include <SD.h>

#define MAX485_DE      9 //3
#define MAX485_RE_NEG  9 //4
#define MAX485_DI     14 //1
#define MAX485_RO     13 //2

float temperature;
float moisture;
const int CS_pin = 7;
String filename = "data.csv";
File myFile;


void setup() {
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
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    short rawtemperature = ModbusRTUClient.read();
    short rawmoisture = ModbusRTUClient.read();

    // To get the temperature in Celsius and the humidity reading as
    // a percentage, divide the raw value by 10.0.
    temperature = rawtemperature / 100.0;
    moisture = rawmoisture / 100.0;
    Serial.print("Temperature:  ");
    Serial.print(temperature);
    Serial.print("  Volumetric Moisture Content:  ");
    Serial.println(moisture);

    String dataString = String(temperature) + "," + String(moisture);
    saveData(dataString);
  }

  delay(1000);
}

void saveData(String dataString) {
  // Reopen SD if Modbus was initialised
  if (!SD.begin(CS_pin)) {
    Serial.println("SD initialization failed!");
    while (1);
  }
  // check the SD card is still there
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
