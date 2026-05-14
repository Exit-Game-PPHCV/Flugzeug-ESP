#include "Zigbee.h"
#include <ESP32Servo.h>

// --- Konfiguration ---
#define SERVO1_PIN 4      // GPIO Pin für Pitch
#define SERVO2_PIN 5      // GPIO Pin für Roll

#define ENDPOINT1_ID 10   // Zigbee Endpoint ID für Pitch
#define ENDPOINT2_ID 11   // Zigbee Endpoint ID für Roll

Servo servo1;
Servo servo2;

// WICHTIG: Wir nutzen "ZigbeeDimmableLight" für Werte von 0-254
ZigbeeDimmableLight zigbeeDevice1(ENDPOINT1_ID);
ZigbeeDimmableLight zigbeeDevice2(ENDPOINT2_ID);

void setup() {
  Serial.begin(115200);

  // --- Servos initialisieren ---
  servo1.setPeriodHertz(50);
  servo1.attach(SERVO1_PIN, 500, 2400); 
  servo1.write(90); // Startposition: 90 Grad (Mitte)

  servo2.setPeriodHertz(50);
  servo2.attach(SERVO2_PIN, 500, 2400); 
  servo2.write(90); // Startposition: 90 Grad (Mitte)

  // --- Zigbee konfigurieren ---
  zigbeeDevice1.setManufacturerAndModel("Flugzeug", "Pitch");
  zigbeeDevice2.setManufacturerAndModel("Flugzeug", "Roll");

  // Die beiden Endpoints im Zigbee-System des ESP registrieren
  Zigbee.addEndpoint(&zigbeeDevice1);
  Zigbee.addEndpoint(&zigbeeDevice2);

  // --- Callback für Servo 1 (Pitch) ---
  zigbeeDevice1.onLightChange([](bool state, uint8_t level) {
    if (state) {
      Serial.printf("Pitch - Zahl empfangen: %d\n", level);
      
      //int angle = map(level, 0, 254, 45, 135);
      
      //Serial.printf("Bewege Pitch auf: %d Grad\n", angle);
      servo1.write(level);
    }
  });

  // --- Callback für Servo 2 (Roll) ---
  zigbeeDevice2.onLightChange([](bool state, uint8_t level) {
    if (state) {
      Serial.printf("Roll - Zahl empfangen: %d\n", level);
      
      //int angle = map(level, 0, 254, 45, 135);
      
      //Serial.printf("Bewege Roll auf: %d Grad\n", angle);
      servo2.write(level);
    }
  });

  // --- Zigbee starten ---
  Zigbee.begin();
  Serial.println("Zigbee gestartet. Versetze deinen Controller in den Pairing-Modus!");
}

void loop() {
  // Die Zigbee-Kommunikation und das Bewegen der Servos passieren 
  // komplett asynchron im Hintergrund.
  delay(100);
}