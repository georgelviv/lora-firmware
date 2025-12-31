#include "my-serial.h"

void MySerial::parseLoraMessage(String msg) {
  String command = getCommand(msg);
  String params = getParams(msg);

  this->display->showTempMsg("Lora", command);
  this->logger.info("Incoming Lora command: ", command);

  this->loraPending = millis();

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
    this->serialBridge->print("Unknown incoming Lora message:");
    this->serialBridge->println(msg);
  }
}

void MySerial::handlePing(String params) {
  this->sendAck(params, "PING");
}

void MySerial::handlePingAck(String params) {
  this->handleAck(params, "PING");
}

// Receive LORA CONFIG_SYNC
void MySerial::handleIncomingConfigSync(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  this->settingsToUpdateOnTransmit = params;

  this->prepareTransmitWithSettingsUpdate(params, "CONFIG_SYNC_ACK");
  String msg = "CONFIG_SYNC_ACK;" + params;

  this->sendLora(msg);
}

// Receive LORA CONFIG_SYNC_CHECK
void MySerial::handleConfigSyncAck(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  this->updateSettings(params);
  
  this->prepareTransmitWithPending(params, "CONFIG_SYNC_CHECK");

  String messageId = getParam(params, "ID");
  String msg = "CONFIG_SYNC_CHECK;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handleConfigSyncCheck(String params) {
  this->sendAck(params, "CONFIG_SYNC_CHECK");
}

void MySerial::handleIncomingConfigSyncCheck(String params) {
  this->handleAck(params, "CONFIG_SYNC_CHECK");
}

void MySerial::handleIncomingSend(String params) {
  this->sendAck(params, "SEND");

  String data = getParam(params, "DATA");
  String msg = "DATA;" + formatParams({"DATA", data});
  this->serialBridge->println(msg);
}

void MySerial::handleSendAck(String params) {
  this->handleAck(params, "SEND");
}

void MySerial::handleIncomingConfigResetSync(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  String defaultParams = formatParams({
    "FQ", String(DEFAULT_FREQUENCY),
    "BW", String(DEFAULT_BANDWIDTH),
    "SF", String(DEFAULT_SPREADING_FACTOR),
    "CR", String(DEFAULT_CODDING_RATE),
    "TP", String(DEFAULT_TRANSMIT_POWER),
    "IH", String(DEFAULT_IMPLICIT_HEADER),
    "HS", String(DEFAULT_HEADER_SIZE),
    "PL", String(DEFAULT_PREAMBLE_LENGTH),
    "CL", String(DEFAULT_PREAMBLE_LENGTH),
    "RT", String(DEFAULT_RETRY)
  });

  this->settingsToUpdateOnTransmit = defaultParams;

  this->prepareTransmitWithPending(params, "CONFIG_RESET_SYNC_ACK");
  String msg = "CONFIG_RESET_SYNC_ACK;" + params;

  this->sendLora(msg);
}

void MySerial::handleIncomingConfigResetSyncAck(String params) {
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  String defaultParams = formatParams({
    "FQ", String(DEFAULT_FREQUENCY),
    "BW", String(DEFAULT_BANDWIDTH),
    "SF", String(DEFAULT_SPREADING_FACTOR),
    "CR", String(DEFAULT_CODDING_RATE),
    "TP", String(DEFAULT_TRANSMIT_POWER),
    "IH", String(DEFAULT_IMPLICIT_HEADER),
    "HS", String(DEFAULT_HEADER_SIZE),
    "PL", String(DEFAULT_PREAMBLE_LENGTH),
    "CL", String(DEFAULT_CURRENT_LIMIT),
    "RT", String(DEFAULT_RETRY)
  });
  this->updateSettings(defaultParams);
  
  this->prepareTransmitWithPending(params, "CONFIG_RESET_SYNC_CHECK");

  String messageId = getParam(params, "ID");
  String msg = "CONFIG_RESET_SYNC_CHECK;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handleIncomingConfigResetSyncCheck(String params) {
  this->prepareTransmitWithPending(params, "CONFIG_RESET_SYNC_CHECK_ACK");

  String messageId = getParam(params, "ID");
  String msg = "CONFIG_RESET_SYNC_CHECK_ACK;" + formatParams({"ID", messageId});

  this->sendLora(msg);
}

void MySerial::handleIncomingConfigResetSyncCheckAck(String params) {
  this->handleAck(params, "CONFIG_RESET_SYNC_CHECK");
}

void MySerial::prepareTransmitWithSettingsUpdate(String params, String command) {
  lora->setTransmitCallback([this, params, command]() {
    if (this->settingsToUpdateOnTransmit != "") {
      this->updateSettings(this->settingsToUpdateOnTransmit);
      this->settingsToUpdateOnTransmit = "";
    }
  
    String messageId = getParam(params, "ID");

    this->logger.info(command + " message sent");

    lora->setTransmitCallback(nullptr);
  });

  lora->setTransmitChunkCallback([this, command](int currentChunk, int totalChunks) {
    this->logger.info(command + " " + currentChunk + " of " + totalChunks + " chunk sent");
  });

  this->logger.info(command + " command accepted");
  this->messageStart = millis();
  String messageId = getParam(params, "ID");

  this->attemptCommand = command;
  this->attemptParams = params;
}

void MySerial::sendAck(String params, String command) {
  String messageId = getParam(params, "ID");
  String msg = command + "_ACK;" + formatParams({"ID", messageId});

  lora->transmit(msg);
}

void MySerial::handleAck(String params, String command) {
  String messageId = getParam(params, "ID");
  if (this->pendingId == messageId) {
    this->serialBridge->println(command + "_ACK;" + this->getStatusString(&this->messageStart, messageId));
    this->pendingId = "";
  }
}

void MySerial::prepareTransmitWithPending(String params, String command) {
  lora->setTransmitCallback([this, params, command]() {  
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
  this->messageStart = millis();
  String messageId = getParam(params, "ID");
  this->pendingTimeoutMsg = command + "_NO_ACK;" + formatParams({"ID", messageId});

  this->attemptCommand = command;
  this->attemptParams = params;
}