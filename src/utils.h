#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"

String getCommand(String msg);
String getParams(String msg);
int parseParams(String str, String paramsPairs[5][2]);

#endif