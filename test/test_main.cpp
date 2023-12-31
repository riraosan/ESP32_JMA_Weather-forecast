
// Unit Test
#include <unity.h>
#include <memory>
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <secrets.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <esp32-hal-log.h>

#include <Display.h>
#include <Weather.h>
#include <Connect.h>

#include "weatherTest.h"
#include "displayTest.h"

Connect _wifi;

#define TEST_DISPLAY
//#define TEST_WEATHER

void setUp(void) {
}

void tearDown(void) {
}

void connectWiFi(void) {
  _wifi.setTaskName("AutoConnect");
  _wifi.setTaskSize(4096 * 1);
  _wifi.setTaskPriority(2);
  _wifi.setCore(0);
  _wifi.begin(SECRET_SSID, SECRET_PASS);
  _wifi.start(nullptr);
}

void setup() {
  // Serial.begin(115200);
  //  NOTE!!! Wait for >2 secs
  delay(2000);
  // connectWiFi();

  UNITY_BEGIN();  // IMPORTANT LINE!

#if defined(TEST_WEATHER)
  // RUN_TEST(weather_test_001);
#elif defined(TEST_DISPLAY)

  if (!SPIFFS.begin()) {
    log_e("FILESYSTEM Mount Failed");
  } else {
    RUN_TEST(display_test_000);
    RUN_TEST(display_test_003);
    // RUN_TEST(display_test_001);
    // RUN_TEST(display_test_002);
  }
#endif

  UNITY_END();  // stop unit testing
}

void loop() {
}
