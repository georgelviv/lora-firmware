#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger {
  public:
    Logger(String module, bool isVerbose);   
};

#endif