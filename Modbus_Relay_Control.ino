/*
  Waveshare Ecosystem Integration - RS485 Modbus RTU Relay Control
  File: Waveshare_Modbus_Relay_Control.ino
  Description: Controls a Waveshare Modbus RTU 8-Channel Relay Module over RS485.
*/

// Hardware serial pins for RS485 communication on ESP32-S3
#define RXD2_PIN 18
#define TXD2_PIN 17
#define RS485_EN_PIN 16 

// Hex command packets to control Waveshare 8-Ch Modbus Module (Device Address 01)
// Format: [Addr] [Func] [RegH] [RegL] [DataH] [DataL] [CRCL] [CRCH]
const uint8_t RELAY1_ON[]  = {0x01, 0x05, 0x00, 0x00, 0xFF, 0x00, 0x8C, 0x3A};
const uint8_t RELAY1_OFF[] = {0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0xCD, 0xCA};
const uint8_t RELAY2_ON[]  = {0x01, 0x05, 0x00, 0x01, 0xFF, 0x00, 0xDD, 0xFA};
const uint8_t RELAY2_OFF[] = {0x01, 0x05, 0x00, 0x01, 0x00, 0x00, 0x9C, 0x0A};
const uint8_t ALL_OFF[]    = {0x01, 0x0F, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0xBE, 0x55};

void sendModbusCommand(const uint8_t* command, size_t length) {
  // Switch RS485 chip transceiver to Transmit Mode
  digitalWrite(RS485_EN_PIN, HIGH);
  delay(2);
  
  // Send bytes over Hardware Serial 2
  Serial2.write(command, length);
  Serial2.flush(); // Wait for data transmission to complete
  
  // Return RS485 chip transceiver to Receive Mode
  digitalWrite(RS485_EN_PIN, LOW);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RS485_EN_PIN, OUTPUT);
  digitalWrite(RS485_EN_PIN, LOW);

  // Waveshare Modbus modules communicate at 9600 baud by default
  Serial2.begin(9600, SERIAL_8N1, RXD2_PIN, TXD2_PIN);
  Serial.println("Waveshare Modbus Relay Controller Initialized.");
  
  // Turn all relays off on boot for safety safety
  delay(500);
  sendModbusCommand(ALL_OFF, sizeof(ALL_OFF));
}

void loop() {
  Serial.println("Triggering Waveshare Relay 1: ON");
  sendModbusCommand(RELAY1_ON, sizeof(RELAY1_ON));
  delay(3000);

  Serial.println("Triggering Waveshare Relay 1: OFF");
  sendModbusCommand(RELAY1_OFF, sizeof(RELAY1_OFF));
  delay(2000);
  
  Serial.println("Triggering Waveshare Relay 2: ON");
  sendModbusCommand(RELAY2_ON, sizeof(RELAY2_ON));
  delay(3000);

  Serial.println("Triggering Waveshare Relay 2: OFF");
  sendModbusCommand(RELAY2_OFF, sizeof(RELAY2_OFF));
  delay(5000); // Wait 5 seconds before repeating sequence
}
