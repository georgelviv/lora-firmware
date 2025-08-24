#include "my-serial.h"

MySerial::MySerial(Lora *lora, Display *display): logger("Serial", true) {
  this->lora = lora;
  this->display = display;
}

void MySerial::parseSerial() {
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    String command = getCommand(msg);
    String params = getParams(msg);

    handleSerialMessage(command, params);
  }
}

void MySerial::handleSerialMessage(String command, String params) {
  this->logger.info("Incoming Serial command: ", command);

  this->display->showTempMsg(command);

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
    Serial.print("Unknown message:");
    Serial.println(command);
  }
}

void MySerial::parseLoraMessage(String msg) {
  String command = getCommand(msg);
  String params = getParams(msg);

  this->logger.info("Incoming Lora command: ", command);

  if (command == "PING") {
    this->handlePing(params);
  } else if (command == "PING_ACK") {
    this->handlePingAck(params);
  } else if (command == "CONFIG_SYNC") {
    this->handleIncomingConfigSync(params);
  } else if (command == "CONFIG_SYNC_ACK") {
    this->handleConfigSyncAck(params);
  } else if (command == "CONFIG_SYNC_CHECK") {
    this->handleConfigSyncCheck(params);
  } else if (command == "CONFIG_SYNC_CHECK_ACK") {
    this->handleIncomingConfigSyncCheck(params);
  } else if (command == "SEND") {
    this->handleIncomingSend(params);
  } else if (command == "SEND_ACK") {
    this->handleSendAck(params);
  } else if (command == "CONFIG_RESET_SYNC") {
    this->handleIncomingConfigResetSync(params);
  } else if (command == "CONFIG_RESET_SYNC_ACK") {
    this->handleIncomingConfigResetSyncAck(params);
  } else if (command == "CONFIG_RESET_SYNC_CHECK") {
    this->handleIncomingConfigResetSyncCheck(params);
  } else if (command == "CONFIG_RESET_SYNC_CHECK_ACK") {
    this->handleIncomingConfigResetSyncCheckAck(params);
  } else {
    Serial.print("Unknown incoming Lora message:");
    Serial.println(msg);
  }
}

void MySerial::sendLora(String msg) {
  auto result = lora->transmit(msg);
  this->payloadSize = result.first;
  this->chunksCount = result.second;
}

void MySerial::prepareTransmit(String params, String command) {
  lora->setTransmitCallback([this, params, command]() {
    if (this->settingsToUpdateOnTransmit) {
      this->updateSettings(this->settingsToUpdateOnTransmit);
      this->settingsToUpdateOnTransmit = "";
    }
  
    String messageId = getParam(params, "ID");
    this->pendingId = messageId;
    this->pendingTimeoutStart = millis();

    this->logger.info(command + " message sent");

    lora->setTransmitCallback(nullptr);
  });

  lora->setTransmitChunkCallback([this, command](int currentChunk, int totalChunks) {
    this->logger.info(command + " " + currentChunk + " of " + totalChunks + " chunk sent");
  });

  this->logger.info(command + " command accepted");
  String messageId = getParam(params, "ID");
  this->pendingTimeoutMsg = command + "_NO_ACK;" + formatParams({"ID", messageId});
  this->pendingStart = millis();
}

void MySerial::handleAck(String params, String command) {
  String messageId = getParam(params, "ID");
  if (this->pendingId == messageId) {
    Serial.println(command + "_ACK;" + this->getStatusString(&this->pendingStart, messageId));
    this->pendingId = "";
  }
}

void MySerial::sendAck(String params, String command) {
  String messageId = getParam(params, "ID");
  String msg = command + "_ACK;" + formatParams({"ID", messageId});

  lora->transmit(msg);
}

void MySerial::sendPing(String params) {
  this->prepareTransmit(params, "PING");

  String messageId = getParam(params, "ID");
  String msg = "PING;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handlePing(String params) {
  this->sendAck(params, "PING");
}

