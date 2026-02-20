#pragma once

#include <WiFi.h>
#include <ProjectConstants.h>
#include <Arduino.h>
#include <Logger.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

class ApiServer {
  public:
    using DataCallback = void (*)(const String);

    void setup();
    void send(String msg);
    void setDataCallback(DataCallback cb);

  private:
    void setupWiFi();
    void setupServer();

    Logger* logger;
    static AsyncWebSocket ws;
    DataCallback dataCallback = nullptr;
    String deviceId;
};