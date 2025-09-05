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

  DashboardItemFloat itemsFloat[] = {
    {"FQ:", settings.frequency},
    {"BW:", settings.bandwidth},
  };

  DashboardItemInt itemsInt[] = {
    {"SW:", settings.syncWord},
    {"SF:", settings.spreagingFactor},
    {"CR:", settings.codingRate},
    {"PL:", settings.preambleLength},
    {"TP:", settings.transmitPower},
    {"CL:", settings.currentLimit},
    {"IH:", settings.implicitHeader},
    {"HS:", settings.headerSize},
    {"RT:", settings.retry}
  };

  int colFloatKey[] = {5, 70};
  int colFloatValue[] = {25, 90};
  int colIntKey[] = {5, 50, 90};
  int colIntValue[] = {25, 70, 110};
  int line[] = {5, 15, 25, 35, 45};

  int itemFloatCount = sizeof(itemsFloat) / sizeof(itemsFloat[0]);
  int itemIntCount = sizeof(itemsInt) / sizeof(itemsInt[0]);

  for (int i = 0; i < itemFloatCount; i++) {
    int col = i % 2;
    int row = i / 2;

    int keyX = colFloatKey[col];
    int valX = colFloatValue[col];
    int y = line[row];

    oled.setCursor(keyX, y);
    oled.println(itemsFloat[i].key);

    oled.setCursor(valX, y);
    oled.println(itemsFloat[i].value);
  }

  for (int i = 0; i < itemIntCount; i++) {
    int col = i % 3;
    int row = (i / 3) + (itemFloatCount / 2);

    int keyX = colIntKey[col];
    int valX = colIntValue[col];
    int y = line[row];

    oled.setCursor(keyX, y);
    oled.println(itemsInt[i].key);

    oled.setCursor(valX, y);
    oled.println(itemsInt[i].value);
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
    if (passedTime > 1000) {
      this->setState(DISPLAY_DASHBOARD);
    }
  }
}