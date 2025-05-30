#include "my-serial.h"

MySerial::MySerial(Lora *lora) {
  this->lora = lora;
}

void MySerial::parseSerial() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    handleSerialMessage(msg);
  }
}

void MySerial::handleSerialMessage(String type) {
  if (type == "PING") {
    Serial.println("handle ping");
    sendPing();
  } else {
    Serial.println("Unknown message");
  }
}

void MySerial::sendPing() {
  lora->transmit("PING");
}