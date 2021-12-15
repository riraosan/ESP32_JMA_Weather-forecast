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
#include <secrets.h>
#include <WeatherCode.h>
#include <Connect.h>
#include <Display.h>
#include <Weather.h>
#include <Ticker.h>
#include <ThingSpeak.h>
#include <HTTPClient.h>

class WeatherDisplay {
 public:
  WeatherDisplay() : _weatherStationChannelNumber(SECRET_CH_ID),
                     _field{1, 2, 3, 4, 5, 6, 7, 8},
                     _statusCode(0) {
  }

  static void sendMessage(MESSAGE message) {
    _message = message;
  }

  static void restart(void) {
    ESP.restart();
    delay(1000);
  }

  static void setNtpTime(void) {
    _clock = true;
  }

  static void dataCallback(void) {
    sendMessage(MESSAGE::MSG_CHECK_DATA);
  }

  static void forecastCallback(void) {
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
    char      buffer[16] = {0};
    struct tm info;

    if (getLocalTime(&info)) {
      sprintf(buffer, "%02d:%02d:%02d", info.tm_hour, info.tm_min, info.tm_sec);

      _ntpTime = buffer;

      _composite.setNtpTime(_ntpTime);
      _composite.sendMessage(MESSAGE::MSG_DISPLAY_CLOCK);
    }
    _clock = false;
  }

  void begin(void) {
    _wifi.setTaskName("AutoConnect");
    _wifi.setTaskSize(4096 * 1);
    _wifi.setTaskPriority(2);
    _wifi.setCore(0);
    _wifi.begin(SECRET_SSID, SECRET_PASS);
    _wifi.start(nullptr);

    ThingSpeak.begin(_wifiClient);

    _weather.setAreaCode(LOCAL_GOV_CODE);
    _weather.begin(_wifiClient);

#if defined(TEST_PERIOD)
    _serverChecker.attach(20, dataCallback);
    _forecastChecker.attach(30, forecastCallback);
#else
    _serverChecker.attach(60 * 10, dataCallback);
    _forecastChecker.attach(60 * 60, forecastCallback);
#endif

    _reboot.attach(60 * 60 * 12, restart);
    beginNtpClock();

    _composite.begin(12, true, 16);
    sendMessage(MESSAGE::MSG_INIT_DATA);
    log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  }

  void update(void) {
    if (_clock) {
      setNtpClock();
    }

    _composite.update();

    switch (_message) {
      case MESSAGE::MSG_INIT_DATA:

        setInformation();

        sendMessage(MESSAGE::MSG_CHECK_FORECAST);
        break;
      case MESSAGE::MSG_CHECK_DATA:

        setInformation();

        sendMessage(MESSAGE::MSG_NOTHING);
        break;
      case MESSAGE::MSG_CHECK_FORECAST:

        setWeatherForecast();

        sendMessage(MESSAGE::MSG_NOTHING);
        break;
      default:
        break;
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
