#pragma once

#include <Arduino.h>

// http://linkdata.org/api/1/rdf1s911i/JapaneseLocalGovermentCodeTable_rdf.json
#define LOCAL_GOV_CODE 27000  // Osaka

enum class MESSAGE : uint32_t {
  MSG_NOTHING = 0,
  MSG_INIT_CLOCK,
  MSG_INIT_DATA,
  MSG_INIT_FORCAST,
  MSG_CHECK_CLOCK,
  MSG_CHECK_DATA,         // 気温・湿度・気圧
  MSG_CHECK_FORCAST,      // 天気予報
  MSG_WEATHER_100 = 100,  // 晴れ
  MSG_WEATHER_101 = 101,  // 晴れ 時々 曇り
  MSG_WEATHER_110 = 110,  // 晴れ のち 曇り
  MSG_WEATHER_200 = 200,  // 曇り
  MSG_WEATHER_201 = 201,  // 曇り 時々 晴れ
  MSG_WEATHER_210 = 210,  // 曇り のち 晴れ
  MSG_WEATHER_211 = 211,  // 曇り のち 時々 晴れ
  MSG_WEATHER_212 = 212,  // 曇り のち 雨
  MSG_WEATHER_300 = 300,  // 雨
  MSG_WEATHER_302 = 302,  // 雨   時々 止む
  MSG_WEATHER_313 = 313,  // 雨   のち 曇り
  MSG_OPEN_GIFFILE,
  MSG_MAX = 9999
};
