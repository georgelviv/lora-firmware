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
  
  oled.setCursor(5, 5);
  oled.println("FQ:");
  oled.setCursor(25, 5);
  oled.println(this->settings.frequency);

  oled.setCursor(70, 5);
  oled.println("SF:");
  oled.setCursor(90, 5);
  oled.println(this->settings.spreagingFactor);

  oled.setCursor(5, 15);
  oled.println("BW:");
  oled.setCursor(25, 15);
  oled.println(this->settings.bandwidth);

  oled.setCursor(70, 15);
  oled.println("CR:");
  oled.setCursor(90, 15);
  oled.println(this->settings.codingRate);

  oled.setCursor(5, 25);
  oled.println("SW:");
  oled.setCursor(25, 25);
  oled.println(this->settings.syncWord);

  oled.display();
}

void Display::setDashboardSettings(LoraSettings settings) {
  this->settings = settings;
  if (this->currentState == DISPLAY_DASHBOARD) {
    this->setDashboard();
  }
}