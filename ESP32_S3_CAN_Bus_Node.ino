/*
  Industrial Automation Protocols - Twin-Wire CAN Bus Network Node
  File: ESP32_S3_CAN_Bus_Node.ino
  Description: Manages multi-node message arrays via native ESP32-S3 TWAI driver layers.
*/

#include "driver/twai.h"

// Hardware pin mappings for integrated board transceivers
#define CAN_TX_PIN 20
#define CAN_RX_PIN 19

void setup() {
  Serial.begin(115200);

  // Set up the internal TWAI (Two-Wire Automotive Interface / CAN) driver configs
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX_PIN, (gpio_num_t)CAN_RX_PIN, TWAI_MODE_NORMAL);
  
  // Industrial default network clock speed configuration standard (250kbps payload pipeline)
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  // Install and turn on the CAN bus subsystem controllers
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("CAN Bus Subsystem Drivers Successfully Installed.");
  }
  if (twai_start() == ESP_OK) {
    Serial.println("CAN Bus Transceiver Pipelines Successfully Started.");
  }
}

void loop() {
  twai_message_t rx_msg;
  
  // Poll network input buffers for incoming frames from other automation nodes
  if (twai_receive(&rx_msg, pdMS_TO_TICKS(100)) == ESP_OK) {
    Serial.printf("[CAN Frame Received] ID: 0x%03X | DLC: %d | Data: ", rx_msg.identifier, rx_msg.data_length_code);
    
    for (int i = 0; i < rx_msg.data_length_code; i++) {
      Serial.printf("0x%02X ", rx_msg.data[i]);
    }
    Serial.println();
    
    // Command parser engine processing
    if (rx_msg.identifier == 0x10A && rx_msg.data[0] == 0xFF) {
      Serial.println("Network Command Match: Triggering Master Overhead Relay Line!");
    }
  }

  // Periodic frame heartbeat generation logic
  static unsigned long lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 5000) {
    lastHeartbeat = millis();
    
    twai_message_t tx_msg;
    tx_msg.identifier = 0x20B; // Node address descriptor label
    tx_msg.extd = 0;           // Standard 11-bit structural frame identifier configuration
    tx_msg.data_length_code = 2;
    tx_msg.data[0] = 0x55;     // Operational status heartbeat code flag 1
    tx_msg.data[1] = 0xAA;     // Operational status heartbeat code flag 2

    if (twai_transmit(&tx_msg, pdMS_TO_TICKS(50)) == ESP_OK) {
      Serial.println("Heartbeat frame transmitted across CAN Bus infrastructure.");
    }
  }
}
