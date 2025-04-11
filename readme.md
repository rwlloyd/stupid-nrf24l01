# stupid-nRF24l01 Communication

Add the ESP8266 libs

http://arduino.esp8266.com/stable/package_esp8266com_index.json

Add RF24 library to Arduino IDE in the usual way

RF24 Docs: https://maniacbug.github.io/RF24/classRF24.html

RF24 TL;DR:

- openWritingPipe() sets where you're sending.

- openReadingPipe(n, addr) sets what addresses you're listening for (up to 6).

- Pipe 0 is tied to both writing and reading — use pipe 1+ for clean setups.

- For reading pipes 1–5, the first 4 bytes must match.

- Always call startListening() after setting reading pipes.

## This Repo

### serial-chat

A simple chat app that works over the serial monitor between two nodes

### gateway

A simple gateway that communicates with two independent nodes.

gateway sends target location and bearing, recieves yaw and current location in response.

### node 

A simple node that is a companion to the gateway.

node receives location and bearing and returns dummy gps location and bearing.

