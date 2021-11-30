// Unit Test

#include <Arduino.h>
#include <unity.h>
#include <memory>
#include <secrets.h>
#include <WiFiClient.h>
#include <Display.h>
#include <Weather.h>
#include <Connect.h>

Connect    _wifi;
Display    _disp;
WiFiClient _client;
Weather    _weather;

void setUp(void) {
  _disp.videoOut->fillScreen(0x0000);
}

void tearDown(void) {
  _disp.update();
}

void weather_test_004(void) {
  String forcast(_weather.getForecast());
  Serial.printf("%s\n", forcast.c_str());
}

void weather_test_005(void) {
  String today(_weather.getTodayForcast());
  Serial.printf("today:%s\n", today.c_str());
}

void weather_test_006(void) {
  String nextday(_weather.getNextdayForcast());
  Serial.printf("nextday:%s\n", nextday.c_str());
}

void font_test1(void) {
  _disp.videoOut->setTextWrap(true);
  _disp.videoOut->setTextSize(1);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);
  _disp.videoOut->setCursor(2, 10);

  for (int i = 0; i < 14; i++) {
    _disp.videoOut->printEfont("あいうえお");
  }
}

void font_test2(void) {
  _disp.videoOut->setTextSize(1);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);//RED
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 1");
}

void font_test3(void) {
  _disp.videoOut->setTextSize(2);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);//BLUE
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 2");
}

void font_test4(void) {
  _disp.videoOut->setTextSize(3);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);//YELLOW
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 3");
}

void icon_test_100(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_100);
}

void icon_test_101(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_101);
}

void icon_test_110(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_110);
}

void icon_test_200(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_200);
}

void icon_test_201(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_201);
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

void icon_test_302(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_302);
}

void icon_test_313(void) {
  _disp.sendMessage(MESSAGE::MSG_WEATHER_313);
}

void starttest(void) {
  _disp.videoOut->setTextSize(4);
  _disp.videoOut->fillScreen(0x03e0);
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->setTextColor(0xffff, 0x03e0);
  _disp.videoOut->printEfont("START");
}

void endtest(void) {
  _disp.videoOut->setTextSize(4);
  _disp.videoOut->fillScreen(0x03e0);
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->setTextColor(0xffff, 0x03e0);
  _disp.videoOut->printEfont("END");
}

void setup() {
  // NOTE!!! Wait for >2 secs
  delay(2000);

  UNITY_BEGIN();  // IMPORTANT LINE!

  _wifi.setTaskName("AutoConnect");
  _wifi.setTaskSize(4096 * 1);
  _wifi.setTaskPriority(2);
  _wifi.setCore(0);
  _wifi.begin(SECRET_SSID, SECRET_PASS);
  _wifi.start(nullptr);
  delay(5000);

#if 0
  _weather.setAreaCode(27000);
  _weather.begin(_client);
  delay(3000);

  RUN_TEST(weather_test_004);
  delay(3000);

  RUN_TEST(weather_test_005);
  delay(3000);

  RUN_TEST(weather_test_006);
  delay(3000);
#endif

  _disp.begin(12, true, 16);
  _disp.sendMessage(MESSAGE::MSG_NOTHING);

  starttest();
  delay(3000);

  RUN_TEST(font_test1);
  delay(3000);

  RUN_TEST(font_test2);
  delay(3000);

  RUN_TEST(font_test3);
  delay(3000);

  RUN_TEST(icon_test_100);
  delay(3000);

  RUN_TEST(icon_test_101);
  delay(3000);

  RUN_TEST(icon_test_110);
  delay(3000);

  RUN_TEST(icon_test_200);
  delay(3000);

  RUN_TEST(icon_test_201);
  delay(3000);

  RUN_TEST(icon_test_210);
  delay(3000);

  RUN_TEST(icon_test_212);
  delay(3000);

  RUN_TEST(icon_test_300);
  delay(3000);

  RUN_TEST(icon_test_302);
  delay(3000);

  RUN_TEST(icon_test_313);
  delay(3000);

  endtest();

  UNITY_END();  // stop unit testing
}

void loop() {
  _disp.update();
  delay(1);
}
