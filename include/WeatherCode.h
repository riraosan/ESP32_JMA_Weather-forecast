
#pragma once

#include <Arduino.h>
#include <codes.h>
#include <filter.h>

// http://linkdata.org/api/1/rdf1s911i/JapaneseLocalGovermentCodeTable_rdf.json
#define LOCAL_GOV_CODE 27000  // Osaka

enum class MESSAGE : uint32_t {
  MSG_NOTHING = 0,
  MSG_INIT_CLOCK,
  MSG_INIT_DATA,
  MSG_INIT_FORCAST,
  MSG_CHECK_CLOCK,
  MSG_CHECK_DATA,
  MSG_CHECK_FORCAST,
  MSG_DISPLAY_CLOCK,    // NTP時刻
  MSG_DISPLAY_DATA,     // 気温・湿度・気圧
  MSG_DISPLAY_FORCAST,  // 天気予報
  MSG_OPEN_GIFFILE,
  MSG_MAX
};
