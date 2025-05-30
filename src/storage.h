#ifndef STORAGE_H
#define STORAGE_H

#include <Preferences.h>
#include "lora.h"

class Storage {

  public:
    Storage();
    void init();
    void saveLoraSettings(LoraSettings settings);
    LoraSettings getLoraSettings();

  private:
    Preferences preferences;
};

#endif