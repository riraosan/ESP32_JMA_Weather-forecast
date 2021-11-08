// Unit Test

#include <Arduino.h>
#include <unity.h>
#include <memory>
#include <secrets.h>
//#include <WiFiClient.h>
#include <HTTPClient.h>
#include <Display.h>
#include <Weather.h>
#include <Connect.h>

//#define _DISPLAY
#define _WEATHER

Connect _wifi;
#if defined(_DISPLAY)
Display _disp;
#endif
WiFiClient _client;
Weather    _weather;

void setUp(void) {
#if defined(_DISPLAY)
  _disp.videoOut->fillScreen(0x0000);
#endif
}

void tearDown(void) {
#if defined(_DISPLAY)
  _disp.update();
#endif
}

void weather_test_001(void) {
  // TEST_ASSERT_EQUAL_STRING("test", "test");
}

void weather_test_002(void) {
  // TEST_ASSERT_EQUAL_STRING("test", "test");
}

void weather_test_003(void) {
  // TEST_ASSERT_EQUAL_STRING("test", "test1");
}

void weather_test_004(void) {
  String forcast(_weather.getForecast(27000));
  //Serial.printf("%s\n", forcast.c_str());
}

void weather_test_005(void) {
  String today(_weather.getTodayForcast());
  Serial.printf("today:%s\n", today.c_str());
}

void weather_test_006(void) {
  String nextday(_weather.getNextdayForcast());
  Serial.printf("nextday:%s\n", nextday.c_str());
}

#if defined(_DISPLAY)
void font_test1(void) {
  _disp.videoOut->setTextSize(1);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);

  for (int i = 0; i < 14; i++) {
    _disp.videoOut->printEfont("あいうえおあいうえおあいうえお", 2, 10 + 16 * i);
  }
}

void font_test2(void) {
  _disp.videoOut->setTextSize(2);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 2");
}

void font_test3(void) {
  _disp.videoOut->setTextSize(3);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 3");
}

void icon_test_100(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_100);
}

void icon_test_110(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_110);
}

void icon_test_200(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_200);
}

void icon_test_210(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_210);
}

void icon_test_212(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_212);
}

void icon_test_300(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_300);
}

void icon_test_313(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_313);
}

void endtest(void) {
  _disp.videoOut->fillScreen(0x03e0);
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->setTextColor(0xffff, 0x03e0);
  _disp.videoOut->printEfont("テスト終了");
}
#endif

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();  // IMPORTANT LINE!

  _wifi.setTaskName("AutoConnect");
  _wifi.setTaskSize(4096 * 1);
  _wifi.setTaskPriority(2);
  _wifi.setCore(0);
  _wifi.begin(SECRET_SSID, SECRET_PASS);
  _wifi.start(nullptr);

  delay(5000);

  _weather.begin(_client);

  delay(1000);

  // RUN_TEST(weather_test_001);
  // RUN_TEST(weather_test_002);
  // RUN_TEST(weather_test_003);
  RUN_TEST(weather_test_004);
  RUN_TEST(weather_test_005);
  RUN_TEST(weather_test_006);

#if defined(_DISPLAY)
  _disp.begin(12, true, 16);
  _disp.sendMessage(MESSAGE::MSG_NOTHING);

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

  RUN_TEST(icon_test_210);
  delay(3000);

  RUN_TEST(icon_test_212);
  delay(3000);

  RUN_TEST(icon_test_300);
  delay(3000);

  RUN_TEST(icon_test_313);
  delay(3000);

  endtest();
#endif
  UNITY_END();  // stop unit testing
}

void loop() {
#if defined(_DISPLAY)
  _disp.update();
#endif
}
