#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>          // For NTP

#ifndef WIFI_SSID
#define WIFI_SSID "DEFAULT_SSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "DEFAULT_PASSWORD"
#endif

#ifndef HIVEMQ_USERNAME
#define HIVEMQ_USERNAME "DEFAULT_HIVEMQ_USERID"
#endif

#ifndef HIVEMQ_PASSWORD
#define HIVEMQ_PASSWORD "DEFAULT_HIVEMQ_PASSWORD"
#endif

#ifndef HIVEMQ_BROKER_URL
#define HIVEMQ_BROKER_URL "DEFAULT_HIVEMQ_BROKER_URL"
#endif

#ifndef HIVEMQ_BROKER_PORT
#define HIVEMQ_BROKER_PORT 0
#endif

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// SoftwareSerial espToArduino(2, 3); // RX, TX (to Arduino TX, RX)

const char* mqtt_server = HIVEMQ_BROKER_URL;
const int mqtt_port = HIVEMQ_BROKER_PORT;
const char* mqtt_user = HIVEMQ_USERNAME;
const char* mqtt_password = HIVEMQ_PASSWORD;

BearSSL::WiFiClientSecure espClient;
PubSubClient client(espClient);

// HiveMQ Cloud Let's Encrypt CA certificate (hardcoded)
static const char ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

// Time has to be obtained from NTP, because of CA expiration date validation.
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

#define COMMAND_TOPIC "car/command"
#define TELEMETRY_TOPIC "car/telemetry"
#define STATUS_TOPIC "car/status"

unsigned long lastHeartbeat = 0;
const unsigned long HEARTBEAT_INTERVAL = 60000; // 60 seconds

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);

  if (topicStr == COMMAND_TOPIC) {
    String msg;
    for (int i = 0; i < length; i++) msg += (char)payload[i];
    Serial.print("Command from MQTT: ");
    Serial.println(msg);
    Serial.print(">");
    Serial.println(msg); // Pass to Arduino via serial
    // espToArduino.print(">");
    // espToArduino.println(msg); // Pass to Arduino via serial
  }
}

void setup_wifi() {
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, IPAddress(8,8,8,8)); // Use Google's DNS
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  char err_buf[256];
  // Serial.println("(Re)Connecting to HiveMQ...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // MQTT client ID
    String clientId = "ESP8266Client-" + String(ESP.getChipId());
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(COMMAND_TOPIC);
      Serial.println("MQTT subscribed.");
      client.publish(STATUS_TOPIC, "connected");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.println(client.state());
      espClient.getLastSSLError(err_buf, sizeof(err_buf));
      Serial.print("SSL error: ");
      Serial.println(err_buf);
      Serial.println("Try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  // espToArduino.begin(9600); // Connection to Arduino
  
  BearSSL::X509List *serverTrustedCA = new BearSSL::X509List(ca_cert);
  espClient.setTrustAnchors(serverTrustedCA);
  setup_wifi();
  setClock(); // Required for X.509 validation
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  //Car Heartbeat
  if (millis() - lastHeartbeat > HEARTBEAT_INTERVAL) {
    lastHeartbeat = millis();
    time_t now = time(nullptr);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    
    char timeBuffer[32];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    
    String heartbeat = String("{\"status\":\"alive\",") +
                       "\"timestamp\":\"" + timeBuffer + "\"," +
                       "\"ip\":\"" + WiFi.localIP().toString() + "\"," +
                       "\"rssi\":" + String(WiFi.RSSI()) + "}";
                       
    client.publish(STATUS_TOPIC, heartbeat.c_str());
  }

  // Read serial from Arduino (e.g., {"1":25})
  if (Serial.available()) {
    String json = Serial.readStringUntil('\n');
    if (json.length() > 2) {
      client.publish(TELEMETRY_TOPIC, json.c_str());
    }
  }
}
