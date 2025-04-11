#include <SPI.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

const byte addresses[][6] = {"00001", "00002"};

struct Setpoint {
  float targetLat;
  float targetLon;
  float targetHeading;
};

struct SensorData {
  float imuYaw;
  float compass;
  float gpsLat;
  float gpsLon;
};

void sendToNode(int nodeIndex, Setpoint sp) {
  radio.stopListening();

  // Send to the node
  radio.openWritingPipe(addresses[nodeIndex]);

  bool ok = radio.write(&sp, sizeof(sp));
  radio.startListening();

  if (ok && radio.available()) {
    SensorData sd;
    radio.read(&sd, sizeof(sd));

    Serial.print("Node ");
    Serial.print(nodeIndex);
    Serial.print(" → Yaw: "); Serial.print(sd.imuYaw);
    Serial.print(" | Compass: "); Serial.print(sd.compass);
    Serial.print(" | GPS: "); Serial.print(sd.gpsLat, 6);
    Serial.print(", "); Serial.println(sd.gpsLon, 6);
  } else {
    Serial.print("Failed to communicate with Node ");
    Serial.println(nodeIndex);
  }
}

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  // radio.setDataRate(RF24_250KBPS);
  radio.enableAckPayload();

  // Pipe needed for incoming ACK payloads (can use same address as writing)
  radio.openReadingPipe(0, addresses[0]);  // Just to enable reading ACKs
  radio.startListening();

  Serial.println("Gateway ready");
}

void loop() {
  Setpoint sp = {
    .targetLat = sp.targetLat + random(-50, 50) * 0.00001,
    .targetLon = sp.targetLat + random(-50, 50) * 0.00001,
    .targetHeading = random(0,360)
  };

  sendToNode(0, sp);  // Node 1
  delay(25);
  sendToNode(1, sp);  // Node 2
  delay(25);
}
