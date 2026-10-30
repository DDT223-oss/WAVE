/*
  Waveshare ESP32-S3 Industrial Control Board - Relay & Input Demo
  File: Relay_Input_Controller.ino
  Description: Debounced optocoupled inputs controlling industrial relays.
*/

// Define GPIO pins based on Waveshare ESP32-S3 Industrial board schematics
// (Adjust these pin numbers if your specific board variant differs)
const int INPUT_PINS[] = {4, 5, 6, 7};   // Optocoupled Inputs
const int RELAY_PINS[] = {12, 13, 14, 15}; // Relay Outputs
const int NUM_CHANNELS = 4;

// Debounce arrays
unsigned long lastDebounceTime[NUM_CHANNELS] = {0};
const unsigned long DEBOUNCE_DELAY = 50; // milliseconds
int lastInputState[NUM_CHANNELS] = {HIGH};
int currentInputState[NUM_CHANNELS] = {HIGH};

void setup() {
  Serial.begin(115200);
  
  // Initialize Relays (Outputs)
  for (int i = 0; i < NUM_CHANNELS; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], LOW); // Ensure relays start OFF
  }
  
  // Initialize Isolated Inputs (Typically active low or requires internal pullup)
  for (int i = 0; i < NUM_CHANNELS; i++) {
    pinMode(INPUT_PINS[i], INPUT_PULLUP);
  }
  
  Serial.println("System Initialized: 4-Channel Relay Control Ready.");
}

void loop() {
  for (int i = 0; i < NUM_CHANNELS; i++) {
    int reading = digitalRead(INPUT_PINS[i]);

    // Check if the physical switch state changed
    if (reading != lastInputState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > DEBOUNCE_DELAY) {
      // If the state has stabilized longer than the delay, accept it
      if (reading != currentInputState[i]) {
        currentInputState[i] = reading;

        // If input goes LOW (button pressed / sensor tripped)
        if (currentInputState[i] == LOW) {
          // Toggle the corresponding relay
          bool currentRelayState = digitalRead(RELAY_PINS[i]);
          digitalWrite(RELAY_PINS[i], !currentRelayState);
          
          Serial.printf("Input %d triggered! Toggling Relay %d to %s\n", 
                        i + 1, i + 1, !currentRelayState ? "ON" : "OFF");
        }
      }
    }
    lastInputState[i] = reading;
  }
}
