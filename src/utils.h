#ifndef UTILS_H
#define UTILS_H

#include "Arduino.h"

String getCommand(String msg);
String getParams(String msg);
int parseParams(String str, String paramsPairs[12][2]);
String getParam(String params, String lookingParam);
String formatParams(std::initializer_list<String> args);

#endif