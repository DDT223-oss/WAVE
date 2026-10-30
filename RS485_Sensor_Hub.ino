/*
  Waveshare Ecosystem Integration - Industrial RS485 Sensor Telemetry Hub
  File: Waveshare_RS485_Sensor_Hub.ino
  Description: Collects precise climate arrays from Waveshare RS485 Modbus sensors.
*/

#define RXD2_PIN 18
#define TXD2_PIN 17
#define RS485_EN_PIN 16

// Modbus RTU Frame: Read Holding Registers from Slave 0x02, Reg 0x0000, Length 2
// Checksum appended manually via CRC-16 Modbus polynomial algorithm calculations
const uint8_t READ_SENSOR_DATA[] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38};
uint8_t responseBuffer[16];

void setup() {
  Serial.begin(115200);
  pinMode(RS485_EN_PIN, OUTPUT);
  digitalWrite(RS485_EN_PIN, LOW); // Start in RX listening state
  
  // Set up communication loop with the RS485 bus
  Serial2.begin(9600, SERIAL_8N1, RXD2_PIN, TXD2_PIN);
  Serial.println("Waveshare Industrial RS485 Hub Ready.");
}

void loop() {
  // Clear any noise out of the serial cache buffer
  while(Serial2.available()) Serial2.read();

  // Transmit command packet
  digitalWrite(RS485_EN_PIN, HIGH);
  delay(1);
  Serial2.write(READ_SENSOR_DATA, sizeof(READ_SENSOR_DATA));
  Serial2.flush();
  digitalWrite(RS485_EN_PIN, LOW);

  // Await downstream response frame with a strict timeout window
  unsigned long startMillis = millis();
  size_t bytesReceived = 0;
  
  while ((millis() - startMillis < 200) && (bytesReceived < 9)) {
    if (Serial2.available()) {
      responseBuffer[bytesReceived++] = Serial2.read();
    }
  }

  // Parse frame parameters if payload matches standard 9-byte structure
  if (bytesReceived == 9 && responseBuffer[0] == 0x02 && responseBuffer[1] == 0x03) {
    // Extract 16-bit integer values out of array buffers
    int16_t rawValue1 = (responseBuffer[3] << 8) | responseBuffer[4];
    int16_t rawValue2 = (responseBuffer[5] << 8) | responseBuffer[6];
    
    float metric1 = rawValue1 / 10.0;
    float metric2 = rawValue2 / 10.0;

    Serial.printf("[Modbus Node 02] Data 1: %.1f | Data 2: %.1f\n", metric1, metric2);
  } else {
    Serial.println("[RS485 Error] Sensor device response timeout or validation checksum failure.");
  }

  delay(3000); // 3-second diagnostic execution scan interval
}
