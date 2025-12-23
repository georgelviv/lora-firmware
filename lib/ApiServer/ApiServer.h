#ifndef API_SERVER_H
#define API_SERVER_H

#include <WiFi.h>
#include <Arduino.h>
#include <Logger.h>
#include <ESPAsyncWebServer.h>

class ApiServer {
  public:
    void setup();

  private:
    void setupWiFi();
    void setupServer();

    Logger* logger;
};

#endif