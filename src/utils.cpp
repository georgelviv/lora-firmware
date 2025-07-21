#include "utils.h"

int parseParams(String str, String paramsPairs[8][2]) {
  int paramIndex = 0;
  int length = str.length();
  bool inQuotes = false;
  String current = "";

  for (int i = 0; i < length; i++) {
    char c = str[i];
    if (c == '"') {
      inQuotes = !inQuotes;
    }
    if (c == ',' && !inQuotes) {
      int eqIndex = current.indexOf('=');
      if (eqIndex != -1) {
        paramsPairs[paramIndex][0] = current.substring(0, eqIndex);
        paramsPairs[paramIndex][1] = current.substring(eqIndex + 1);
        paramIndex++;
      }
      current = "";
    } else {
      current += c;
    }
  }

  if (current.length() > 0) {
    int eqIndex = current.indexOf('=');
    if (eqIndex != -1) {
      paramsPairs[paramIndex][0] = current.substring(0, eqIndex);
      paramsPairs[paramIndex][1] = current.substring(eqIndex + 1);
      paramIndex++;
    }
  }

  return paramIndex;
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
  String parsedParams[5][2];
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