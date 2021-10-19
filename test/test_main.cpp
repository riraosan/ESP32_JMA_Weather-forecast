
#include <Arduino.h>
#include <unity.h>
#include <memory>
#include <Display.h>

Display disp;

void setUp(void) {
  disp.videoOut->fillScreen(0x0000);
}

void tearDown(void) {
  disp.update();
}

void font_test1(void) {
  disp.videoOut->setTextSize(1);
  disp.videoOut->setTextColor(0xFFFF, 0x0000);

  for (int i = 0; i < 14; i++) {
    disp.videoOut->printEfont("あいうえおあいうえおあいうえお", 2, 10 + 16 * i);
  }
}

void font_test2(void) {
  disp.videoOut->setTextSize(2);
  disp.videoOut->setTextColor(0xFFFF, 0x0000);
  disp.videoOut->setCursor(5, 10);
  disp.videoOut->printEfont("フォント2");
}

void font_test3(void) {
  disp.videoOut->setTextSize(3);
  disp.videoOut->setTextColor(0xFFFF, 0x0000);
  disp.videoOut->setCursor(5, 10);
  disp.videoOut->printEfont("フォント3");
}

void icon_test_100(void) {
  disp.sendMessage(MESSAGE::MSG_WRITE_100);
}

void icon_test_200(void) {
  disp.sendMessage(MESSAGE::MSG_WRITE_200);
}

void icon_test_300(void) {
  disp.sendMessage(MESSAGE::MSG_WRITE_300);
}

void endtest(void){
  disp.videoOut->fillScreen(0x03e0);
  disp.videoOut->setCursor(5, 10);
  disp.videoOut->setTextColor(0xffff, 0x03e0);
  disp.videoOut->printEfont("テスト終了");
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();  // IMPORTANT LINE!

  disp.begin(12, true, 16);
  disp.sendMessage(MESSAGE::MSG_DO_NOTHING);

  RUN_TEST(font_test1);
  delay(3000);

  RUN_TEST(font_test2);
  delay(3000);

  RUN_TEST(font_test3);
  delay(3000);

  RUN_TEST(icon_test_100);
  delay(3000);

  RUN_TEST(icon_test_200);
  delay(3000);

  RUN_TEST(icon_test_300);
  delay(3000);

  endtest();

  UNITY_END();  // stop unit testing
}

void loop() {
  disp.update();
}
