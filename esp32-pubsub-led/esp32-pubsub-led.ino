#include <WiFi.h>
#include <ESP32Ping.h>
//#include <HTTPClient.h>
#include <PubSubClient.h>

const int ledPin = 21;

const char* wifi_ssid = "";
const char* wifi_password = "";

const char* mqtt_topic_sub = "test-esp32/led";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  WiFi.begin(wifi_ssid, wifi_password);

  Serial.println("Conecting to WiFi...");

  unsigned long startAttemptTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nSuccessfully connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Connection failed. Check credentials or network.");
  }

  if (Ping.ping(IPAddress(8, 8, 8, 8))) {
    Serial.println("Ping successful on google.com");
  } else {
    Serial.println("Ping failure.");
  }

  //HTTPClient http;
  //http.begin("https://google.com.br");
  //int httpCode = http.GET();
  //if (httpCode > 0) {
  //  Serial.printf("HTTP response code: %d\n", httpCode);
  //} else {
  //  Serial.printf("HTTP request error: %s\n", http.errorToString(httpCode).c_str());
  //}
  //http.end();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received in topic: ");
  Serial.println(topic);

  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("msg: ");
  Serial.println(msg);

  if (msg == "on") {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
  } else if (msg == "off") {
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
  } else {
    Serial.println("Unknown command");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Trying to connect to MQTT...");

    String clientId = "TestESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Connected to the MQTT broker");
      client.subscribe(mqtt_topic_sub);
    } else {
      Serial.print("Connection failed. Error code: ");
      Serial.print(client.state());
      Serial.println("Trying again in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  //digitalWrite(ledPin, HIGH);
  //delay(5000);
  //digitalWrite(ledPin, LOW);
  //delay(1000);

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}
