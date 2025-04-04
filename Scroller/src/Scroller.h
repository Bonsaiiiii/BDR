#ifndef Scroller_h
#define Scroller_h

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class Scroller {
  private:
    Adafruit_SSD1306& display;
    unsigned long lastUpdate = 0;
    bool scrollReverse = false;
    int scrollType, scrollSpeed, initialPos, endPos, heightPos;
    const char* variableText;
    int scrollVal;

  public:  
    Scroller(int initialPos, int endPos, int heightPos, const char* variableText, Adafruit_SSD1306& display, int scrollType = 1, int scrollSpeed = 100);
    void scrollLine();

  private:
    void fullScrollLine(int textWidth, int reverse);
    void handleLongText(int displayArea, int textWidth);
    void handleShortText();
};

#endif