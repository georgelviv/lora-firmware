#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ProjectConstants.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

enum DisplayState {
  DISPLAY_INIT,
  DISPLAY_DASHBOARD,
  DISPLAY_TMP_MESSAGE
};

struct DashboardItem {
  const char* key;
  float value;
};

class Display {

  public:
    Display();
    bool init();
    void setState(DisplayState state);
    void setDashboardSettings(LoraSettings settings);
    void check();
    void showTempMsg(String msg1, String ms2);

  private:
    Adafruit_SSD1306 oled;
    DisplayState currentState = DISPLAY_INIT;
    LoraSettings settings;

    void setInit();
    void setDashboard();
    void setTempMsg();

    String tmpMsg1;
    String tmpMsg2;
    unsigned long tmpMsgStart = 0;
};

#endif