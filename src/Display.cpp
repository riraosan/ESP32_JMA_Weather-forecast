
#include <Display.h>
#include <esp32-hal-log.h>

#if defined(ENABLE_ANIMATION)
#include "earth.h"
#endif
MESSAGE Display::_message = MESSAGE::MSG_DO_NOTHING;

int16_t                        Display::_textOffset_x = 2;
int16_t                        Display::_textOffset_y = 10;
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

void Display::begin(void) {}

void Display::sendMessage(MESSAGE msg) {
  _message = msg;
}

void Display::begin(uint16_t irPin, bool ntsc, uint8_t colorDepth) {
  videoOut.reset(new ESP_8_BIT_GFX(ntsc, colorDepth));

  videoOut->begin();
  videoOut->copyAfterSwap = true;  // gif library depends on data from previous buffer
  videoOut->fillScreen(_bgColor);
  videoOut->waitForFrame();

  sendMessage(MESSAGE::MSG_WRITE_BUFFER);
}

void Display::setWeatherInfo(float temperature, float humidity, float pressure, String time) {
  _temperature = temperature;
  _humidity    = humidity;
  _pressure    = pressure;
  _time        = time;
}

#if defined(ENABLE_ANIMATION)
void Display::GIFDraw(GIFDRAW *pDraw) {
  uint8_t * s;
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
          s--;  // back up to treat it like transparent
        } else  // opaque
        {
          *d++ = usPalette[c];
          iCount++;
        }
      }            // while looking for opaque pixels
      if (iCount)  // any opaque pixels?
      {
        for (int xOffset = 0; xOffset < iCount; xOffset++) {
          videoOut->drawPixel(pDraw->iX + x + xOffset, _textOffset_y + y, usTemp[xOffset]);
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
      videoOut->drawPixel(_textOffset_x + x, _textOffset_y + y + 32, usPalette[*s++]);
    }
  }
}
#endif

void Display::displayWeatherInfo(void) {
  //log_i("[%s] %2.1f*C, %2.1f%%, %4.1fhPa", _time.c_str(), _temperature, _humidity, _pressure);

  //Header
  String time(" __TIME__         ");
  time.replace("__TIME__", _time);
  videoOut->setTextColor(0xFFFF, _bgTitle);
  videoOut->printEfont(" Osaka Weather Station        ", _textOffset_x, _textOffset_y + 16 * 0, 1);
  //videoOut->printEfont(time.c_str(), _textOffset_x, _textOffset_y + 16 * 1, 1);

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

  //footer
  String ntpTime(" __NTP__                     ");
  ntpTime.replace("__NTP__", _ntpTime);
  videoOut->setTextColor(0xFFFF, _bgTitle);
  videoOut->printEfont(ntpTime.c_str(), _textOffset_x, _textOffset_y + 16 * 13, 1);  //NTP Clock
}

void Display::setNtpTime(String ntpTime) {
  _ntpTime = ntpTime;
}

void Display::update() {
#if defined(ENABLE_ANIMATION)
  if (gif.open((uint8_t *)earth, 409042, GIFDraw)) {
    while (gif.playFrame(true, NULL)) {
      switch (_message) {
        case MESSAGE::MSG_WRITE_BUFFER:
          displayWeatherInfo();
          _message = MESSAGE::MSG_DO_NOTHING;
          break;
        default:
          _message = MESSAGE::MSG_DO_NOTHING;
      }
      videoOut->waitForFrame();
    }
    videoOut->waitForFrame();
    gif.close();
  }
#else
  switch (_message) {
    case MESSAGE::MSG_WRITE_BUFFER:
      displayWeatherInfo();
      _message = MESSAGE::MSG_DO_NOTHING;
      break;
    default:
      _message = MESSAGE::MSG_DO_NOTHING;
  }
  videoOut->waitForFrame();
  delay(1);
#endif
}
