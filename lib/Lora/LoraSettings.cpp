#include "LoraSettings.h"

LoraSettingsManager::LoraSettingsManager(SX1276 *radio, Logger *logger) {
  this->radio = radio;
  this->logger = logger;
};

void LoraSettingsManager::updateSettings(LoraSettings settings) {
  this->updateFrequency(settings.frequency, false);
  this->updateBandwidth(settings.bandwidth, false);
  this->updateTransmitPower(settings.transmitPower, false);
  this->updateSpreadingFactor(settings.spreagingFactor, false);

  if (this->radio->setCodingRate(settings.codingRate) == RADIOLIB_ERR_INVALID_CODING_RATE) {
    Serial.println(F("Selected coding rate is invalid for this module!"));
    Serial.println(settings.codingRate);
    settings.codingRate = DEFAULT_CODDING_RATE;
    this->updateSettings(settings);
    return;
  }

  if (this->radio->setSyncWord(settings.syncWord) != RADIOLIB_ERR_NONE) {
    Serial.println(F("Unable to set sync word!"));
    while (true);
  }

  this->settings = settings;
  if (this->settingsUpdatedCallback) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::setSettingsUpdatedCallback(void (* callback)(LoraSettings settings)) {
  this->settingsUpdatedCallback = callback;
}

void LoraSettingsManager::updateFrequency(float freq, bool callCb) {
  if (this->radio->setFrequency(freq) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    this->logger->log("Selected FQ is invalid for this module:", freq);
    if (freq != DEFAULT_BANDWIDTH) {
      this->logger->log("Set default FQ:", DEFAULT_FREQUENCY);
      this->updateFrequency(DEFAULT_BANDWIDTH);
    } else {
      while (true) { delay(10); }
    }
  } else {
    this->logger->info("FQ updated to:", freq);
  }

  this->settings.frequency = freq;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateBandwidth(float bandwidth, bool callCb) {
  if (this->radio->setBandwidth(bandwidth) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
    this->logger->log("Selected BW is invalid for this module:", bandwidth);
    if (bandwidth != DEFAULT_BANDWIDTH) {
      this->logger->log("Set default BW:", DEFAULT_BANDWIDTH);
      this->updateBandwidth(DEFAULT_BANDWIDTH);
    } else {
      while (true) { delay(10); }
    }
  } else {
    this->logger->info("BW updated to:", bandwidth);
  }

  this->settings.bandwidth = bandwidth;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateSpreadingFactor(int spreagingFactor, bool callCb) {
  if (this->radio->setSpreadingFactor(spreagingFactor) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
    this->logger->log("Selected SF is invalid for this module:", spreagingFactor);
    if (spreagingFactor != DEFAULT_SPREADING_FACTOR) {
      this->logger->log("Set default SF:", DEFAULT_SPREADING_FACTOR);
      this->updateSpreadingFactor(DEFAULT_SPREADING_FACTOR);
    } else {
      while (true) { delay(10); }
    }
  } else {
    this->logger->info("SF updated to:", spreagingFactor);
  }

  this->settings.spreagingFactor = spreagingFactor;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateTransmitPower(int transmitPower, bool callCb) {
  if (this->radio->setOutputPower(transmitPower) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    this->logger->log("Selected TP is invalid for this module:", transmitPower);
    if (transmitPower != DEFAULT_TRANSMIT_POWER) {
      this->logger->log("Set default TP:", DEFAULT_TRANSMIT_POWER);
      this->updateTransmitPower(DEFAULT_TRANSMIT_POWER);
    } else {
      while (true) { delay(10); }
    }
  } else {
    this->logger->info("TP updated to:", transmitPower);
  }

  this->settings.transmitPower = transmitPower;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

LoraSettings LoraSettingsManager::getSettings() {
  return this->settings;
}