void MySerial::handlePingAck(String params) {
  this->handleAck(params, "PING");
}

void MySerial::sendData(String params) {
  this->prepareTransmit(params, "SEND");

  String messageId = getParam(params, "ID");
  String data = getParam(params, "DATA");
  String msg = "SEND;" + formatParams({"ID", messageId, "DATA", data});

  this->sendLora(msg);
}

void MySerial::handleIncomingSend(String params) {
  this->sendAck(params, "SEND");

  String data = getParam(params, "DATA");
  String msg = "DATA;" + formatParams({"DATA", data});
  Serial.println(msg);
}

void MySerial::handleSendAck(String params) {
  this->handleAck(params, "SEND");
}

void MySerial::sendConfigSync(String params) {
  this->prepareTransmit(params, "CONFIG_SYNC");
  String msg = "CONFIG_SYNC;" + params;
  this->sendLora(msg);
}

void MySerial::handleIncomingConfigSync(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  this->settingsToUpdateOnTransmit = params;

  this->prepareTransmit(params, "CONFIG_SYNC_ACK");
  String msg = "CONFIG_SYNC_ACK;" + params;

  this->sendLora(msg);
}

void MySerial::handleConfigSyncAck(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  this->updateSettings(params);

  
  this->prepareTransmit(params, "CONFIG_SYNC_CHECK");

  String messageId = getParam(params, "ID");
  String msg = "CONFIG_SYNC_CHECK;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handleConfigSyncCheck(String params) {
  this->prepareTransmit(params, "CONFIG_SYNC_CHECK_ACK");

  String messageId = getParam(params, "ID");
  String msg = "CONFIG_SYNC_CHECK_ACK;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handleIncomingConfigSyncCheck(String params) {
  this->handleAck(params, "CONFIG_SYNC_CHECK");
}

void MySerial::updateSettings(String str) {
  String configs[8][2];
  int configsCount = parseParams(str, configs);

  for (int i = 0; i < configsCount; i++) {
    String key = configs[i][0];
    String val = configs[i][1];
    if (key == "FQ") {
      this->lora->settings.updateFrequency(val.toFloat());
    } else if (key == "BW") {
       this->lora->settings.updateBandwidth(val.toFloat());
    } else if (key == "SF") {
       this->lora->settings.updateSpreadingFactor(val.toInt());
    } else if (key == "IH") {
       this->lora->settings.updateImplicitHeader(val.toInt());
    } else if (key == "HS") {
       this->lora->settings.updateHeaderSize(val.toInt());
    } else if (key == "CR") {
       this->lora->settings.updateCodingRate(val.toInt());
    } else if (key == "TP") {
       this->lora->settings.updateTransmitPower(val.toInt());
    } else if (key == "SW") {
       this->lora->settings.updateSyncWord(val.toInt());
    } else if (key == "PL") {
       this->lora->settings.updatePreambleLength(val.toInt());
    } else if (key == "CL") {
       this->lora->settings.updateCurrentLimit(val.toInt());
    } else if (key == "ID") {
      continue;
    } else {
      Serial.println("Unknown config to update");
      Serial.println(key);
    }
  }
};

String MySerial::getStatusString(unsigned long* startTime, String messageId) {
  unsigned long diff = millis() - *startTime;

  *startTime = 0;
  float rssi = lora->getRSSI();
  float snr = lora->getSNR();
  int toa = lora->getTOA(this->payloadSize);
  int bps = int(((float)this->payloadSize) / (float)toa * 1000.0);
  int chunksCount = this->chunksCount;

  if (messageId != "") {
    return formatParams({
      "ID", messageId,
      "DELAY", String(diff),
      "RSSI", String(rssi),
      "SNR", String(snr),
      "TOA", String(toa),
      "BPS", String(bps),
      "CHC", String(chunksCount)
    });
  } else {
    return formatParams({
      "DELAY", String(diff),
      "RSSI", String(rssi),
      "SNR", String(snr),
      "TOA", String(toa),
      "BPS", String(bps),
      "CHC", String(chunksCount)
    });
  }
}

