#ifndef LORA_SETTINGS_H
#define LORA_SETTINGS_H

#include "Arduino.h"
#include <RadioLib.h>

#define DEFAULT_FREQUENCY 868
#define DEFAULT_BANDWIDTH 125
#define DEFAULT_SPREADING_FACTOR 10
#define DEFAULT_CODDING_RATE 6
#define DEFAULT_SYNC_WORD 0xAB

struct LoraSettings {
  float frequency;
  float bandwidth;
  int spreagingFactor;
  int codingRate;
  int syncWord;
};

class LoraSettingsManager {
  public:
    LoraSettingsManager(SX1276 *radio);
    void updateSettings(LoraSettings settings);
    void updateFrequency(float freq, bool callCb = true);
    void updateBandwidth(float bandwidth, bool callCb = true);
    void setSettingsUpdatedCallback(void (* callback)(LoraSettings settings));

  private:
    SX1276 *radio;
    LoraSettings settings;
    void (* settingsUpdatedCallback)(LoraSettings settings);
};

#endif