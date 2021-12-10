// Unit Test

#include <Arduino.h>
#include <unity.h>
#include <memory>
#include <secrets.h>
#include <WiFiClient.h>
#include <Display.h>
#include <Weather.h>
#include <Connect.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>
#include <esp32-hal-log.h>

Connect    _wifi;
Display    _disp;
WiFiClient _client;
Weather    _weather;

StaticJsonDocument<255> _forecast;
StaticJsonDocument<100> filter;

void setUp(void) {}

void tearDown(void) {}

void weather_test_004(void) {
  String forecast(_weather.getForecast());
  Serial.printf("%s\n", forecast.c_str());
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
}

void weather_test_005(void) {
  String today(_weather.getTodayForecast());
  Serial.printf("today:%s\n", today.c_str());
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
}

void weather_test_006(void) {
  String nextday(_weather.getNextdayForecast());
  Serial.printf("nextday:%s\n", nextday.c_str());
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
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
  for (int i = 100; i < 451; i++) {
    _disp.setWeatherCode(i);
    _disp.sendMessage(MESSAGE::MSG_DISPLAY_FORECAST);
    _disp.update();

    delay(10);
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

void getForecastString_100(void) {
  String   _forecastFilter(R"({"__CODE__": [true]})");
  uint16_t code = 100;

  _forecastFilter.replace("__CODE__", String(code));

  deserializeJson(filter, _forecastFilter);

  File file = FILESYSTEM.open("/codes.json");
  TEST_ASSERT(file.size() > 0);

  if (file) {
    DeserializationError error = deserializeJson(_forecast,
                                                 file,
                                                 DeserializationOption::Filter(filter));
    if (error) {
      log_e("Failed to read file, using default configuration");
      TEST_FAIL();
      return;
    }

    JsonArray root = _forecast[String(code).c_str()];

    if (root.isNull() == false) {
      String _filename(String("/") + String((const char *)root[0]));  // "100.gif"
      String _forecast_jp((const char *)root[3]);                     // "晴"
      String _forecast_en((const char *)root[4]);                     // "CLEAR"

      TEST_ASSERT_EQUAL_STRING("/100.gif", _filename.c_str());
      TEST_ASSERT_EQUAL_STRING("晴", _forecast_jp.c_str());
      TEST_ASSERT_EQUAL_STRING("CLEAR", _forecast_en.c_str());

      log_i("%s_%s_%s", _filename.c_str(), _forecast_jp.c_str(), _forecast_en.c_str());
    } else {
      TEST_FAIL();
    }
  }

  file.close();
}
void getForecastString_450(void) {
  String   _forecastFilter(R"({"__CODE__": [true]})");
  uint16_t code = 450;

  _forecastFilter.replace("__CODE__", String(code));

  deserializeJson(filter, _forecastFilter);

  File file = FILESYSTEM.open("/codes.json");
  TEST_ASSERT(file.size() > 0);

  if (file) {
    DeserializationError error = deserializeJson(_forecast,
                                                 file,
                                                 DeserializationOption::Filter(filter));
    if (error) {
      log_e("Failed to read file, using default configuration");
      TEST_FAIL();
      return;
    }

    JsonArray root = _forecast[String(code).c_str()];

    if (root.isNull() == false) {
      String _filename(String("/") + String((const char *)root[0]));
      String _forecast_jp((const char *)root[3]);
      String _forecast_en((const char *)root[4]);

      TEST_ASSERT_EQUAL_STRING("/400.gif", _filename.c_str());
      TEST_ASSERT_EQUAL_STRING("雪で雷を伴う", _forecast_jp.c_str());
      TEST_ASSERT_EQUAL_STRING("SNOW AND THUNDER", _forecast_en.c_str());

      log_i("%s_%s_%s", _filename.c_str(), _forecast_jp.c_str(), _forecast_en.c_str());
    } else {
      TEST_FAIL();
    }
  }

  file.close();
}

void setup() {
  // NOTE!!! Wait for >2 secs
  delay(2000);

  UNITY_BEGIN();  // IMPORTANT LINE!

#if 1
  if (!FILESYSTEM.begin()) {
    TEST_FAIL();
  }

  RUN_TEST(getForecastString_100);
  RUN_TEST(getForecastString_450);
#endif

#if 0
  _wifi.setTaskName("AutoConnect");
  _wifi.setTaskSize(4096 * 1);
  _wifi.setTaskPriority(2);
  _wifi.setCore(0);
  _wifi.begin(SECRET_SSID, SECRET_PASS);
  _wifi.start(nullptr);
  delay(5000);

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

#if 0
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

  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  RUN_TEST(parse_weathercode);
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

  endtest();
#endif
  UNITY_END();  // stop unit testing
}

void loop() {
  //_disp.update();
  delay(1);
}
