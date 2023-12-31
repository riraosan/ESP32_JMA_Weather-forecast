
#include <Display.h>
#include <esp32-hal-log.h>

MESSAGE Display::_message = MESSAGE::MSG_NOTHING;

LGFX_8BIT_CVBS Display::_display;
M5Canvas       Display::_animation;

File Display::_file;

int Display::_width  = 0;
int Display::_height = 0;

Display::Display() : _temperature(0.0),
                     _humidity(0.0),
                     _pressure(0.0),
                     _ntpTime(""),
                     _ymd(""),
                     _daytimeFormat("__YMD__ __NTP__"),
                     _bgColor(0x10cd),
                     _bgTitle(0x0019),
                     _bgTemperature(0x1c43),
                     _bgPressure(0x1c43),
                     _bgHumidity(0x1c43),
                     _filename(""),
                     _IllustrationName("/job_kisyou_yohou.gif") {
}

void Display::sendMessage(MESSAGE message) {
  _message = message;
}

void Display::setNtpTime(String ntpTime) {
  _ntpTime = ntpTime;
}

void Display::setYMD(String ymd) {
  _ymd = ymd;
}

void Display::setWeatherForecast(String filename, String forecastJP, String forecastEN) {
  _filename   = filename;
  _forecastJP = forecastJP;
  _forecastEN = forecastEN;

  log_d("%s, %s, %s", _filename.c_str(), _forecastJP.c_str(), _forecastEN.c_str());
}

void Display::begin(void) {
  _display.begin();

  _display.startWrite();

  _display.fillScreen(_bgColor);
  _display.setRotation(0);
  _display.setColorDepth(8);

  //_animation.setPsram(true);
  _animation.setColorDepth(8);

  _data.setFont(&fonts::lgfxJapanGothic_16);
  _data.setTextWrap(true, true);
  _data.setPsram(false);
  _data.setColorDepth(8);

  _title.setFont(&fonts::lgfxJapanGothic_16);
  _title.setTextWrap(true, true);
  _title.setPsram(false);
  _title.setColorDepth(8);
}

void Display::setWeatherInfo(float temperature, float humidity, float pressure, String time) {
  _temperature = temperature;
  _humidity    = humidity;
  _pressure    = pressure;
}

void Display::displayWeatherInfo(void) {
  if (!_data.createSprite(320, 96)) {
    log_e("data allocation failed");
    return;
  }

  char tempe[10] = {0};
  char humid[10] = {0};
  char press[10] = {0};

  sprintf(tempe, "%4.1f", _temperature);
  sprintf(humid, "%4.1f", _humidity);
  sprintf(press, "%4.1f", _pressure);

  _data.fillSprite(_bgColor);

  // 予報（日本語）
  _data.setCursor(0, 16 * 0);
  _data.setTextColor(0xFFFF, _bgColor);
  _data.setTextSize(2);
  _data.print(" ");
  _data.print(_forecastJP.c_str());

  // 予報（英語）
  _data.setCursor(0, 16 * 2);
  _data.setTextColor(0xFFFF, _bgColor);
  _data.setTextSize(1);
  _data.print("");
  _data.print(_forecastEN.c_str());

  // 気温
  _data.setCursor(0, 16 * 3);
  _data.setTextColor(0xFFFF, _bgTemperature);
  _data.setTextSize(1);
  _data.print("Temperature:");
  _data.print(tempe);
  _data.print("*C   ");

  // 湿度
  _data.setCursor(0, 16 * 4);
  _data.setTextColor(0xFFFF, _bgHumidity);
  _data.setTextSize(1);
  _data.print("   Humidity:");
  _data.print(humid);
  _data.print("%    ");

  // 大気圧
  _data.setCursor(0, 16 * 5);
  _data.setTextColor(0xFFFF, _bgPressure);
  _data.setTextSize(1);
  _data.print("   Pressure:");
  _data.print(press);
  _data.print("hPa");

  log_d("%2.1f*C, %2.1f%%, %4.1fhPa", _temperature, _humidity, _pressure);

  _data.pushSprite(&_display, 3, 125, _bgColor);
  _data.deleteSprite();
}

