// Unit Test

#include <Arduino.h>
#include <unity.h>
#include <memory>
#include <secrets.h>
#include <WiFiClient.h>
#include <Display.h>
#include <Weather.h>
#include <Connect.h>
#include <codes.h>
#include <ArduinoJson.h>
#include <esp32-hal-log.h>

Connect    _wifi;
Display    _disp;
WiFiClient _client;
Weather    _weather;

StaticJsonDocument<25000> _doc;

void setUp(void) {}

void tearDown(void) {}

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

  _disp.update();
}

void font_test2(void) {
  _disp.videoOut->setTextSize(1);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);  // RED
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 1");
  _disp.update();
}

void font_test3(void) {
  _disp.videoOut->setTextSize(2);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);  // BLUE
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 2");
  _disp.update();
}

void font_test4(void) {
  _disp.videoOut->setTextSize(3);
  _disp.videoOut->setTextColor(0xFFFF, 0x0000);  // YELLOW
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->printEfont("Font 3");
  _disp.update();
}

void starttest(void) {
  _disp.videoOut->setTextSize(4);
  _disp.videoOut->fillScreen(0x03e0);
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->setTextColor(0xffff, 0x03e0);
  _disp.videoOut->printEfont("START");
  _disp.update();
}

void parse_weathercode(void) {
  DeserializationError error = deserializeJson(_doc, weatherCodes);

  if (error) {
    log_e("deserializeJson() failed: ");
    log_e("%s", error.c_str());
    return;
  }

  for (int i = 100; i < 451; i++) {
    String    code(i);
    JsonArray root = _doc[(const char*)code.c_str()];

    if (root.isNull() == false) {
      log_printf("%d, ", i);                   // weather code
      log_printf("MSG_WEATHER_CODE_%d, ", i);  // weather code

      const char* root_0 = root[0];  // "100.svg"
      log_printf("%s, ", root_0);
      const char* root_1 = root[1];  // "500.svg"
      log_printf("%s, ", root_1);
      const char* root_2 = root[2];  // "100"
      log_printf("%s, ", root_2);
      const char* root_3 = root[3];  // "晴"
      log_printf("%s, ", root_3);
      const char* root_4 = root[4];  // "CLEAR"
      log_printf("%s\n", root_4);

      _disp.setWeatherCode(i);
      _disp.sendMessage(MESSAGE::MSG_DISPLAY_FORCAST);
      _disp.update();

      delay(3000);
    }
  }
}

void endtest(void) {
  _disp.videoOut->setTextSize(4);
  _disp.videoOut->fillScreen(0x03e0);
  _disp.videoOut->setCursor(5, 10);
  _disp.videoOut->setTextColor(0xffff, 0x03e0);
  _disp.videoOut->printEfont("END");
  _disp.update();
}

void setup() {
  // NOTE!!! Wait for >2 secs
  delay(2000);

  UNITY_BEGIN();  // IMPORTANT LINE!

#if 0
  _wifi.setTaskName("AutoConnect");
  _wifi.setTaskSize(4096 * 1);
  _wifi.setTaskPriority(2);
  _wifi.setCore(0);
  _wifi.begin(SECRET_SSID, SECRET_PASS);
  _wifi.start(nullptr);
  delay(5000);
#endif

  RUN_TEST(parse_weathercode);

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

#if 1
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

  endtest();
#endif

  UNITY_END();  // stop unit testing
}

void loop() {
  _disp.update();
  delay(1);
}
