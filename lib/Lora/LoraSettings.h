#pragma once

#include "Arduino.h"
#include <RadioLib.h>
#include <ProjectConstants.h>
#include <Logger.h>

class LoraSettingsManager {
  public:
    LoraSettingsManager(SX1276 *radio, Logger *logger);

    void updateSettings(LoraSettings settings);
    void updateFrequency(float freq, bool callCb = true);
    void updateBandwidth(float bandwidth, bool callCb = true);
    void updateTransmitPower(int transmitPower, bool callCb = true);
    void updateSpreadingFactor(int spreadingFactor, bool callCb = true);
    void updateImplicitHeader(int isImplicitHeader, bool callCb = true);
    void updateHeaderSize(int headerSize, bool callCb = true);
    void updateCodingRate(int codingRate, bool callCb = true);
    void updateSyncWord(int syncWord, bool callCb = true);
    void updatePreambleLength(int preambleLength, bool callCb = true);
    void updateCurrentLimit(int currentLimit, bool callCb = true);
    void updateRetry(int retry, bool callCb = true);
  
    void setSettingsUpdatedCallback(void (* callback)(LoraSettings settings));
    LoraSettings getSettings();

    Logger *logger;
    void setDefaultSettings();
    void runSettingsCallback();

  private:
    SX1276 *radio;
    LoraSettings settings;
    void (* settingsUpdatedCallback)(LoraSettings settings);
};