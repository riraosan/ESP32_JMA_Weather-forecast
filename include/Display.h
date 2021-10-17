#ifndef _DISP_WEATHERINFO_APP
#define _DISP_WEATHERINFO_APP

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#if defined(ENABLE_ANIMATION)
#include <AnimatedGIF.h>
#endif
#include <ESP_8_BIT_GFX.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <memory>

enum class MESSAGE : int {
  MSG_DO_NOTHING,
  MSG_WRITE_BUFFER,
  MSG_MAX,
};

class Display {
 public:
  Display(void);
  void begin(void);
  void begin(uint16_t irPin, bool ntsc, uint8_t colorDepth);
  void update(void);
  void setTextOffset(int16_t x, int16_t y);
  void displayWeatherInfo(void);
  void setWeatherInfo(float temperature, float humidity, float pressure, String time);
  void setNtpTime(String ntpTime);

  static void sendMessage(MESSAGE msg);

#if defined(ENABLE_ANIMATION)
  static void GIFDraw(GIFDRAW* pDraw);
  AnimatedGIF gif;
#endif

  static std::unique_ptr<ESP_8_BIT_GFX> videoOut;
  std::unique_ptr<IRsend>               irsend;

 private:
  static MESSAGE _message;
  static int16_t _textOffset_x;
  static int16_t _textOffset_y;

  //data
  float  _temperature;
  float  _humidity;
  float  _pressure;
  String _time;
  String _ntpTime;

  //color
  uint16_t _bgColor;
  uint16_t _bgTitle;
  uint16_t _bgTemperature;
  uint16_t _bgPressure;
  uint16_t _bgHumidity;
};

#endif
