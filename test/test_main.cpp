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

constexpr uint16_t CODES_JSON_SIZE = 25000;

void setUp(void) {}

void tearDown(void) {}

void weather_test_004(void) {
  String forcast(_weather.getForecast());
  Serial.printf("%s\n", forcast.c_str());
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
}

void weather_test_005(void) {
  String today(_weather.getTodayForcast());
  Serial.printf("today:%s\n", today.c_str());
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
}

void weather_test_006(void) {
  String nextday(_weather.getNextdayForcast());
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
    _disp.sendMessage(MESSAGE::MSG_DISPLAY_FORCAST);
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

constexpr char filter[] PROGMEM = R"({"100": [true]})";

StaticJsonDocument<255> doc;
StaticJsonDocument<100> _filter;

void getWeatherCodesSpiffs(void) {
  if (!FILESYSTEM.begin()) {
    TEST_FAIL();
  }

  File file = SPIFFS.open("/codes.json");
  TEST_ASSERT(file.size() > 0);

  if (file) {
    deserializeJson(_filter, filter);

    DeserializationError error = deserializeJson(doc,
                                                 file,
                                                 DeserializationOption::Filter(_filter));
    if (error) {
      log_e("Failed to read file, using default configuration");
      TEST_FAIL();
      return;
    }

    JsonArray root = doc["100"];

    if (root.isNull() == false) {
      String _filename(String("/") + String((const char *)root[0]));  // "100.gif"
      String _forcast_jp((const char *)root[3]);                      // "晴"
      String _forcast_en((const char *)root[4]);                      // "CLEAR"

      TEST_ASSERT_EQUAL_STRING("/100.gif", _filename.c_str());
      TEST_ASSERT_EQUAL_STRING("晴", _forcast_jp.c_str());
      TEST_ASSERT_EQUAL_STRING("CLEAR", _forcast_en.c_str());

      log_i("%s, %s, %s", _filename.c_str(), _forcast_jp.c_str(), _forcast_en.c_str());
    }
  }

  file.close();
}

void setup() {
  // NOTE!!! Wait for >2 secs
  delay(2000);

  UNITY_BEGIN();  // IMPORTANT LINE!

  RUN_TEST(getWeatherCodesSpiffs);

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
