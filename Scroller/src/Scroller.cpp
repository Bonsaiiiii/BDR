#include "Scroller.h"

Scroller::Scroller(int initialPos, int endPos, int heightPos, const char* variableText, Adafruit_SSD1306& display, int scrollType, int scrollSpeed)
  : display(display),
    initialPos(initialPos),
    endPos(endPos),
    heightPos(heightPos),
    variableText(variableText),
    scrollType(scrollType),
    scrollSpeed(scrollSpeed),
    scrollVal(initialPos) {}

void Scroller::scrollLine() {
  const unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate < scrollSpeed) return;

  const int displayArea = endPos - initialPos;
  const int textWidth = strlen(variableText) * 6;

  display.setTextWrap(false);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.fillRect(initialPos, heightPos, displayArea, 10, SSD1306_BLACK);

  if (textWidth >= displayArea) {
    switch (scrollType) {  
        case 0: 
          handleShortText();
        break;
        case 1: 
          handleLongText(displayArea, textWidth);
        break;
        case 2:
          fullScrollLine(textWidth, 0);
        break;
        case 3:
          fullScrollLine(textWidth, 1);
        break;
    }
  } else {
      handleShortText();
  }
  lastUpdate = currentMillis;
}

void Scroller::fullScrollLine(int textWidth, int reverse) {
  display.setCursor(scrollVal, heightPos);
  display.print(variableText);
  if (reverse == 0) {
    scrollVal--;
    if ((scrollVal + textWidth) - initialPos < 0 ) {
      scrollVal = endPos;
    }
  } else if (reverse == 1) {
    scrollVal++;
    if (scrollVal > initialPos + textWidth) {
      scrollVal = initialPos - textWidth;
    }
  }
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