/*
  Waveshare ESP32-S3 Industrial Control Board - Modbus RTU RS485 Master
  File: Modbus_RS485_Master.ino
  Description: Reads data from an industrial Modbus slave sensor over RS485.
*/

#include <ModbusMaster.h>

// RS485 Hardware Serial Pins on ESP32-S3
#define RXD2_PIN 18
#define TXD2_PIN 17
#define RS485_EN_PIN 16 // Flow control / Transmit Enable pin

ModbusMaster node;

// Callback functions to toggle RS485 chip between Transmit and Receive modes
void preTransmission() {
  digitalWrite(RS485_EN_PIN, HIGH); 
}

void postTransmission() {
  digitalWrite(RS485_EN_PIN, LOW);  
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RS485_EN_PIN, OUTPUT);
  digitalWrite(RS485_EN_PIN, LOW); // Start in Receive mode

  // Initialize Hardware Serial 2 for RS485 at 9600 baud (standard for Modbus)
  Serial2.begin(9600, SERIAL_8N1, RXD2_PIN, TXD2_PIN);

  // Modbus slave ID configuration (Default is usually 1)
  node.begin(1, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  Serial.println("Modbus RTU RS485 Master Initialized.");
}

void loop() {
  uint8_t result;
  
  // Read 2 holding registers starting at address 0x0000 (Common for Temp/Humidity)
  result = node.readHoldingRegisters(0x0000, 2);
  
  if (result == node.ku8MBSuccess) {
    // Extract data from the buffer response
    float temperature = node.getResponseBuffer(0) / 10.0;
    float humidity = node.getResponseBuffer(1) / 10.0;
    
    Serial.printf("Modbus Read Success -> Temp: %.2f °C | Humidity: %.2f %%\n", temperature, humidity);
  } else {
    Serial.printf("Modbus Read Error: 0x%02X\n", result);
  }
  
  delay(2000); // Poll every 2 seconds
}
