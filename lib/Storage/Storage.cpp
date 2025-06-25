#include "Storage.h"

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
  settings.implicitHeader = DEFAULT_IMPLICIT_HEADER;
  settings.headerSize = DEFAULT_HEADER_SIZE;
  settings.preambleLength = DEFAULT_PREAMBLE_LENGTH;

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

  bool implicitHeaderExists = this->preferences.isKey("implicitHeader");
  if (implicitHeaderExists) {
    settings.implicitHeader = this->preferences.getInt("implicitHeader");
  }

  bool headerSizeExists = this->preferences.isKey("headerSize");
  if (headerSizeExists) {
    settings.headerSize = this->preferences.getInt("headerSize");
  }

  bool preambleLengthExists = this->preferences.isKey("preambleLength");
  if (preambleLengthExists) {
    settings.preambleLength = this->preferences.getInt("preambleLength");
  }

  return settings;
}

void Storage::saveLoraSettings(LoraSettings settings) {
  this->preferences.putFloat("frequency", settings.frequency);
  this->preferences.putFloat("bandwidth", settings.bandwidth);
  this->preferences.putInt("spreagingFactor", settings.spreagingFactor);
  this->preferences.putInt("codingRate", settings.codingRate);
  this->preferences.putInt("syncWord", settings.syncWord);
  this->preferences.putInt("transmitPower", settings.transmitPower);
  this->preferences.putInt("implicitHeader", settings.implicitHeader);
  this->preferences.putInt("headerSize", settings.headerSize);
  this->preferences.putInt("preambleLength", settings.preambleLength);
}