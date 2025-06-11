#include "utils.h"

int parseParams(String str, String paramsPairs[7][2]) {
  String params[7];
  int paramIndex = 0;
  int startIndex = 0;
  int commaIndex;
  
  while ((commaIndex = str.indexOf(",", startIndex)) != -1) {
    params[paramIndex] = str.substring(startIndex, commaIndex);
    startIndex = commaIndex + 1;
    paramIndex++;
  }

  params[paramIndex] = str.substring(startIndex);

  for (int i = 0; i < paramIndex + 1; i++) {
    String param = params[i];
    int eqIndex = param.indexOf('=');
    String key = param.substring(0, eqIndex);
    String val = param.substring(eqIndex + 1);

    paramsPairs[i][0] = key;
    paramsPairs[i][1] = val;
  }

  return paramIndex + 1;
}

String getCommand(String msg) {
  String command;

  int commaIndex = msg.indexOf(';');

  if (commaIndex != -1) {
    command = msg.substring(0, commaIndex);
  } else {
    command = msg;
  }

  return command;
}

String getParams(String msg) {
  String params;

  int commaIndex = msg.indexOf(';');

  if (commaIndex != -1) {
    params = msg.substring(commaIndex + 1);
  }

  return params;
}

String formatParams(std::initializer_list<String> args) {
  String result;
  int count = 0;
  for (auto it = args.begin(); it != args.end(); ++it) {
    if (count % 2 == 0) {
      result += *it + "=";
    } else {
      result += *it;
      if (count < args.size() - 1) result += ",";
    }
    count++;
  }
  return result;
}

String getParam(String params, String lookingParam) {
  String parsedParams[1][2];
  int paramsCount = parseParams(params, parsedParams);

  for (int i = 0; i < paramsCount; i++) {
    String key = parsedParams[i][0];
    String val = parsedParams[i][1];
    if (key == lookingParam) {
      return val;
    }
  }

  return "";
}