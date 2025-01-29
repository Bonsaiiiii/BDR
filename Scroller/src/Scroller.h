#ifndef Scroller_h
#define Scroller_h

#include <Arduino.h>
#include <Adafruit_SSD1306.h>  // Include the display library

class Scroller {
  private:
    unsigned long lastUpdate = 0;
    bool scrollReverse = 0;
    unsigned long currentMillis;
    int initialPos, endPos, heightPos;
    char* variableText;
    int scrollVal;
    Adafruit_SSD1306* display;  // Pointer to an Adafruit_SSD1306 object

  public:  
    Scroller(int initialPos, int endPos, int heightPos, char* variableText, Adafruit_SSD1306* display);  // Constructor

    void scrollLine();  // Method to scroll the line
};

#endif
