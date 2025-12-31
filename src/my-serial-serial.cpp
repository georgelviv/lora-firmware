#include "my-serial.h"

void MySerial::parseSerial() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    this->runCommand(msg);
  }
}

void MySerial::handleSerialMessage(String command, String params) {
  this->logger.info("Incoming Serial command: ", command);

  this->display->showTempMsg("Serial", command);
  this->attempt = 0;

  if (command == "PING") {
    this->sendPing(params);
  } else if (command == "CONFIG") {
    this->updateSettings(params);
  } else if (command == "CONFIG_SYNC") {
    this->sendConfigSync(params);
  } else if (command == "CONFIG_GET") {
    this->printConfig();
  } else if (command == "SEND") {
    this->sendData(params);
  } else if (command == "CONFIG_RESET") {
    this->handleConfigReset();
  } else if (command == "CONFIG_RESET_SYNC") {
    this->handleConfigResetSync(params);
  }else {
    this->serialBridge->print("Unknown message:");
    this->serialBridge->println(command);
  }
}

void MySerial::sendPing(String params) {
  this->prepareTransmitWithPending(params, "PING");

  String messageId = getParam(params, "ID");
  String msg = "PING;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

// Receive SERIAL CONFIG_SYNC
void MySerial::sendConfigSync(String params) {
  this->prepareTransmitWithPending(params, "CONFIG_SYNC");
  String msg = "CONFIG_SYNC;" + params;

  this->sendLora(msg);
}

void MySerial::printConfig() {
  LoraSettings settings = lora->settings.getSettings();

  this->serialBridge->print("CONFIG_GET;");
  this->serialBridge->println(formatParams({
    "FQ", String(settings.frequency),
    "BW", String(settings.bandwidth),
    "SF", String(settings.spreagingFactor),
    "CR", String(settings.codingRate),
    "TP", String(settings.transmitPower),
    "IH", String(settings.implicitHeader),
    "HS", String(settings.headerSize),
    "PL", String(settings.preambleLength),
    "CL", String(settings.currentLimit),
    "RT", String(settings.retry),
  }));
}

void MySerial::sendData(String params) {
  this->prepareTransmitWithPending(params, "SEND");

  String messageId = getParam(params, "ID");
  String data = getParam(params, "DATA");
  String msg = "SEND;" + formatParams({"ID", messageId, "DATA", data});

  this->sendLora(msg);
}

void MySerial::handleConfigReset() {
  this->lora->settings.setDefaultSettings();
}

void MySerial::handleConfigResetSync(String params) {
  this->prepareTransmitWithPending(params, "CONFIG_RESET_SYNC");
  String msg = "CONFIG_RESET_SYNC;" + params;
  this->sendLora(msg);
}