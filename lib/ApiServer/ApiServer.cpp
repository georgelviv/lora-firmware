#include "ApiServer.h"

const String ssid = "LoraTunning";
const String password = "2233445566";

AsyncWebServer server(80);

void ApiServer::setup() {

  this->logger = new Logger("API Server", true);

  this->setupWiFi();
  this->setupServer();
}

void ApiServer::setupWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  this->logger->log("IP:", ip.toString());
}

void ApiServer::setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "LoraTunning Hotspot OK");
  });

  server.begin();
}