#ifndef PROJECTS_CONSTANT_H
#define PROJECTS_CONSTANT_H

#define DEFAULT_FREQUENCY 868
#define DEFAULT_BANDWIDTH 125
#define DEFAULT_SPREADING_FACTOR 10
#define DEFAULT_CODDING_RATE 6
#define DEFAULT_TRANSMIT_POWER 10
#define DEFAULT_SYNC_WORD 0xAB

struct LoraSettings {
  float frequency;
  float bandwidth;
  int spreagingFactor;
  int codingRate;
  int syncWord;
  int transmitPower;
};

#endif