
#pragma once

#include <memory>
#include <Arduino.h>
#include <SPIFFS.h>
#include <AnimatedGIF.h>
#include <ESP_8_BIT_GFX.h>
#include <ArduinoJson.h>
#include <WeatherCode.h>

#define FILESYSTEM SPIFFS

class Display {
 public:
  Display(void);
  void begin(uint16_t irPin, bool ntsc, uint8_t colorDepth);
  void update(void);
  void setTextOffset(int16_t x, int16_t y);
  void setNtpTime(String ntpTime);
  void setWeatherInfo(float temperature, float humidity, float pressure, String time);
  void displayWeatherInfo(void);
  void setWeatherCode(uint32_t weatherCode);
  void displayIcon(void);

  static void sendMessage(MESSAGE message);

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
  uint32_t _code;
  String   _filename;
  String   _forcast_jp;
  String   _forcast_en;

  static StaticJsonDocument<25000> _doc;
  //   Free Heap : 100096
  //   Free Heap :  74672

  static File _file;
};
