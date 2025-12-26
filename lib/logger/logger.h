#pragma once

#include <Arduino.h>

class Logger {
  public:
    Logger(String context, bool isVerbose);

    void info(String msg);
    void info(String msg, int msg2);
    void info(String msg, float msg2);
    void info(String msg, String msg2);
    void log(String msg);
    void log(String msg, int msg2);
    void log(String msg, float msg2);
    void log(String msg, String msg2);

  private:
    String context;
    bool isVerbose;
};