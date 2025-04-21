#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

// WiFi credentials
const char* ssid = "";  // WiFi SSID
const char* password = "";  // WiFi password

// ThingSpeak settings
unsigned long myChannelNumber = ;  // ThingSpeak channel number
const char* myWriteAPIKey = "";  // ThingSpeak write API key

WiFiClient client;

void setup() {
  Serial.begin(9600); // Set the baud rate to match MSP430 settings
  WiFi.begin(ssid, password); // Start the WiFi connection

  while (WiFi.status() != WL_CONNECTED) { // Wait until the device is connected to WiFi
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop() {
  if (Serial.available() >= 5) { // Ensure there is enough data to parse
    unsigned char RH_byte1 = Serial.read();
    unsigned char RH_byte2 = Serial.read();
    unsigned char T_byte1 = Serial.read();
    unsigned char T_byte2 = Serial.read();
    unsigned char checksum = Serial.read();

    // Compute checksum to verify data integrity
    unsigned char calculatedChecksum = RH_byte1 + RH_byte2 + T_byte1 + T_byte2;
    if (calculatedChecksum == checksum) {
      // Calculate and send the data directly to ThingSpeak
      ThingSpeak.setField(1, static_cast<float>(T_byte1) + static_cast<float>(T_byte2) / 10.0f);
      ThingSpeak.setField(2, static_cast<float>(RH_byte1) + static_cast<float>(RH_byte2) / 10.0f);
      if (ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey) == ) {
        Serial.println("Data sent to ThingSpeak successfully.");
      } else {
        Serial.println("Failed to update ThingSpeak.");
      }
    } else {
      Serial.println("Checksum error.");
    }
  }
}