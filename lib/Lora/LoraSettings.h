#ifndef LORA_SETTINGS_H
#define LORA_SETTINGS_H

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
  
    void setSettingsUpdatedCallback(void (* callback)(LoraSettings settings));
    LoraSettings getSettings();

  private:
    SX1276 *radio;
    Logger *logger;
    LoraSettings settings;
    void (* settingsUpdatedCallback)(LoraSettings settings);
};

#endif