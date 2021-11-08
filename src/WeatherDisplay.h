
#pragma once

#include <Arduino.h>
#include <secrets.h>
#include <Connect.h>
#include <Display.h>
#include <Weather.h>

#include <Ticker.h>
#include <ThingSpeak.h>
#if defined(TS_ENABLE_SSL)
#include <WiFiClientSecure.h>
#else
#include <WiFiClient.h>
#endif

class WeatherDisplay {
 public:
  WeatherDisplay() :
#if defined(TS_ENABLE_SSL)
                     _certificate(SECRET_TS_ROOT_CA),
#endif
                     _weatherStationChannelNumber(SECRET_CH_ID),
                     _field{1, 2, 3, 4, 5, 6, 7, 8},
                     _statusCode() {
  }

  static void setNtpTime(void) {
    _ntpClock = true;
  }

  void beginNtpClock(void) {
    configTzTime(TIME_ZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
    _ntpclocker.attach_ms(500, setNtpTime);
  }

  static void timerCallback(void) {
    _timer = true;
  }

  void getInformation(void) {
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
      _composite.sendMessage(MESSAGE::MSG_WRITE_DATA);
    } else {
      log_e("Problem reading channel. HTTP error code %d", _statusCode);
    }
  }

  void begin(void) {
    _wifi.setTaskName("AutoConnect");
    _wifi.setTaskSize(4096 * 1);
    _wifi.setTaskPriority(2);
    _wifi.setCore(0);
    _wifi.begin();
    _wifi.start(nullptr);

#if defined(TS_ENABLE_SSL)
    _client.setCACert(_certificate);
#endif

    // ThingSpeak.begin(_client);
    _weather.begin(_client);

#if defined(TEST_PERIOD)
    _serverChecker.attach(30, timerCallback);
#else
    _serverChecker.attach(60 * 10, timerCallback);
#endif

    beginNtpClock();

    delay(5000);
    setNtpTime();
    timerCallback();

    _composite.begin(12, true, 16);

    log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  }

  void update(void) {
    _composite.update();
    if (_timer) {
      // getInformation();
      _timer = false;
    }

    if (_ntpClock) {
      time_t     t  = time(NULL);
      struct tm* tm = localtime(&t);

      char buffer[16] = {0};
      sprintf(buffer, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

      _ntpTime = buffer;

      _composite.setNtpTime(buffer);
      _composite.sendMessage(MESSAGE::MSG_WRITE_DATA);
      _ntpClock = false;
    }
  }

 private:
  Connect _wifi;
  Display _composite;
  Weather _weather;
  Ticker  _serverChecker;
  Ticker  _ntpclocker;
  Ticker  _weatherChecker;
#if defined(TS_ENABLE_SSL)
  WiFiClientSecure _client;
#else
  WiFiClient _client;
#endif

  static bool _timer;
  static bool _ntpClock;

#if defined(TS_ENABLE_SSL)
  const char* _certificate;
#endif
  unsigned long _weatherStationChannelNumber;
  int           _field[8];
  int           _statusCode;
  String        _ntpTime;
};

bool WeatherDisplay::_timer    = false;
bool WeatherDisplay::_ntpClock = false;
