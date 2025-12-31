#include "my-serial.h"

MySerial::MySerial(
  Lora *lora, Display *display, SerialBridge *serialBridge
): logger("Serial", true) {
  this->lora = lora;
  this->display = display;
  this->serialBridge = serialBridge;
}

void MySerial::runCommand(String msg) {
  String command = getCommand(msg);
  String params = getParams(msg);

  handleSerialMessage(command, params);
}


void MySerial::sendLora(String msg) {
  auto result = lora->transmit(msg);
  this->payloadSize = result.first;
  this->chunksCount = result.second;
}

void MySerial::updateSettings(String str) {
  String configs[12][2];
  int configsCount = parseParams(str, configs);

  LoraSettings settings = lora->settings.getSettings();

  for (int i = 0; i < configsCount; i++) {
    String key = configs[i][0];
    String val = configs[i][1];
    if (key == "FQ") {
      settings.frequency = val.toFloat();
    } else if (key == "BW") {
      settings.bandwidth = val.toFloat();
    } else if (key == "SF") {
      settings.spreagingFactor = val.toInt();
    } else if (key == "IH") {
      settings.implicitHeader = val.toInt();
    } else if (key == "HS") {
      settings.headerSize = val.toInt();
    } else if (key == "CR") {
      settings.codingRate = val.toInt();
    } else if (key == "TP") {
      settings.transmitPower = val.toInt();
    } else if (key == "SW") {
      settings.syncWord = val.toInt();
    } else if (key == "PL") {
      settings.preambleLength = val.toInt();
    } else if (key == "CL") {
      settings.currentLimit = val.toInt();
    } else if (key == "RT") {
      settings.retry = val.toInt();
    } else if (key == "ID") {
      continue;
    } else {
      this->serialBridge->print("Unknown config to update");
      this->serialBridge->println(key);
    }
  }

  this->lora->settings.updateSettings(settings);

  this->lora->settings.runSettingsCallback();  
};

String MySerial::getStatusString(unsigned long* startTime, String messageId) {
  unsigned long diff = millis() - *startTime;

  *startTime = 0;
  float rssi = lora->getRSSI();
  float snr = lora->getSNR();
  int rtoa = lora->getRTOA();
  int toa = lora->getTOA(this->payloadSize);
  LoraSettings settings = lora->settings.getSettings();
  float etx = lora->getETX(rtoa, settings.transmitPower, settings.currentLimit);
  int bps = int(((float)this->payloadSize) / (float)toa * 1000.0);
  int chunksCount = this->chunksCount;

  if (messageId != "") {
    return formatParams({
      "ID", messageId,
      "DELAY", String(diff),
      "RSSI", String(rssi),
      "SNR", String(snr),
      "TOA", String(toa),
      "RTOA", String(rtoa),
      "BPS", String(bps),
      "CHC", String(chunksCount),
      "ATT", String(this->attempt + 1),
      "ETX", String(etx)
    });
  } else {
    return formatParams({
      "DELAY", String(diff),
      "RSSI", String(rssi),
      "SNR", String(snr),
      "TOA", String(toa),
      "RTOA", String(rtoa),
      "BPS", String(bps),
      "CHC", String(chunksCount),
      "ATT", String(this->attempt + 1),
      "ETX", String(etx)
    });
  }
}

void MySerial::checkPending() {
  if (this->pendingId != "") {
    unsigned long passedTime = millis() - this->pendingTimeoutStart;
    if (passedTime > ACK_TIMEOUT) {
      if (this->attemptCommand == "PING") {
        int retry = this->lora->settings.getSettings().retry;
        if (retry > this->attempt) {
          this->attempt += 1;
          this->logger.info(this->attemptCommand + " attempt:", this->attempt + 1);
          this->sendPing(this->attemptParams);
        } else {
          this->serialBridge->println(this->pendingTimeoutMsg);
        }
      } else if (this->attemptCommand == "CONFIG_SYNC_ACK") {
        this->lora->settings.updateSettings(this->fallbackConfigSyncSettings);
        this->serialBridge->println(this->pendingTimeoutMsg);
      } else {
        this->serialBridge->println(this->pendingTimeoutMsg);
      }
      this->pendingId = "";
    }
  }

  if (this->loraPending != 0) {
    unsigned long passedTime = millis() - this->loraPending;
    if (passedTime > 30 * 60 * 1000) {
      this->loraPending = 0;
      this->serialBridge->println("MODULE AUTORESET");
      this->lora->settings.setDefaultSettings();
    }
  } 
}

void MySerial::handleChunkReceived(int chunk, int totalChunks) {
  String msg = "Received " + String(chunk) + " of " + String(totalChunks) + " chunks";
  this->logger.info(msg);
}