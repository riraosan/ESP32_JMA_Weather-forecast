/**
 *
 *  @file       WeatherDisplay.h
 *  @author     @riraosan
 *  @version    0.0.1
 *  @date       2021-11-20
 *  @copyright  MIT license.
 */

#pragma once

#include <Arduino.h>
#include <Connect.h>
#include <Display.h>
#include <HTTPClient.h>
#include <ThingSpeak.h>
#include <Ticker.h>
#include <Weather.h>
#include <WeatherCode.h>
#include <secrets.h>

class WeatherDisplay {
 public:
  WeatherDisplay() : _weatherStationChannelNumber(SECRET_CH_ID),
                     _field{1, 2, 3, 4, 5, 6, 7, 8},
                     _statusCode(0) {
  }

  static void sendMessage(MESSAGE message) {
    _message = message;
  }

  static void setNtpTime(void) {
    _clock = true;
  }

  static void timerCallback(void) {
    sendMessage(MESSAGE::MSG_CHECK_FORECAST);
  }

  void beginNtpClock(void) {
    configTzTime(TIME_ZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
    _ntpclocker.attach_ms(500, setNtpTime);
  }

  void setInformation(void) {
    log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

    _statusCode = ThingSpeak.readMultipleFields(_weatherStationChannelNumber);

    if (_statusCode == 200) {
      // Fetch the stored data
      log_i("Fetch the stored data. code %d", _statusCode);

      float  temperature = ThingSpeak.getFieldAsFloat(_field[0]);  // Field 1
      float  humidity    = ThingSpeak.getFieldAsFloat(_field[1]);  // Field 2
      float  pressure    = ThingSpeak.getFieldAsFloat(_field[2]);  // Field 3
      String createdAt   = ThingSpeak.getCreatedAt();              // Created-at timestamp

      log_i("[%s] %2.1f*C, %2.1f%%, %4.1fhPa", createdAt.c_str(), temperature, humidity, pressure);

      _composite.setWeatherInfo(temperature, humidity, pressure, createdAt);
      _composite.sendMessage(MESSAGE::MSG_DISPLAY_DATA);
    } else {
      log_e("Problem reading channel. HTTP error code %d", _statusCode);
    }
  }

  void setWeatherForecast(void) {
    log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    String result();

    if (_weather.getJMAForecast() == "success") {
      log_i("Success to get Forecast code");

      _weather.getJMAWeathers();

      _composite.setWeatherForecast(_weather.getICONFilename(), _weather.getWeathersJp(), _weather.getWeathersEn());
      _composite.sendMessage(MESSAGE::MSG_DISPLAY_FORECAST);
    }
  }

  void setNtpClock(void) {
    char        time[16] = {0};
    char        ymd[16]  = {0};
    const char *wday[]   = {"Sun.", "Mon.", "Tue.", "Wed.", "Thu.", "Fri.", "Sat."};

    struct tm info;

    if (getLocalTime(&info)) {
      sprintf(time, "%02d:%02d:%02d", info.tm_hour, info.tm_min, info.tm_sec);
      sprintf(ymd, "%s %02d %02d %04d", wday[info.tm_wday], info.tm_mon + 1, info.tm_mday, info.tm_year + 1900);

      if (info.tm_hour == 23 && info.tm_min == 59 && info.tm_sec == 59) {
        ESP.restart();
        delay(1000);
        return;
      }

      _composite.setYMD(String(ymd));
      _composite.setNtpTime(String(time));
      _composite.sendMessage(MESSAGE::MSG_DISPLAY_CLOCK);
    }
    _clock = false;
  }

  void begin(void) {
    _wifi.setTaskName("AutoConnect");
    _wifi.setTaskSize(4096 * 1);
    _wifi.setTaskPriority(2);
    _wifi.setCore(0);
#if defined(TEST_PERIOD)
    _wifi.begin(SECRET_SSID, SECRET_PASS);
#else
    _wifi.begin();
#endif
    _wifi.start(nullptr);

    ThingSpeak.begin(_wifiClient);

    _weather.setAreaCode(LOCAL_GOV_CODE);
    _weather.begin(_wifiClient);

#if defined(TEST_PERIOD)
    _serverChecker.attach(30, timerCallback);
#else
    _serverChecker.attach(60 * 15, timerCallback);  // every 15min
#endif

    beginNtpClock();
    _composite.begin(12, true, 16);

    timerCallback();
    log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  }

  void update(void) {
    switch (_message) {
      case MESSAGE::MSG_CHECK_DATA:
        setInformation();

        sendMessage(MESSAGE::MSG_NOTHING);
        break;
      case MESSAGE::MSG_CHECK_FORECAST:
        setWeatherForecast();

        sendMessage(MESSAGE::MSG_CHECK_DATA);
        break;
      default:
        break;
    }

    _composite.update();

    if (_clock) {
      setNtpClock();
      _composite.update();
    }

    delay(1);
  }

 private:
  WiFiClient _wifiClient;
  Ticker     _serverChecker;
  Ticker     _ntpclocker;
  Ticker     _forecastChecker;
  Ticker     _reboot;

  Connect _wifi;
  Display _composite;
  Weather _weather;

  unsigned long _weatherStationChannelNumber;
  int           _field[8];
  int           _statusCode;
  String        _ntpTime;

  static MESSAGE _message;
  static bool    _clock;
};

MESSAGE WeatherDisplay::_message = MESSAGE::MSG_NOTHING;
bool    WeatherDisplay::_clock   = false;
