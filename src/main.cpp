#include "WiFi.h"
#include "WiFiClient.h"
#include "WebServer.h"
#include "ESPmDNS.h"
#include "SPIFFS.h"
#include "string" 

// Replace with your network credentials
const char *ssid = "HUAWEI P20 lite 8357";
const char *password = "12345678";

// Set LED GPIO
const int ledTrasero1 = 18;
const int ledTrasero2 = 4;

const int ledFrontarl1 = 2;
const int ledFrontarl2 = 23;

bool reversa = false;
bool lucesEncendidad = false;

// PWM
// const int ledCRed = 0;
// const int ledCGreen = 1;
// const int ledCBlue = 2;
// const int freq = 5000;
// const int res = 8;

// Stores LED state
// String ledState;

// Create AsyncWebServer object on port 80
WebServer server(80);


// Funcion para manejar la reversa
void handleReverse(){
  if (reversa){
    digitalWrite(ledTrasero1, HIGH);
    digitalWrite(ledTrasero2, HIGH);
  } else {
    digitalWrite(ledTrasero1, LOW);
    digitalWrite(ledTrasero2, LOW);
  }

  reversa =! reversa;
}

void handleLuces(){
  if (lucesEncendidad){
    digitalWrite(ledFrontarl1, HIGH);
    digitalWrite(ledFrontarl2, HIGH);
  } else {
    digitalWrite(ledFrontarl1, LOW);
    digitalWrite(ledFrontarl2, LOW);
  }

  lucesEncendidad =! lucesEncendidad;
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(ledTrasero1, OUTPUT);
  pinMode(ledTrasero2, OUTPUT);
  pinMode(ledFrontarl1, OUTPUT);
  pinMode(ledFrontarl1, OUTPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Conection successful");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    Serial.println();
  }

  // Print ESP32 Local IP Address
  Serial.print("Conectado a la red: ");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());


  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/styles.css", SPIFFS, "/styles.css");
  server.serveStatic("/script.js", SPIFFS, "/script.js");

  server.on("/reversa", handleReverse);
  server.on("/luces", handleLuces);


  // Start server
  server.begin();
}

void loop() {

  server.handleClient();

}
