#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "AsyncTCP.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "string"

// Replace with your network credentials
const char *ssid = "Dani's Network";
const char *password = "kekojones1234";

// Set LED GPIO
const int led_r = 4;
const int led_g = 2;
const int led_b = 5;

// Set POTENCIOMETER GPIO
const int pote_1 = 33;
const int pote_2 = 32;
const int pote_3 = 34;

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

void handlePut(AsyncWebServerRequest *request) {
  // Send OK response
  request->send(200);
}

// Function to handle body of PUT request
void handleLeds(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  // Create a buffer to store the body data
  char buffer[total + 1];
  // Copy the data to the buffer and add a null terminator
  memcpy(buffer, data, total);
  buffer[total] = '\0';

  // Create a JSON document with enough capacity to store the object
  StaticJsonDocument<64> doc;

  // Parse the JSON object from the buffer
  DeserializationError error = deserializeJson(doc, buffer);

  // Check for parsing errors
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Get the values of led1, led2 and led3 from the JSON object
  String led1 = doc["led1"];
  String led2 = doc["led2"];
  String led3 = doc["led3"];

  // Print the values for debugging purposes
  Serial.print("Red: ");
  Serial.println(led1);
  Serial.print("Green: ");
  Serial.println(led2);
  Serial.print("Blue: ");
  Serial.println(led3);

  // Turn on or off the LEDs according to the values received
  ledcWrite(ledCRed, led1.toInt());
  ledcWrite(ledCGreen, led2.toInt());
  ledcWrite(ledCBlue, led3.toInt());
}

void setup()
{
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
  Serial.print("Conectado a la red: ");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
    request->send(SPIFFS, "/index.html", "text/html"); 
  });  

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) { 
    request->send(SPIFFS, "/styles.css", "text/css"); 
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) { 
    request->send(SPIFFS, "/script.js", "application/javascript"); 
  });

  // Route to set GPIO to HIGH
  server.on("/led_r", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("value")) {
      String value = request->getParam("value")->value();
      int brightness = value.toInt();
      ledcWrite(ledCRed, brightness);
    }

    request->send(SPIFFS, "/index.html", String(), false); 
  });

  server.on("/led", HTTP_POST, handlePut, NULL, handleLeds);

  // Route to get current time as JSON object
  server.on("/potenciometros", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    doc["pote1"] = analogRead(pote_1);
    doc["pote2"] = analogRead(pote_2);
    doc["pote3"] = analogRead(pote_3);
    serializeJson(doc, *response);
    request->send(response); 
  });

  // Start server
  server.begin();
}

void loop(){}
