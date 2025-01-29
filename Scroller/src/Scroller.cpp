#include "Scroller.h"

// Constructor definition
Scroller::Scroller(int initialPos, int endPos, int heightPos, char* variableText, Adafruit_SSD1306* display) {
  this->initialPos = initialPos;
  this->endPos = endPos;
  this->heightPos = heightPos;
  this->variableText = variableText;
  this->scrollVal = initialPos;
  this->display = display;  // Assign the display object
}

// Method to scroll the text line
void Scroller::scrollLine() {
  currentMillis = millis();
  int displayArea = endPos - initialPos;
  int variableTextSize = String(variableText).length() * 6;  // Assuming each character is 6px wide
  int textDisparity = variableTextSize - displayArea;

  if (currentMillis - lastUpdate > 100) {  // Scroll update interval
    display->setTextWrap(false);  // Use the display pointer
    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(1);
    display->fillRect(initialPos, heightPos, displayArea, 10, 0);  // Clear previous text
    
    if (variableTextSize >= displayArea) {  // If text is longer than display
      display->setCursor(scrollVal, heightPos);
      display->print(variableText);

      if (scrollReverse == 0) {
        if (scrollVal + textDisparity != initialPos - 2) {
          scrollVal -= 1;
          if (scrollVal + textDisparity == initialPos - 2) {
            scrollReverse = 1;
          }
        }
      } else if (scrollReverse == 1) {
        if (scrollVal != initialPos + 2) {
          scrollVal += 1;
          if (scrollVal == initialPos + 2) {
            scrollReverse = 0;
          }
        }
      }
    } else {
      display->setCursor(initialPos, heightPos);
      display->print(variableText);
    }

    lastUpdate = currentMillis;  // Update the last update time
  }
}
