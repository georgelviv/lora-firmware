#include "lora-settings.h"

LoraSettingsManager::LoraSettingsManager(SX1276 *radio) {
  this->radio = radio;
};

void LoraSettingsManager::updateSettings(LoraSettings settings) {
  if (this->radio->setFrequency(settings.frequency) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    Serial.println(F("Selected frequency is invalid for this module!"));
    while (true) { delay(10); }
  }
  Serial.println(F("Frequency was updated"));

  if (this->radio->setBandwidth(settings.bandwidth) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
    Serial.println(F("Selected bandwidth is invalid for this module!"));
    while (true);
  }

  Serial.println(F("Bandwidth was updated"));

  if (this->radio->setSpreadingFactor(settings.spreagingFactor) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
    Serial.println(F("Selected spreading factor is invalid for this module!"));
    Serial.println(settings.spreagingFactor);
    settings.spreagingFactor = DEFAULT_SPREADING_FACTOR;
    this->updateSettings(settings);
    return;
  }

  Serial.println(F("Spreading factor was updated"));

  if (this->radio->setCodingRate(settings.codingRate) == RADIOLIB_ERR_INVALID_CODING_RATE) {
    Serial.println(F("Selected coding rate is invalid for this module!"));
    Serial.println(settings.codingRate);
    settings.codingRate = DEFAULT_CODDING_RATE;
    this->updateSettings(settings);
    return;
  }

  Serial.println(F("Coding rate was updated"));

  if (this->radio->setSyncWord(settings.syncWord) != RADIOLIB_ERR_NONE) {
    Serial.println(F("Unable to set sync word!"));
    while (true);
  }

  Serial.println(F("Sync word was updated"));

  this->settings = settings;
  if (this->settingsUpdatedCallback) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::setSettingsUpdatedCallback(void (* callback)(LoraSettings settings)) {
  this->settingsUpdatedCallback = callback;
}