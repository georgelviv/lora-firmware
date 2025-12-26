#include "SerialBridge.h"

void SerialBridge::println(String message) {
  Serial.println(message);

  if (this->logCallback) {
    this->logCallback(message+ "\r\n");
  }
};

void SerialBridge::print(String message) {
  Serial.print(message);

  if (this->logCallback) {
    this->logCallback(message);
  }
};

void SerialBridge::setLogCallback(LogCallback cb) {
  this->logCallback = cb;
}