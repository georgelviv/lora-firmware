#include "LoraSettings.h"

LoraSettingsManager::LoraSettingsManager(SX1276 *radio) {
  this->radio = radio;
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
    Serial.println(F("Selected frequency is invalid for this module:"));
    Serial.println(freq);
    if (freq != DEFAULT_BANDWIDTH) {
      Serial.print("Set default frequency:");
      Serial.println(DEFAULT_FREQUENCY);
      this->updateFrequency(DEFAULT_BANDWIDTH);
    } else {
      while (true) { delay(10); }
    }
  }

  this->settings.frequency = freq;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateBandwidth(float bandwidth, bool callCb) {
  if (this->radio->setBandwidth(bandwidth) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
    Serial.print(F("Selected bandwidth is invalid for this module:"));
    Serial.println(bandwidth);
    if (bandwidth != DEFAULT_BANDWIDTH) {
      Serial.print("Set default bandwidth:");
      Serial.println(DEFAULT_BANDWIDTH);
      this->updateBandwidth(DEFAULT_BANDWIDTH);
    } else {
      while (true) { delay(10); }
    }
  }

  this->settings.bandwidth = bandwidth;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateSpreadingFactor(int spreagingFactor, bool callCb) {
  if (this->radio->setSpreadingFactor(spreagingFactor) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
    Serial.print(F("Selected spreagingFactor is invalid for this module:"));
    Serial.println(spreagingFactor);
    if (spreagingFactor != DEFAULT_SPREADING_FACTOR) {
      Serial.print("Set default spreagingFactor:");
      Serial.println(DEFAULT_SPREADING_FACTOR);
      this->updateSpreadingFactor(DEFAULT_SPREADING_FACTOR);
    } else {
      while (true) { delay(10); }
    }
  } else {
    Serial.print("[Lora] Sf updated to:");
    Serial.println(spreagingFactor);
  }

  this->settings.spreagingFactor = spreagingFactor;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateTransmitPower(int transmitPower, bool callCb) {
  if (this->radio->setOutputPower(transmitPower) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
    Serial.print(F("Selected transmit power is invalid for this module:"));
    Serial.println(transmitPower);
    if (transmitPower != DEFAULT_TRANSMIT_POWER) {
      Serial.print("Set default transmit power:");
      Serial.println(DEFAULT_TRANSMIT_POWER);
      this->updateTransmitPower(DEFAULT_TRANSMIT_POWER);
    } else {
      while (true) { delay(10); }
    }
  }

  this->settings.transmitPower = transmitPower;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

LoraSettings LoraSettingsManager::getSettings() {
  return this->settings;
}