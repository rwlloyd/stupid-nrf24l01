/*
Simple two way text communication over serial terminal

Target devices: ESP8266 (Wemos D1 Mini) or Arduino Nano Equipped with nRF24l01 modules

rwlloyd, Lincoln, 04/2025
*/

#include <SPI.h>
#include <RF24.h>

// Define NRF24L01 pins for Wemos D1 Mini
#ifdef ESP32
#define CE_PIN 4
#define CSN_PIN 5
#else
// Define NRF24L01 pins for Arduino Nano
#define CE_PIN 9
#define CSN_PIN 10
#endif

// #define NODE0 //Default: NODE1

// Create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Address for communication
const byte addresses[][6] = { "00001", "00002" }; //only the last byte can be different

void setup() {
  Serial.begin(115200);
  radio.begin();
#ifdef NODE0
  radio.openWritingPipe(addresses[1]);     // 00002
  radio.openReadingPipe(1, addresses[0]);  // 00001 (Listening on pipe 1)
#else
  radio.openWritingPipe(addresses[0]);     // 00001
  radio.openReadingPipe(1, addresses[1]);  // 00002
#endif
  // radio.enableDynamicPayloads();

  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  Serial.println("Node Ready");
}

void loop() {
  // Check for incoming messages
  if (radio.available()) {
    char receivedMessage[32] = { 0 };
    radio.read(&receivedMessage, sizeof(receivedMessage));
    Serial.print("Received: ");
    Serial.println(receivedMessage);
  }

  // Check for user input from Serial Monitor
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    message.trim();                                               // Clean up whitespace/newlines
    if (message.length() > 0) {
      char messageBuffer[32] = { 0 };                             // Max 32 bytes for NRF24
      message.toCharArray(messageBuffer, sizeof(messageBuffer));  //convert bytes to Char Array
        
      radio.stopListening();                                       // stop listening while we write
      radio.write(&messageBuffer, sizeof(messageBuffer));
      radio.startListening();

      Serial.print("Sent: ");
      Serial.println(message);
    }
  }
}
