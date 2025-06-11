#include "my-serial.h"

MySerial::MySerial(Lora *lora): logger("Serial", true) {
  this->lora = lora;
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
  if (command == "PING") {
    this->sendPing(params);
  } else if (command == "CONFIG") {
    this->updateSettings(params);
  } else if (command == "CONFIG_SYNC") {
    this->sendConfigSync(params);
  } else if (command == "CONFIG_GET") {
    this->printConfig();
  } else {
    Serial.print("Unknown message:");
    Serial.println(command);
  }
}

void MySerial::parseLoraMessage(String msg) {
  String command = getCommand(msg);
  String params = getParams(msg);

  this->logger.info("Incoming Lora command:", command);

  if (command == "PING") {
    this->sendPingBack(params);
  } else if (command == "PING_ACK") {
    this->printPingSuccess(params);
  } else if (command == "CONFIG_SYNC") {
    this->syncConfig(params);
  } else if (command == "CONFIG_SYNC_ACK") {
    this->updateSettingsAndCheck(params);
  } else if (command == "CONFIG_SYNC_CHECK") {
    this->sendConfigCheckAck();
  } else if (command == "CONFIG_SYNC_CHECK_ACK") {
    this->printSuccessConfigUpdate(params);
  } else {
    Serial.print("Unknown incoming Lora message:");
    Serial.println(msg);
  }
}

void MySerial::sendPing(String params) {
  this->pingStart = millis();

  String messageId = getParam(params, "ID");
  String msg = "PING;" + formatParams({"ID", messageId});
  this->pingPendingId = messageId;

  lora->transmit(msg);
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
    } else if (key == "SF") {
       this->lora->settings.updateSpreadingFactor(val.toInt());
    } else if (key == "IH") {
       this->lora->settings.updateImplicitHeader(val.toInt());
    } else if (key == "HS") {
       this->lora->settings.updateHeaderSize(val.toInt());
    } else {
      Serial.println("Uknown config to update");
      Serial.println(key);
    }
  }
};

void MySerial::sendPingBack(String params) {
  String messageId = getParam(params, "ID");
  String msg = "PING_ACK;" + formatParams({"ID", messageId});

  lora->transmit(msg);
}

void MySerial::sendConfigSync(String configMsg) {
  this->configSyncStart = millis();
  lora->transmit("CONFIG_SYNC;" + configMsg);
  
  this->isConfigPending = true;

  this->logger.info("Config sync message sent");
}

void MySerial::syncConfig(String params) {
  lora->setTransmitCallback([this, params]() {
    this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
    Serial.println(this->fallbackConfigSyncSettings.spreagingFactor);
    this->updateSettings(params);
    this->isConfigCheckPending = true;
    this->configCheckStart = millis();
  });

  lora->transmit("CONFIG_SYNC_ACK;" + params);
}

void MySerial::updateSettingsAndCheck(String params) {
  this->isConfigPending = false;
  this->fallbackConfigSyncSettings = this->lora->settings.getSettings();
  Serial.println(this->fallbackConfigSyncSettings.spreagingFactor);
  this->updateSettings(params);
  this->isConfigCheckPending = true;
  this->configCheckStart = millis();
  lora->transmit("CONFIG_SYNC_CHECK");
}

void MySerial::printPingSuccess(String params) {
  String messageId = getParam(params, "ID");
  if (this->pingPendingId == messageId) {
    Serial.println("PING_ACK;" + this->getStatusString(&this->pingStart, messageId));
    this->pingPendingId = "";
  }
}

void MySerial::sendConfigCheckAck() {
  lora->transmit("CONFIG_SYNC_CHECK_ACK");
}

void MySerial::printSuccessConfigUpdate(String params) {
  String messageId = getParam(params, "ID");
  Serial.println("CONFIG_SYNC_CHECK_ACK;" +  this->getStatusString(&this->configSyncStart, messageId));
}

String MySerial::getStatusString(unsigned long* startTime, String messageId) {
  unsigned long diff = millis() - *startTime;

  *startTime = 0;
  float rssi = lora->getRSSI();
  float snr = lora->getSNR();

  if (messageId != "") {
    return formatParams({
      "ID", messageId,
      "DELAY", String(diff),
      "RSSI", String(rssi),
      "SNR", String(snr)
    });
  } else {
    return formatParams({
      "DELAY", String(diff),
      "RSSI", String(rssi),
      "SNR", String(snr)
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
    "HS", String(settings.headerSize)
  }));
}

void MySerial::checkPending() {
  if (this->pingPendingId != "") {
    unsigned long passedPingTime = millis() - this->pingStart;
    if (passedPingTime > ACK_TIMEOUT) {
      String msg = "PING_NO_ACK;" + formatParams({"ID", this->pingPendingId});
      Serial.println(msg);
      this->pingPendingId = "";
    }
  }

  if (this->isConfigPending) {
    unsigned long passedConfigSyncTime = millis() - this->configSyncStart;
    if (passedConfigSyncTime > ACK_TIMEOUT) {
      String msg = "CONFIG_SYNC_NO_ACK";
      Serial.println(msg);
      this->configSyncStart = 0;
      this->isConfigPending = false;
    }
  }

  if (this->isConfigCheckPending) {
    unsigned long passedConfigCheckTime = millis() - this->configCheckStart;
    if (passedConfigCheckTime > ACK_TIMEOUT) {
      String msg = "CONFIG_SYNC_CHECK_NO_ACK";
      Serial.println(msg);
      this->configCheckStart = 0;
      this->isConfigCheckPending = false;
      this->lora->settings.updateSettings(this->fallbackConfigSyncSettings);
    }
  }
}