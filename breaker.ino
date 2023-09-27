#include <esp_wifi.h>
#include <WiFi.h>
#include <HardwareSerial.h>


#define SSID "<your_SSID>"
#define PASSWORD "<your_Password>"
#define SPEED 115200
#define PORT 8080

WiFiServer server(PORT);

const int Led_pin = 2;
const int Key_pin = 13;
const int D1_pin = 12;
const int D2_pin = 14;

void setup() {
  Serial.begin(SPEED);
  pinMode(Key_pin, OUTPUT);
  pinMode(Led_pin, OUTPUT);
  pinMode(D1_pin, OUTPUT);
  pinMode(D2_pin, OUTPUT);

  digitalWrite(Key_pin, LOW);
  digitalWrite(D1_pin, HIGH);
  digitalWrite(D2_pin, HIGH);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);

    delay(1500);
    Serial.println("Connecting to WiFi...");
  }

  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String command = "";

    Serial.println("Client connected");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        command += c;

        if (c == '\r') {
          if (command == "ping\r") {
            client.print("USB_BREAKER\r");
            Serial.println("ping");
          } else {
            Serial.print("Received command: ");
            Serial.println(command);

            if (command == "Check_status\r") {
              snprintf(status_string, sizeof(status_string), "{\"lines\": [{\"name\": \"Key\",\"status\": \"%s\"},"
                                                                          "{\"name\": \"D+\",\"status\": \"%s\"},"
                                                                          "{\"name\": \"D-\",\"status\": \"%s\"}]}\r",
                                                                            digitalRead(Key_pin) ? "closed" : "open",
                                                                            digitalRead(D1_pin) ? "open" : "closed",
                                                                            digitalRead(D2_pin) ? "open" : "closed");
              digitalWrite(LED_BUILTIN, HIGH);
              delay(200);
              digitalWrite(LED_BUILTIN, LOW);

              client.print(status_string);
              memset(status_string, 0, sizeof(status_string));
            } else if (command == "Key_on\r") {
              digitalWrite(Key_pin, LOW); // Включение Vbus
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else if (command == "Key_off\r") {
              digitalWrite(Key_pin, HIGH); // Выключение Vbus
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else if (command == "D1_on\r") {
              digitalWrite(D1_pin, HIGH);
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else if (command == "D1_off\r") {
              digitalWrite(D1_pin, LOW);
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else if (command == "D2_on\r") {
              digitalWrite(D2_pin, HIGH);
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else if (command == "D2_off\r") {
              digitalWrite(D2_pin, LOW);
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else if(command == "All_on\r"){
              digitalWrite(Key_pin, LOW);

              delay(10);
              digitalWrite(D1_pin, HIGH);
              digitalWrite(D2_pin, HIGH);
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else if(command == "All_off\r"){
              digitalWrite(D1_pin, LOW);
              digitalWrite(D2_pin, LOW);
              delay(10);
              digitalWrite(Key_pin, HIGH);
              digitalWrite(LED_BUILTIN, HIGH);
              delay(20);
              digitalWrite(LED_BUILTIN, LOW);
              client.print("Ok\r");
            } else {
              client.print("Error: Invalid command\r");
              Serial.println("Error: Invalid command - " + command);
            }
          }
          command = "";
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");

  }
}
