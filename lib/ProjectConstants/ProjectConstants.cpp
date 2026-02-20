#include "ProjectConstants.h"

String getDeviceId() {
  uint64_t chipId = ESP.getEfuseMac();

  uint16_t shortId = (chipId & 0xFFFF);

  char buf[5];
  sprintf(buf, "%04X", shortId);

  return String(buf);
}