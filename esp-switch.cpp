#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "_";
const char* password = "i forgot";

ESP8266WebServer server(80);

const int LIGHT = 4;
int lightState = 0;

const int BUTTON = 5;
int buttonState = 1;
int oldButtonState = buttonState;

int lastMillis = -400;
int currentMillis = 0;

void handleRoot() {
  if (server.hasArg("state")) {
    lightState = server.arg("state").toInt();
  }
  server.send(200, "text/plain", "light state: " + String(lightState) + " " + String(millis() - lastMillis));
  digitalWrite(LIGHT, lightState);
}

void setup(void){
  pinMode(LIGHT, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  digitalWrite(LIGHT, lightState);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  digitalWrite(LIGHT, HIGH);
  delay(200);
  digitalWrite(LIGHT, LOW);

  MDNS.begin("esp8266");

  server.on("/", handleRoot);
  server.begin();
}

void loop(void){
  server.handleClient();

  currentMillis = millis();
  if ((currentMillis - lastMillis < 300)) {
    return;
  }

  buttonState = digitalRead(BUTTON);

  if (buttonState == HIGH && oldButtonState == LOW) {
    lightState = 1 - lightState;
    digitalWrite(LIGHT, lightState);
    lastMillis = currentMillis;
  }

  oldButtonState = buttonState;
}