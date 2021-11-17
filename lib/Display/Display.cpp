
#include <Display.h>
#include <esp32-hal-log.h>

#include "icon/100.h"
#include "icon/101.h"
#include "icon/110.h"
#include "icon/200.h"
#include "icon/201.h"
#include "icon/210.h"
#include "icon/212.h"
#include "icon/300.h"
#include "icon/302.h"
#include "icon/313.h"

MESSAGE Display::_message = MESSAGE::MSG_NOTHING;

int16_t Display::_textOffset_x = 0;
int16_t Display::_textOffset_y = 10;
int16_t Display::_gifOffset_x  = 10;
int16_t Display::_gifOffset_y  = 10;

std::unique_ptr<ESP_8_BIT_GFX> Display::videoOut;

Display::Display() : _temperature(0.0),
                     _humidity(0.0),
                     _pressure(0.0),
                     _time("0000-00-00T00:00:00Z"),
                     _ntpTime("00:00:00"),
                     _bgColor(0x0000),
                     _bgTitle(0x0019),
                     _bgTemperature(_bgColor),
                     _bgPressure(_bgColor),
                     _bgHumidity(_bgColor) {
}

void Display::setTextOffset(int16_t x, int16_t y) {
  _textOffset_x = x;
  _textOffset_y = y;
}

void Display::sendMessage(MESSAGE message) {
  _message = message;
}

void Display::setNtpTime(String ntpTime) {
  _ntpTime = ntpTime;
}

void Display::begin(uint16_t irPin, bool ntsc, uint8_t colorDepth) {
  videoOut.reset(new ESP_8_BIT_GFX(ntsc, colorDepth));

  videoOut->begin();
  videoOut->copyAfterSwap = true;  // gif library depends on data from previous buffer
  videoOut->fillScreen(_bgColor);
  videoOut->waitForFrame();

  sendMessage(MESSAGE::MSG_CHECK_DATA);
}

void Display::setWeatherInfo(float temperature, float humidity, float pressure, String time) {
  _temperature = temperature;
  _humidity    = humidity;
  _pressure    = pressure;
  _time        = time;
}

void Display::displayWeatherInfo(void) {
  // log_i("[%s] %2.1f*C, %2.1f%%, %4.1fhPa", _time.c_str(), _temperature, _humidity, _pressure);

  // Header
  String time(" __TIME__         ");
  time.replace("__TIME__", _time);
  videoOut->setTextColor(0xFFFF, _bgTitle);
  videoOut->printEfont(" Osaka Weather Station        ", _textOffset_x, _textOffset_y + 16 * 0, 1);

  char tempe[10] = {0};
  char humid[10] = {0};
  char press[10] = {0};

  sprintf(tempe, "%2.1f", _temperature);
  sprintf(humid, " %2.0f", _humidity);
  sprintf(press, " %4.1f", _pressure);

  //気温
  videoOut->setCursor(_textOffset_x + 8 * 9 - 2, _textOffset_y + 16 * 12);
  videoOut->setTextColor(0xFFFF, _bgTemperature);
  videoOut->setTextSize(1);
  videoOut->printEfont(tempe);
  videoOut->printEfont("℃");

  //湿度
  videoOut->setTextColor(0xFFFF, _bgHumidity);
  videoOut->setTextSize(1);
  videoOut->printEfont(humid);
  videoOut->printEfont("％");

  //大気圧
  videoOut->setTextColor(0xFFFF, _bgPressure);
  videoOut->setTextSize(1);
  videoOut->printEfont(press);
  videoOut->printEfont("hPa");

  // footer
  String ntpTime(" __NTP__                     ");
  ntpTime.replace("__NTP__", _ntpTime);
  videoOut->setTextColor(0xFFFF, _bgTitle);
  videoOut->printEfont(ntpTime.c_str(), _textOffset_x, _textOffset_y + 16 * 13, 1);  // NTP Clock
}

void Display::update() {
  switch (_message) {
    case MESSAGE::MSG_CHECK_DATA:
      displayWeatherInfo();
      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    case MESSAGE::MSG_WEATHER_100:
      if (gif.open((uint8_t *)_100, size_100, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_101:
      if (gif.open((uint8_t *)_101, size_101, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_110:
      if (gif.open((uint8_t *)_110, size_110, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_200:
      if (gif.open((uint8_t *)_200, size_200, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_201:
      if (gif.open((uint8_t *)_201, size_201, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_210:
      if (gif.open((uint8_t *)_210, size_210, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_212:
      if (gif.open((uint8_t *)_212, size_212, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_300:
      if (gif.open((uint8_t *)_300, size_300, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_302:
      if (gif.open((uint8_t *)_302, size_302, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_WEATHER_313:
      if (gif.open((uint8_t *)_313, size_313, GIFDraw)) {
        gif.playFrame(true, NULL);
      }
      sendMessage(MESSAGE::MSG_GIF_CLOSE);
      break;
    case MESSAGE::MSG_GIF_CLOSE:
      gif.close();
      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    default:
      break;
  }
  videoOut->waitForFrame();
}

void Display::GIFDraw(GIFDRAW *pDraw) {
  uint8_t  *s;
  uint16_t *d, *usPalette, usTemp[320];
  int       x, y;

  usPalette = pDraw->pPalette;
  y         = pDraw->iY + pDraw->y;  // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2)  // restore to background color
  {
    for (x = 0; x < pDraw->iWidth; x++) {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }
  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency)  // if transparency used
  {
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int      x, iCount;
    pEnd   = s + pDraw->iWidth;
    x      = 0;
    iCount = 0;  // count non-transparent pixels
    while (x < pDraw->iWidth) {
      c = ucTransparent - 1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent)  // done, stop
        {
          s--;    // back up to treat it like transparent
        } else {  // opaque
          *d++ = usPalette[c];
          iCount++;
        }
      }            // while looking for opaque pixels
      if (iCount)  // any opaque pixels?
      {
        for (int xOffset = 0; xOffset < iCount; xOffset++) {
          videoOut->drawPixel(pDraw->iX + x + xOffset + _gifOffset_x, y + _gifOffset_y + 32, usTemp[xOffset]);
        }
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd) {
        c = *s++;
        if (c == ucTransparent)
          iCount++;
        else
          s--;
      }
      if (iCount) {
        x += iCount;  // skip these
        iCount = 0;
      }
    }
  } else {
    s = pDraw->pPixels;
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    for (x = 0; x < pDraw->iWidth; x++) {
      videoOut->drawPixel(x + _textOffset_x, y + _textOffset_y + 32, usPalette[*s++]);
    }
  }
}
