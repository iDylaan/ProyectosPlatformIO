#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>

const char *ssid = "Dani's network";
const char *password = "kekojones1234";

WebServer server(5252);

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Conection successful");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  while (WiFi.waitForConnectResult()!= WL_CONNECTED) {
    Serial.print("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }
  // Iniciamos el sistema de archivos SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("Error al montar el sistema de archivos SPIFFS");
    return;
  }

  // Servimos los archivos estáticos que se encuentran en la carpeta "data"
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/styles.css", SPIFFS, "/styles.css");
  server.serveStatic("/script.js", SPIFFS, "/script.js");

  // Iniciamos el servidor web
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}
