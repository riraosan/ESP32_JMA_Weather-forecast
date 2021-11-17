#pragma once

#include <Arduino.h>

#define LOCAL_GOV_CODE 27000  // Osaka

enum class MESSAGE : uint32_t {
  MSG_NOTHING = 0,
  MSG_INIT_CLOCK,
  MSG_INIT_DATA,
  MSG_INIT_FORCAST,
  MSG_CHECK_CLOCK,
  MSG_CHECK_DATA,         //気温、湿度、気圧
  MSG_CHECK_FORCAST,      //天気予報
  MSG_WEATHER_100 = 100,  //晴れ
  MSG_WEATHER_101 = 101,  //晴れ時々曇り
  MSG_WEATHER_110 = 110,  //晴れのち曇り
  MSG_WEATHER_200 = 200,  //曇り
  MSG_WEATHER_201 = 201,  //
  MSG_WEATHER_210 = 210,  //曇りのち晴れ
  MSG_WEATHER_212 = 212,  //曇りのち雨
  MSG_WEATHER_300 = 300,  //雨
  MSG_WEATHER_302 = 302,  //
  MSG_WEATHER_313 = 313,  //雨のち曇り
  MSG_GIF_CLOSE,
  MSG_MAX = 9999
};
