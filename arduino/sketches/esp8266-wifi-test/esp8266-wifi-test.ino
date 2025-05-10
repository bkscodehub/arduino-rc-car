#include <ESP8266WiFi.h>

#ifndef WIFI_SSID
#define WIFI_SSID "DEFAULT_SSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "DEFAULT_PASSWORD"
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");

    // Wait for client to send data
    while (!client.available()) {
      delay(1);
    }

    // Read first line of HTTP request
    String request = client.readStringUntil('\r');
    Serial.print("Request: ");
    Serial.println(request);
    client.readStringUntil('\n');  // consume the newline

    // Determine command
    if (request.indexOf("/?cmd=F") != -1) {
      Serial.println("Command: FORWARD");
    } else if (request.indexOf("/?cmd=B") != -1) {
      Serial.println("Command: BACKWARD");
    } else {
      Serial.println("Command: UNKNOWN");
    }

    // Send a proper HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("OK");

    delay(1000);  // allow client to receive data
    client.stop();
    Serial.println("Client disconnected.");
  }
}
