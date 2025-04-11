#include <SPI.h>
#include <RF24.h>

#define NODE_ID 0  // Set to 1 or 2

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

// 5-byte ASCII addresses (NOT null-terminated)
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

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  // radio.setDataRate(RF24_250KBPS);
  radio.enableAckPayload();

  // Open reading pipe for this node (pipe 0 for ackPayload)
  radio.openReadingPipe(0, addresses[NODE_ID]);
  // Gateway sends to this node
  radio.openWritingPipe(addresses[NODE_ID]);

  radio.startListening();

  // Preload ackPayload with dummy data
  SensorData dummy = {0.0, 0.0, 0.0, 0.0};
  radio.writeAckPayload(0, &dummy, sizeof(dummy));

  Serial.print("Node ");
  Serial.print(NODE_ID);
  Serial.println(" ready");
}

void loop() {
  if (radio.available()) {
    Setpoint sp;
    radio.read(&sp, sizeof(sp));

    Serial.println("Setpoint received:");
    Serial.print("Lat: "); Serial.println(sp.targetLat, 6);
    Serial.print("Lon: "); Serial.println(sp.targetLon, 6);
    Serial.print("Heading: "); Serial.println(sp.targetHeading, 2);

    // Simulated response
    SensorData sd = {
      .imuYaw = random(0, 360),
      .compass = random(0, 360),
      .gpsLat = sp.targetLat + random(-50, 50) * 0.00001,
      .gpsLon = sp.targetLon + random(-50, 50) * 0.00001
    };

    radio.writeAckPayload(0, &sd, sizeof(sd));
  }
}
