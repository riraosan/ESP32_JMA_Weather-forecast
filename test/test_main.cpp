
#include <Arduino.h>
#include <unity.h>
#include <memory>
#include "Display.h"

std::unique_ptr<Display> pApp(new Display);

//void setUp(void) {
// // set stuff up here
// }

void tearDown(void) {
  //clean stuff up here
  pApp.reset();
}

void TEST01(void) {
  pApp->videoOut->setTextSize(1);
  pApp->videoOut->setTextColor(0xFF, 0x00);

  for (int i = 0; i < 14; i++) {
    pApp->videoOut->printEfont("あいうえおあいうえおあいうえお", 5, 10 + 16 * i);
  }
}

void TEST02(void) {
  pApp->videoOut->setTextSize(2);
  pApp->videoOut->setTextColor(0xFFFF, 0x0000);
  pApp->videoOut->printEfont("test2");
}

void TEST03(void) {
  pApp->videoOut->setTextSize(3);
  pApp->videoOut->setTextColor(0xFFFF, 0x0000);
  pApp->videoOut->printEfont("test3");
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();  // IMPORTANT LINE!

  pApp->begin();
  TEST01();

  UNITY_END();  // stop unit testing
}

void loop() {
  pApp->update();
}
