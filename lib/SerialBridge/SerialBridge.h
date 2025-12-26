#pragma once

#include <Arduino.h>

class SerialBridge {
  public:
    using LogCallback = void (*)(const String);

    void println(String message);
    void print(String message);
    void setLogCallback(LogCallback cb);

  private:
    LogCallback logCallback = nullptr;
};