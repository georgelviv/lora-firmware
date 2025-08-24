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

  int row1Key = 5;
  int row2Key = 70;
  int row1Value = 25;
  int row2Value = 90;

  int line1 = 5;
  int line2 = 15;
  int line3 = 25;
  int line4 = 35;
  int line5 = 45;

  // Line 1
  oled.setCursor(row1Key, line1);
  oled.println("FQ:");
  oled.setCursor(row1Value, line1);
  oled.println(this->settings.frequency);

  oled.setCursor(row2Key, line1);
  oled.println("BW:");
  oled.setCursor(row2Value, line1);
  oled.println(this->settings.bandwidth);

  //Line 2
  oled.setCursor(row1Key, line2);
  oled.println("SW:");
  oled.setCursor(row1Value, line2);
  oled.println(this->settings.syncWord);

  oled.setCursor(row2Key, line2);
  oled.println("SF:");
  oled.setCursor(row2Value, line2);
  oled.println(this->settings.spreagingFactor);

  // Line 3
  oled.setCursor(row1Key, line3);
  oled.println("CR:");
  oled.setCursor(row1Value, line3);
  oled.println(this->settings.codingRate);

  oled.setCursor(row2Key, line3);
  oled.println("PL:");
  oled.setCursor(row2Value, line3);
  oled.println(this->settings.preambleLength);


  // Line 4
  oled.setCursor(row1Key, line4);
  oled.println("TP:");
  oled.setCursor(row1Value, line4);
  oled.println(this->settings.transmitPower);

  oled.setCursor(row2Key, line4);
  oled.println("CL:");
  oled.setCursor(row2Value, line4);
  oled.println(110);

  // Line 5
  oled.setCursor(row1Key, line5);
  oled.println("IH:");
  oled.setCursor(row1Value, line5);
  oled.println(this->settings.implicitHeader);

  oled.setCursor(row2Key, line5);
  oled.println("HS:");
  oled.setCursor(row2Value, line5);
  oled.println(this->settings.headerSize);

  oled.display();
}

void Display::setDashboardSettings(LoraSettings settings) {
  this->settings = settings;
  if (this->currentState == DISPLAY_DASHBOARD) {
    this->setDashboard();
  }
}