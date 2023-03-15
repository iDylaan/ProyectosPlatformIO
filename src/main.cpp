#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char *ssid = "Dani's Network";
const char *password = "kekojones1234";

// Set LED GPIO
const int led_r = 2;
const int led_g = 4;
const int led_b = 5;

// PWM
const int ledCRed = 0;
const int ledCGreen = 1;
const int ledCBlue = 2;
const int freq = 5000;
const int res = 8; 

// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  ledcSetup(ledCRed, freq, res);
  ledcSetup(ledCGreen, freq, res);
  ledcSetup(ledCBlue, freq, res);

  ledcAttachPin(led_r, ledCRed);
  ledcAttachPin(led_g, ledCGreen);
  ledcAttachPin(led_b, ledCBlue);

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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.println();
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  

  // Servimos los archivos estáticos que se encuentran en la carpeta "data"
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/styles.css", SPIFFS, "/styles.css");
  server.serveStatic("/script.js", SPIFFS, "/script.js");

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  // Route to load style.css file
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/styles.css", "text/css"); 
  });

  // Route to set GPIO to HIGH
  server.on("/led_r", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      int value = request->getParam("value")->value();
    }
    ledcWrite(ledCRed, 255);
    request->send(SPIFFS, "/index.html", String(), false); 
  });

  // Route to set GPIO to LOW
  server.on("/led_r_off", HTTP_GET, [](AsyncWebServerRequest *request) {
    ledcWrite(ledCRed, 0);
    request->send(SPIFFS, "/index.html", String(), false); 
  });

  // Start server
  server.begin();
}

void loop()
{
}
