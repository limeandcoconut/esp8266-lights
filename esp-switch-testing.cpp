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
// bool buttonReady = true;

int lastMillis = -400;
int currentMillis = 0;

void handleRoot() {
  int statusCode = 401;

  if (server.hasArg("state")) {
    lightState = server.arg("state").toInt();
    int statusCode = 200;

    // return;
  }

    // server.send(200, "text/plain", "current state: " + String(lightState));
  server.send(statusCode, "text/plain", "light state: " + String(lightState) + " " + String(millis() - lastMillis));
  // lightState = 1 - lightState;
  digitalWrite(LIGHT, lightState);

}

void setup(void){
  pinMode(LIGHT, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  digitalWrite(LIGHT, lightState);
//   Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
//   Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }

  digitalWrite(LIGHT, HIGH);
  delay(200);
  digitalWrite(LIGHT, LOW);
//   Serial.println("");
//   Serial.print("Connected to ");
//   Serial.println(ssid);
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
  MDNS.begin("esp8266");
//   if (MDNS.begin("esp8266")) {
//     Serial.println("MDNS responder started");
//   }

  server.on("/", handleRoot);

//   server.on("/inline", [](){
//     server.send(200, "text/plain", "this works as well");
//   });

//   server.onNotFound(handleNotFound);

  server.begin();
//   Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();

  currentMillis = millis();
  if ((currentMillis - lastMillis < 300)) {
  //   buttonReady
  // }

  // if (!buttonReady) {
    return;
  }

  buttonState = digitalRead(BUTTON);

  if (buttonState == HIGH && oldButtonState == LOW) {
    lightState = 1 - lightState;
    digitalWrite(LIGHT, lightState);
    // buttonReady = false;
    lastMillis = currentMillis;
  }


  oldButtonState = buttonState;
}