
#pragma once

#include <memory>
#include <Arduino.h>
#include <SPIFFS.h>
#include <AnimatedGIF.h>
#include <ArduinoJson.h>
#include <WeatherCode.h>
#include <efontEnableJaMini.h>
#include <efontFontData.h>
#include <M5Unified.h>
#include <ESP32_8BIT_CVBS.h>

class Display {
public:
  Display(void);
  void begin(void);
  void update(void);
  void setNtpTime(String ntpTime);
  void setYMD(String ymd);
  void setWeatherInfo(float temperature, float humidity, float pressure, String time);
  void displayWeatherInfo(void);
  void setWeatherForecast(String filename, String forecastJP, String forecastEN);
  void displayIllustration(void);

  static void sendMessage(MESSAGE message);

private:
  static inline void    _GIFDraw(GIFDRAW *pDraw);
  static inline void   *_GIFOpenFile(const char *fname, int32_t *pSize);
  static inline void    _GIFCloseFile(void *pHandle);
  static inline int32_t _GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen);
  static inline int32_t _GIFSeekFile(GIFFILE *pFile, int32_t iPosition);

  AnimatedGIF _gif;

  // data
  float  _temperature;
  float  _humidity;
  float  _pressure;
  String _ntpTime;
  String _ymd;
  String _daytimeFormat;

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

  static int _width;
  static int _height;

  static MESSAGE _message;

  static ESP32_8BIT_CVBS _display;
  static M5Canvas        _animation;
  M5Canvas               _title;
  M5Canvas               _data;
};
