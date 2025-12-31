#include "LoraSettings.h"

LoraSettingsManager::LoraSettingsManager(SX1276 *radio, Logger *logger) {
  this->radio = radio;
  this->logger = logger;
};

void LoraSettingsManager::updateSettings(LoraSettings settings) {
  this->radio->standby();

  this->updateFrequency(settings.frequency, false);
  this->updateBandwidth(settings.bandwidth, false);
  this->updateTransmitPower(settings.transmitPower, false);
  this->updateSpreadingFactor(settings.spreagingFactor, false);
  this->updateImplicitHeader(settings.implicitHeader, false);
  this->updateHeaderSize(settings.headerSize, false);
  this->updateCodingRate(settings.codingRate, false);
  this->updateSyncWord(settings.syncWord, false);
  this->updatePreambleLength(settings.preambleLength, false);
  this->updateCurrentLimit(settings.currentLimit, false);
  this->updateRetry(settings.retry, false);

  if (this->settingsUpdatedCallback) {
    this->settingsUpdatedCallback(this->settings);
  }
}

void LoraSettingsManager::setSettingsUpdatedCallback(void (* callback)(LoraSettings settings)) {
  this->settingsUpdatedCallback = callback;
}

void LoraSettingsManager::updateFrequency(float freq, bool callCb) {
  if (this->radio->setFrequency(freq) == RADIOLIB_ERR_INVALID_FREQUENCY) {
    this->logger->log("Selected FQ is invalid for this module:", freq);
    if (freq != DEFAULT_FREQUENCY) {
      this->logger->log("Set default FQ:", DEFAULT_FREQUENCY);
      this->updateFrequency(DEFAULT_FREQUENCY, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("FQ updated to:", freq);
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
      this->updateBandwidth(DEFAULT_BANDWIDTH, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("BW updated to:", bandwidth);
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
      this->updateSpreadingFactor(DEFAULT_SPREADING_FACTOR, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("SF updated to:", spreagingFactor);
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
      this->updateTransmitPower(DEFAULT_TRANSMIT_POWER, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("TP updated to:", transmitPower);
  this->settings.transmitPower = transmitPower;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateImplicitHeader(int isImplicitHeader, bool callCb) {
  if (isImplicitHeader) {
    this->radio->implicitHeader(this->settings.headerSize);
  } else {
    this->radio->explicitHeader();
  }

  this->logger->info("IH updated to:", isImplicitHeader);
  this->settings.implicitHeader = isImplicitHeader;

  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateHeaderSize(int headerSize, bool callCb) {
  if (this->settings.implicitHeader) {
    this->radio->implicitHeader(headerSize);
  }

  this->logger->info("HS updated to:", headerSize);
  this->settings.headerSize = headerSize;

  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateRetry(int retry, bool callCb) {
  this->logger->info("RT updated to:", retry);
  this->settings.retry = retry;

  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::runSettingsCallback() {
  if (this->settingsUpdatedCallback) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateCodingRate(int codingRate, bool callCb) {
  if (this->radio->setCodingRate(codingRate) == RADIOLIB_ERR_INVALID_CODING_RATE) {
    this->logger->log("Selected CR is invalid for this module:", codingRate);
    if (codingRate != DEFAULT_CODDING_RATE) {
      this->logger->log("Set default CR:", DEFAULT_CODDING_RATE);
      this->updateCodingRate(DEFAULT_CODDING_RATE, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("CR updated to:", codingRate);
  this->settings.codingRate = codingRate;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateSyncWord(int syncWord, bool callCb) {
  if (this->radio->setSyncWord(syncWord) == RADIOLIB_ERR_INVALID_SYNC_WORD) {
    this->logger->log("Selected SW is invalid for this module:", syncWord);
    if (syncWord != DEFAULT_SYNC_WORD) {
      this->logger->log("Set default SW:", DEFAULT_SYNC_WORD);
      this->updateSyncWord(DEFAULT_SYNC_WORD, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("SW updated to:", syncWord);
  this->settings.syncWord = syncWord;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updatePreambleLength(int preambleLength, bool callCb) {
  if (this->radio->setPreambleLength(preambleLength) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH ) {
    this->logger->log("Selected PL is invalid for this module:", preambleLength);
    if (preambleLength != DEFAULT_PREAMBLE_LENGTH) {
      this->logger->log("Set default PL:", DEFAULT_PREAMBLE_LENGTH);
      this->updatePreambleLength(DEFAULT_PREAMBLE_LENGTH, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("PL updated to:", preambleLength);
  this->settings.preambleLength = preambleLength;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

void LoraSettingsManager::updateCurrentLimit(int currentLimit, bool callCb) {
  if (this->radio->setCurrentLimit(currentLimit) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT ) {
    this->logger->log("Selected CL is invalid for this module:", currentLimit);
    if (currentLimit != DEFAULT_CURRENT_LIMIT) {
      this->logger->log("Set default CL:", DEFAULT_CURRENT_LIMIT);
      this->updateCurrentLimit(DEFAULT_CURRENT_LIMIT, callCb);
      return;
    } else {
      while (true) { delay(10); }
    }
  }
  
  this->logger->info("CL updated to:", currentLimit);
  this->settings.currentLimit = currentLimit;
  if (this->settingsUpdatedCallback && callCb) {
    this->settingsUpdatedCallback(settings);
  }
}

LoraSettings LoraSettingsManager::getSettings() {
  return this->settings;
}

void LoraSettingsManager::setDefaultSettings() {
  LoraSettings defaultSettings = {
    DEFAULT_FREQUENCY,
    DEFAULT_BANDWIDTH,
    DEFAULT_SPREADING_FACTOR,
    DEFAULT_CODDING_RATE,
    DEFAULT_SYNC_WORD,
    DEFAULT_TRANSMIT_POWER,
    DEFAULT_IMPLICIT_HEADER,
    DEFAULT_HEADER_SIZE,
    DEFAULT_PREAMBLE_LENGTH,
    DEFAULT_CURRENT_LIMIT,
    DEFAULT_RETRY
  };
  this->updateSettings(defaultSettings);
}