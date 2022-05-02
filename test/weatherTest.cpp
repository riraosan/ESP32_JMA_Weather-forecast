
#include <Arduino.h>
#include <unity.h>
#include <Weather.h>
#include <esp32-hal-log.h>

#include "weatherTest.h"

Weather _weather;

void weather_test_001(void) {
  _weather.begin(LOCAL_GOV_CODE);
  TEST_ASSERT_EQUAL_STRING("success", _weather.getJMAForecastJson().c_str());
}

void weather_test_002(void) {
  _weather.begin(LOCAL_GOV_CODE);
  TEST_ASSERT_EQUAL_STRING("111", _weather.getTodayForecast().c_str());
}
