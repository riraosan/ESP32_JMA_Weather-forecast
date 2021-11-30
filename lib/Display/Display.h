
#pragma once

#include <memory>
#include <Arduino.h>
#include <SPIFFS.h>
#include <AnimatedGIF.h>
#include <ESP_8_BIT_GFX.h>
#include <WeatherCode.h>

#define FILESYSTEM SPIFFS

class Display {
 public:
  Display(void);
  void begin(uint16_t irPin, bool ntsc, uint8_t colorDepth);
  void update(void);
  void setTextOffset(int16_t x, int16_t y);
  void displayWeatherInfo(void);
  void setWeatherInfo(float temperature, float humidity, float pressure, String time);
  void setNtpTime(String ntpTime);

  static std::unique_ptr<ESP_8_BIT_GFX> videoOut;

  static void sendMessage(MESSAGE message);
  static inline void GIFDraw(GIFDRAW *pDraw);

  static void *GIFOpenFile(const char *fname, int32_t *pSize) {
    _file = FILESYSTEM.open(fname);

    if (_file) {
      *pSize = _file.size();
      return (void *)&_file;
    }

    return NULL;
  }

  static void GIFCloseFile(void *pHandle) {
    File *f = static_cast<File *>(pHandle);

    if (f != NULL)
      f->close();
  }

  static int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f    = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
      iBytesRead = pFile->iSize - pFile->iPos - 1;  // <-- ugly work-around
    if (iBytesRead <= 0)
      return 0;

    iBytesRead  = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();

    return iBytesRead;
  }

  static int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
    int   i = micros();
    File *f = static_cast<File *>(pFile->fHandle);

    f->seek(iPosition);
    pFile->iPos = (int32_t)f->position();
    i           = micros() - i;
    //  Serial.printf("Seek time = %d us\n", i);
    return pFile->iPos;
  }

 private:
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
  String _time;
  String _ntpTime;

  // color
  uint16_t _bgColor;
  uint16_t _bgTitle;
  uint16_t _bgTemperature;
  uint16_t _bgPressure;
  uint16_t _bgHumidity;

  static File _file;
};