void Display::update() {
  String format;
  switch (_message) {
    case MESSAGE::MSG_DISPLAY_CLOCK:
      if (!_title.createSprite(320, 32)) {
        log_e("title allocation failed");
        return;
      }

      _title.fillSprite(_bgTitle);
      // Title
      _title.setTextColor(0xFFFF, _bgTitle);
      _title.setTextSize(1);
      _title.setCursor(0, 16 * 0);
      _title.print(" Osaka Weather Station");

      format = _daytimeFormat;
      format.replace("__NTP__", _ntpTime);
      format.replace("__YMD__", _ymd);
      _title.setCursor(96, 16 * 1);
      _title.print(format.c_str());

      _title.pushSprite(&_display, 2, 0);
      _title.deleteSprite();

      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    case MESSAGE::MSG_DISPLAY_DATA:
      displayWeatherInfo();

      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    case MESSAGE::MSG_DISPLAY_FORECAST:
      displayIllustration();
      displayIcon();

      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    default:
      break;
  }

  _display.display();

  delay(1);
}

void Display::displayIcon(void) {
  log_d("%s, %s, %s", _filename.c_str(), _forecastJP.c_str(), _forecastEN.c_str());

  _width  = 120;
  _height = 120;

  if (!_animation.createSprite(_width, _height)) {
    log_e("image allocation failed");
    return;
  }

  _animation.fillSprite(_bgColor);
  if (_gif.open(_filename.c_str(), _GIFOpenFile, _GIFCloseFile, _GIFReadFile, _GIFSeekFile, _GIFDraw)) {
    log_d("success to open %s", _filename.c_str());
    _gif.playFrame(true, NULL);
  } else {
    log_e("failure to open %s", _filename.c_str());
  }

  _gif.close();
  _gif.reset();

  _animation.pushSprite(&_display, 20, 40);
  _animation.deleteSprite();
}

void Display::displayIllustration(void) {
  _width  = 140;
  _height = 160;

  if (!_animation.createSprite(_width, _height)) {
    log_e("image allocation failed");
    return;
  }

  _animation.fillSprite(_bgColor);
  if (_gif.open(_IllustrationName.c_str(), _GIFOpenFile, _GIFCloseFile, _GIFReadFile, _GIFSeekFile, _GIFDraw)) {
    log_d("success to open %s", _IllustrationName.c_str());
    _gif.playFrame(true, NULL);
  } else {
    log_e("failure to open %s", _IllustrationName.c_str());
  }

  _gif.close();
  _gif.reset();

  _animation.pushSprite(&_display, 180, 50);
  _animation.deleteSprite();
}

void *Display::_GIFOpenFile(const char *fname, int32_t *pSize) {
  _file = SPIFFS.open(fname);

  if (_file) {
    *pSize = _file.size();
    return (void *)&_file;
  }

  return NULL;
}

void Display::_GIFCloseFile(void *pHandle) {
  File *f = static_cast<File *>(pHandle);

  if (f != NULL)
    f->close();
}

int32_t Display::_GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
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

int32_t Display::_GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
  int   i = micros();
  File *f = static_cast<File *>(pFile->fHandle);

  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i           = micros() - i;
  //  Serial.printf("Seek time = %d us\n", i);
  return pFile->iPos;
}

void Display::_GIFDraw(GIFDRAW *pDraw) {
  uint8_t  *s;
  uint16_t *d, *usPalette, usTemp[320];
  int       x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > _width)
    iWidth = _width;

  usPalette = pDraw->pPalette;
  y         = pDraw->iY + pDraw->y;  // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2)  // restore to background color
  {
    for (x = 0; x < iWidth; x++) {
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
    pEnd   = s + iWidth;
    x      = 0;
    iCount = 0;  // count non-transparent pixels
    while (x < iWidth) {
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
      }              // while looking for opaque pixels
      if (iCount) {  // any opaque pixels?
        for (int xOffset = 0; xOffset < iCount; xOffset++) {
          _animation.drawPixel(x + xOffset, y, usTemp[xOffset]);
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
      _animation.drawPixel(x, y, usPalette[*s++]);
    }
  }
}
