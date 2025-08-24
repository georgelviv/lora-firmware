#include "display.h"

Display::Display() 
  : oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

bool Display::init() {
  Wire.begin(21, 22);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  oled.clearDisplay();
  oled.display();

  this->setInit();

  return true;
}

void Display::setState(DisplayState state) {
  this->currentState = state;

  switch (this->currentState) {
    case DISPLAY_INIT:
      this->setInit();
      break;
    case DISPLAY_DASHBOARD:
      this->setDashboard();
      break;
    case DISPLAY_TMP_MESSAGE:
      this->setTempMsg();
      break;
  }
}

void Display::setInit() {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(10, 10);
  oled.setTextSize(1);
  oled.println("Initializing");
  oled.display();
}

void Display::setDashboard() {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);

  DashboardItem items[] = {
    {"FQ:", settings.frequency},
    {"BW:", settings.bandwidth},
    {"SW:", (float)settings.syncWord},
    {"SF:", (float)settings.spreagingFactor},
    {"CR:", (float)settings.codingRate},
    {"PL:", (float)settings.preambleLength},
    {"TP:", (float)settings.transmitPower},
    {"CL:", (float)settings.currentLimit},
    {"IH:", (float)settings.implicitHeader},
    {"HS:", (float)settings.headerSize},
  };

  int colKey[] = {5, 70};
  int colValue[] = {25, 90};
  int line[] = {5, 15, 25, 35, 45};

  int itemCount = sizeof(items) / sizeof(items[0]);

  for (int i = 0; i < itemCount; i++) {
    int col = i % 2;
    int row = i / 2;

    int keyX = colKey[col];
    int valX = colValue[col];
    int y = line[row];

    oled.setCursor(keyX, y);
    oled.println(items[i].key);

    oled.setCursor(valX, y);
    oled.println(items[i].value);
  }

  oled.display();
}

void Display::setDashboardSettings(LoraSettings settings) {
  this->settings = settings;
  if (this->currentState == DISPLAY_DASHBOARD) {
    this->setDashboard();
  }
}

void Display::showTempMsg(String msg1, String msg2) {
  this->tmpMsgStart = millis();
  this->tmpMsg1 = msg1;
  this->tmpMsg2 = msg2;
  this->setState(DISPLAY_TMP_MESSAGE);
}

void Display::setTempMsg() {
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(10, 10);
  oled.setTextSize(1);
  oled.println(this->tmpMsg1);

  oled.setCursor(10, 20);
  oled.println(this->tmpMsg2);
  oled.display();
}

void Display::check() {
  if (this->currentState == DISPLAY_TMP_MESSAGE) {
    unsigned long passedTime = millis() - this->tmpMsgStart;
    if (passedTime > 2000) {
      this->setState(DISPLAY_DASHBOARD);
    }
  }
}