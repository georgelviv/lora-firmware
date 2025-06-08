#include "Logger.h"

Logger::Logger(String context, bool isVerbose) {
  this->context = context;
  this->isVerbose = isVerbose;
}

void Logger::info(String msg) {
  if (this->isVerbose) {
    this->log(msg);
  }
}

void Logger::info(String msg, int msg2) {
  if (this->isVerbose) {
    this->log(msg, msg2);
  }
}

void Logger::info(String msg, float msg2) {
  if (this->isVerbose) {
    this->log(msg, msg2);
  }
}

void Logger::log(String msg) {
  Serial.print("[");
  Serial.print(this->context);
  Serial.print("] ");
  Serial.println(msg);
}

void Logger::log(String msg, int msg2) {
  Serial.print("[");
  Serial.print(this->context);
  Serial.print("] ");
  Serial.print(msg);
  Serial.println(msg2);
}

void Logger::log(String msg, float msg2) {
  Serial.print("[");
  Serial.print(this->context);
  Serial.print("] ");
  Serial.print(msg);
  Serial.println(msg2);
}