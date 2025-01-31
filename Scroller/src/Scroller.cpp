#include "Scroller.h"

Scroller::Scroller(int initialPos, int endPos, int heightPos, const char* variableText, Adafruit_SSD1306& display)
  : display(display),
    initialPos(initialPos),
    endPos(endPos),
    heightPos(heightPos),
    variableText(variableText),
    scrollVal(initialPos) {}

void Scroller::scrollLine() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate < 100) return;

  const int displayArea = endPos - initialPos;
  const int textWidth = strlen(variableText) * 6;

  display.setTextWrap(false);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.fillRect(initialPos, heightPos, displayArea, 10, SSD1306_BLACK);

  (textWidth >= displayArea) ? handleLongText(displayArea, textWidth) 
                             : handleShortText();

  lastUpdate = currentMillis;
}

void Scroller::handleLongText(int displayArea, int textWidth) {
  display.setCursor(scrollVal, heightPos);
  display.print(variableText);

  const int textDisparity = textWidth - displayArea;
  const int reverseThreshold = initialPos - 2;
  const int forwardThreshold = initialPos + 2;

  if (!scrollReverse) {
    scrollVal--;
    if (scrollVal + textDisparity <= reverseThreshold) {
      scrollReverse = true;
    }
  } else {
    scrollVal++;
    if (scrollVal >= forwardThreshold) {
      scrollReverse = false;
    }
  }
}

void Scroller::handleShortText() {
  display.setCursor(initialPos, heightPos);
  display.print(variableText);
}