void MySerial::printConfig() {
  LoraSettings settings = lora->settings.getSettings();

  Serial.print("CONFIG_GET;");
  Serial.println(formatParams({
    "FW", String(settings.frequency),
    "BW", String(settings.bandwidth),
    "SF", String(settings.spreagingFactor),
    "CR", String(settings.codingRate),
    "TP", String(settings.transmitPower),
    "IH", String(settings.implicitHeader),
    "HS", String(settings.headerSize),
    "PL", String(settings.preambleLength),
    "CL", String(settings.currentLimit)
  }));
}

void MySerial::checkPending() {
  if (this->pendingId != "") {
    unsigned long passedTime = millis() - this->pendingTimeoutStart;
    if (passedTime > ACK_TIMEOUT) {
      Serial.println(this->pendingTimeoutMsg);
      this->pendingId = "";
    }
  }
}

void MySerial::handleChunkReceived(int chunk, int totalChunks) {
  String msg = "Received " + String(chunk) + " of " + String(totalChunks) + " chunks";
  this->logger.info(msg);
}

void MySerial::handleConfigReset() {
  this->lora->settings.setDefaultSettings();
}

void MySerial::handleConfigResetSync(String params) {
  this->prepareTransmit(params, "CONFIG_RESET_SYNC");
  String msg = "CONFIG_RESET_SYNC;" + params;
  this->sendLora(msg);
}

void MySerial::handleIncomingConfigResetSync(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  String defaultParams = formatParams({
    "FW", String(DEFAULT_FREQUENCY),
    "BW", String(DEFAULT_BANDWIDTH),
    "SF", String(DEFAULT_SPREADING_FACTOR),
    "CR", String(DEFAULT_CODDING_RATE),
    "TP", String(DEFAULT_TRANSMIT_POWER),
    "IH", String(DEFAULT_IMPLICIT_HEADER),
    "HS", String(DEFAULT_HEADER_SIZE),
    "PL", String(DEFAULT_PREAMBLE_LENGTH),
    "CL", String(DEFAULT_PREAMBLE_LENGTH)
  });

  this->settingsToUpdateOnTransmit = defaultParams;

  this->prepareTransmit(params, "CONFIG_RESET_SYNC_ACK");
  String msg = "CONFIG_RESET_SYNC_ACK;" + params;

  this->sendLora(msg);
}

void MySerial::handleIncomingConfigResetSyncAck(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  String defaultParams = formatParams({
    "FW", String(DEFAULT_FREQUENCY),
    "BW", String(DEFAULT_BANDWIDTH),
    "SF", String(DEFAULT_SPREADING_FACTOR),
    "CR", String(DEFAULT_CODDING_RATE),
    "TP", String(DEFAULT_TRANSMIT_POWER),
    "IH", String(DEFAULT_IMPLICIT_HEADER),
    "HS", String(DEFAULT_HEADER_SIZE),
    "PL", String(DEFAULT_PREAMBLE_LENGTH),
    "CL", String(DEFAULT_CURRENT_LIMIT)
  });
  this->updateSettings(defaultParams);
  
  this->prepareTransmit(params, "CONFIG_RESET_SYNC_CHECK");

  String messageId = getParam(params, "ID");
  String msg = "CONFIG_RESET_SYNC_CHECK;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handleIncomingConfigResetSyncCheck(String params) {
  this->prepareTransmit(params, "CONFIG_RESET_SYNC_CHECK_ACK");

  String messageId = getParam(params, "ID");
  String msg = "CONFIG_RESET_SYNC_CHECK_ACK;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handleIncomingConfigResetSyncCheckAck(String params) {
  this->handleAck(params, "CONFIG_RESET_SYNC_CHECK");
}