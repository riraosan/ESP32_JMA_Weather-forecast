
#pragma once

#include <memory>
#include <Arduino.h>
#include <AnimatedGIF.h>
#include <ESP_8_BIT_GFX.h>
#include <WeatherCode.h>

class Display {
 public:
  Display(void);
  void begin(uint16_t irPin, bool ntsc, uint8_t colorDepth);
  void update(void);
  void setTextOffset(int16_t x, int16_t y);
  void displayWeatherInfo(void);
  void setWeatherInfo(float temperature, float humidity, float pressure, String time);
  void setNtpTime(String ntpTime);

  static void sendMessage(MESSAGE message);

  static void GIFDraw(GIFDRAW* pDraw);
  AnimatedGIF gif;

  static std::unique_ptr<ESP_8_BIT_GFX> videoOut;

 private:
  static MESSAGE _message;
  static int16_t _textOffset_x;
  static int16_t _textOffset_y;
  static int16_t _gifOffset_x;
  static int16_t _gifOffset_y;

  // data
  float  _temperature;
  float  _humidity;
  float  _pressure;
  String _time;
  String _ntpTime;

  // color
  uint16_t _bgColor;
  uint16_t _bgTitle;
  uint16_t _bgTemperature;
  uint16_t _bgPressure;
  uint16_t _bgHumidity;
};
