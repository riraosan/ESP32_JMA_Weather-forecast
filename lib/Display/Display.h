
#pragma once

#include <AnimatedGIF.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP_8_BIT_GFX.h>
#include <SPIFFS.h>
#include <WeatherCode.h>

#include <memory>

class Display {
 public:
  Display(void);
  void begin(uint16_t irPin, bool ntsc, uint8_t colorDepth);
  void update(void);
  void setTextOffset(int16_t x, int16_t y);
  void setNtpTime(String ntpTime);
  void setWeatherInfo(float temperature, float humidity, float pressure, String time);
  void displayWeatherInfo(void);
  void setWeatherForecast(String filename, String forecastJP, String forecastEN);
  void displayIcon(void);
  void displayIllustration(void);

  static void
  sendMessage(MESSAGE message);

  static std::unique_ptr<ESP_8_BIT_GFX> videoOut;

 private:
  static inline void    _GIFDraw(GIFDRAW *pDraw);
  static inline void   *_GIFOpenFile(const char *fname, int32_t *pSize);
  static inline void    _GIFCloseFile(void *pHandle);
  static inline int32_t _GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen);
  static inline int32_t _GIFSeekFile(GIFFILE *pFile, int32_t iPosition);

  AnimatedGIF gif;

  static MESSAGE _message;
  static int16_t _textOffset_x;
  static int16_t _textOffset_y;
  static int16_t _gifOffset_x;
  static int16_t _gifOffset_y;

  // data
  float  _temperature;
  float  _humidity;
  float  _pressure;
  String _ntpTime;

  // color
  uint16_t _bgColor;
  uint16_t _bgTitle;
  uint16_t _bgTemperature;
  uint16_t _bgPressure;
  uint16_t _bgHumidity;

  // weatherCodes
  String _filename;
  String _forecastJP;
  String _forecastEN;

  // Illustration
  String _IllustrationName;

  static File _file;
};
