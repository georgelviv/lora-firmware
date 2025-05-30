#include "my-serial.h"

MySerial::MySerial(Lora *lora) {
  this->lora = lora;
}

void MySerial::parseSerial() {
  if (Serial.available()) {
    String command;
    String params;
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    int commaIndex = msg.indexOf(';');

    if (commaIndex != -1) {
      command = msg.substring(0, commaIndex);
      params = msg.substring(commaIndex + 1);
    } else {
      command = msg;
    }

    handleSerialMessage(command, params);
  }
}

void MySerial::handleSerialMessage(String command, String params) {
  if (command == "PING") {
    this->sendPing();
  } else if (command == "CONFIG") {
    this->updateSettings(params);
  } else {
    Serial.println("Unknown message");
  }
}

void MySerial::sendPing() {
  lora->transmit("PING");
}

void MySerial::parseLoraMessage(String msg) {
  Serial.print("Incoming lora message: ");
  Serial.println(msg);
}

void MySerial::updateSettings(String str) {
  String configs[5][2];
  int configsCount = parseParams(str, configs);

  for (int i = 0; i < configsCount; i++) {
    String key = configs[i][0];
    String val = configs[i][1];
    if (key == "FQ") {
      this->lora->settings.updateFrequency(val.toFloat());
    } else if (key == "BW") {
       this->lora->settings.updateFrequency(val.toFloat());
    } else {
      Serial.println("Uknown config to update");
      Serial.println(key);
    }
  }
};

int MySerial::parseParams(String str, String paramsPairs[5][2]) {
  String params[5];
  int paramIndex = 0;
  int startIndex = 0;
  int commaIndex;
  
  while ((commaIndex = str.indexOf(",", startIndex)) != -1) {
    params[paramIndex] = str.substring(0, commaIndex);
    startIndex = commaIndex + 1;
    paramIndex++;
  }

  params[paramIndex] = str.substring(startIndex);

  for (int i = 0; i < paramIndex + 1; i++) {
    String param = params[i];
    int eqIndex = param.indexOf('=');
    String key = param.substring(0, eqIndex);
    String val = param.substring(eqIndex + 1);

    paramsPairs[i][0] = key;
    paramsPairs[i][1] = val;
  }

  return paramIndex + 1;
}