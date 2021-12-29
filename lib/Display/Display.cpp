
#include <Display.h>
#include <esp32-hal-log.h>

MESSAGE Display::_message = MESSAGE::MSG_NOTHING;

File Display::_file;

int16_t Display::_textOffset_x = 5;
int16_t Display::_textOffset_y = 10;
int16_t Display::_gifOffset_x  = 5;
int16_t Display::_gifOffset_y  = 40;

std::unique_ptr<ESP_8_BIT_GFX> Display::videoOut;

Display::Display() : _temperature(0.0),
                     _humidity(0.0),
                     _pressure(0.0),
                     _ntpTime(""),
                     _ymd(""),
                     _daytimeFormat("      __YMD__ __NTP__ "),
                     _bgColor(0x1122),
                     _bgTitle(0x0019),
                     _bgTemperature(0x1c43),
                     _bgPressure(0x1c43),
                     _bgHumidity(0x1c43),
                     _filename(""),
                     _IllustrationName("/job_kisyou_yohou.gif") {
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

void Display::setYMD(String ymd) {
  _ymd = ymd;
}

void Display::setWeatherForecast(String filename, String forecastJP, String forecastEN) {
  _filename   = filename;
  _forecastJP = forecastJP;
  _forecastEN = forecastEN;

  log_d("%s, %s, %s", _filename.c_str(), _forecastJP.c_str(), _forecastEN.c_str());
}

void Display::begin(uint16_t irPin, bool ntsc, uint8_t colorDepth) {
  videoOut.reset(new ESP_8_BIT_GFX(ntsc, colorDepth));

  videoOut->begin();
  videoOut->copyAfterSwap = true;  // gif library depends on data from previous buffer
  videoOut->fillScreen(_bgColor);
  videoOut->waitForFrame();

  if (!SPIFFS.begin()) {
    log_e("FILESYSTEM Mount Failed");
    return;
  }

  sendMessage(MESSAGE::MSG_CHECK_DATA);
}

void Display::setWeatherInfo(float temperature, float humidity, float pressure, String time) {
  _temperature = temperature;
  _humidity    = humidity;
  _pressure    = pressure;
}

void Display::displayWeatherInfo(void) {
  // Title
  videoOut->setTextColor(0xFFFF, _bgTitle);
  videoOut->printEfont(" Osaka Weather Station        ", _textOffset_x - 2, _textOffset_y + 16 * 0, 1);

  char tempe[10] = {0};
  char humid[10] = {0};
  char press[10] = {0};

  sprintf(tempe, "%6.1f", _temperature);
  sprintf(humid, "%6.1f", _humidity);
  sprintf(press, "%6.1f", _pressure);

  // 予報（日本語）
  videoOut->setCursor(_textOffset_x + 32, _textOffset_y + 16 * 7);
  videoOut->setTextColor(0xFFFF, _bgColor);
  videoOut->setTextSize(2);
  videoOut->printEfont(_forecastJP.c_str());

  // 予報（英語）
  videoOut->setCursor(_textOffset_x + 32, _textOffset_y + 16 * 10);
  videoOut->setTextColor(0xFFFF, _bgColor);
  videoOut->setTextSize(1);
  videoOut->printEfont(_forecastEN.c_str());

  // 気温
  videoOut->setCursor(_textOffset_x + 8 * 1 - 2, _textOffset_y + 16 * 11);
  videoOut->setTextColor(0xFFFF, _bgTemperature);
  videoOut->setTextSize(1);
  videoOut->printEfont("Temperature:");
  videoOut->printEfont(tempe);
  videoOut->printEfont("*C ");

  // 湿度
  videoOut->setCursor(_textOffset_x + 8 * 1 - 2, _textOffset_y + 16 * 12);
  videoOut->setTextColor(0xFFFF, _bgHumidity);
  videoOut->setTextSize(1);
  videoOut->printEfont("   Humidity:");
  videoOut->printEfont(humid);
  videoOut->printEfont("%  ");

  // 大気圧
  videoOut->setCursor(_textOffset_x + 8 * 1 - 2, _textOffset_y + 16 * 13);
  videoOut->setTextColor(0xFFFF, _bgPressure);
  videoOut->setTextSize(1);
  videoOut->printEfont("   Pressure:");
  videoOut->printEfont(press);
  videoOut->printEfont("hPa");

  log_d("%2.1f*C, %2.1f%%, %4.1fhPa", _temperature, _humidity, _pressure);
}

void Display::update() {
  String format;
  switch (_message) {
    case MESSAGE::MSG_DISPLAY_CLOCK:
      format = _daytimeFormat;
      format.replace("__NTP__", _ntpTime);
      format.replace("__YMD__", _ymd);
      videoOut->setTextColor(0xFFFF, _bgTitle);
      videoOut->printEfont(format.c_str(), _textOffset_x - 2, _textOffset_y + 16 * 1, 1);

      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    case MESSAGE::MSG_DISPLAY_DATA:
      displayWeatherInfo();

      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    case MESSAGE::MSG_DISPLAY_FORECAST:
      videoOut->fillScreen(_bgColor);
      displayIllustration();

      sendMessage(MESSAGE::MSG_NOTHING);
      break;
    default:
      break;
  }

  videoOut->waitForFrame();
  delay(1);
}

void Display::displayIllustration(void) {
  _gifOffset_x = 110;
  _gifOffset_y = 42;

  if (gif.open(_IllustrationName.c_str(), _GIFOpenFile, _GIFCloseFile, _GIFReadFile, _GIFSeekFile, _GIFDraw)) {
    log_d("success to open %s", _IllustrationName.c_str());
    gif.playFrame(true, NULL);
  } else {
    log_e("failure to open %s", _IllustrationName.c_str());
  }

  gif.close();

  _gifOffset_x = 5;
  _gifOffset_y = 42;

  log_d("%s, %s, %s", _filename.c_str(), _forecastJP.c_str(), _forecastEN.c_str());

  if (gif.open(_filename.c_str(), _GIFOpenFile, _GIFCloseFile, _GIFReadFile, _GIFSeekFile, _GIFDraw)) {
    log_d("success to open %s", _filename.c_str());
    gif.playFrame(true, NULL);
  } else {
    log_e("failure to open %s", _filename.c_str());
  }

  gif.close();
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
          videoOut->drawPixel(pDraw->iX + x + xOffset + _gifOffset_x, y + _gifOffset_y, usTemp[xOffset]);
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
      videoOut->drawPixel(x + _textOffset_x, y + _textOffset_y, usPalette[*s++]);
    }
  }
}
