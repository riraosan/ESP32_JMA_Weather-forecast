#pragma once

#include <Arduino.h>

enum class MESSAGE : uint32_t {
  MSG_NOTHING,
  MSG_WRITE_DATA,   //気温、湿度、気圧
  MSG_WEATHER_100,  //晴れ
  MSG_WEATHER_200,  //曇り
  MSG_WEATHER_300,  //雨
  MSG_WEATHER_110,  //晴れのち曇り
  MSG_WEATHER_210,  //曇りのち晴れ
  MSG_WEATHER_212,  //曇りのち雨
  MSG_WEATHER_313,  //雨のち曇り
  MSG_MAX
};
