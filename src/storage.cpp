#include "storage.h"

Storage::Storage() {
  Preferences preferences;
  this->preferences = preferences;
}

void Storage::init() {
  this->preferences.begin("lora-settings", false);
}

LoraSettings Storage::getLoraSettings() {
  LoraSettings settings;

  settings.frequency = DEFAULT_FREQUENCY;
  settings.bandwidth = DEFAULT_BANDWIDTH;
  settings.spreagingFactor = DEFAULT_SPREADING_FACTOR;
  settings.codingRate = DEFAULT_CODDING_RATE;
  settings.syncWord = DEFAULT_SYNC_WORD;
  settings.transmitPower = DEFAULT_TRANSMIT_POWER;

  bool frequencyExists = this->preferences.isKey("frequency");
  if (frequencyExists) {
    settings.frequency = this->preferences.getFloat("frequency");
  }

  bool bandwidthExists = this->preferences.isKey("bandwidth");
  if (bandwidthExists) {
    settings.bandwidth = this->preferences.getFloat("bandwidth");
  }

  bool spreagingFactorExists = this->preferences.isKey("spreagingFactor");
  if (spreagingFactorExists) {
    settings.spreagingFactor = this->preferences.getInt("spreagingFactor");
  }

  bool codingRateExists = this->preferences.isKey("codingRate");
  if (codingRateExists) {
    settings.codingRate = this->preferences.getInt("codingRate");
  }

  bool syncWordExists = this->preferences.isKey("syncWord");
  if (syncWordExists) {
    settings.syncWord = this->preferences.getInt("syncWord");
  }

  bool transmitPowerExists = this->preferences.isKey("transmitPower");
  if (transmitPowerExists) {
    settings.transmitPower = this->preferences.getInt("transmitPower");
  }

  return settings;
}

void Storage::saveLoraSettings(LoraSettings settings) {
  this->preferences.putFloat("frequency", settings.frequency);
  this->preferences.putFloat("bandwidth", settings.bandwidth);
  this->preferences.putInt("spreagingFactor", settings.spreagingFactor);
  this->preferences.putInt("codingRate", settings.codingRate);
  this->preferences.putInt("syncWord", settings.syncWord);
}