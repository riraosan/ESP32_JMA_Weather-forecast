
#pragma once

#include <Arduino.h>
#include <secrets.h>
#include <WeatherCode.h>
#include <Connect.h>
#include <Display.h>
#include <Weather.h>
#include <Ticker.h>
#include <ThingSpeak.h>
#if defined(TS_ENABLE_SSL)
#include <WiFiClientSecure.h>
#else
//#include <WiFiClient.h>
#include <HTTPClient.h>
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

  static void sendMessage(MESSAGE message) {
    _message = message;
  }

  static void setNtpTime(void) {
    sendMessage(MESSAGE::MSG_CHECK_CLOCK);
  }

  static void dataCallback(void) {
    sendMessage(MESSAGE::MSG_CHECK_DATA);
  }

  static void forcastCallback(void) {
    sendMessage(MESSAGE::MSG_CHECK_FORCAST);
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
      _composite.sendMessage(MESSAGE::MSG_CHECK_DATA);
    } else {
      log_e("Problem reading channel. HTTP error code %d", _statusCode);
    }
  }

  void setWeatherForcast(void) {
    log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    String result(_weather.getForecast(LOCAL_GOV_CODE));

    if (result.isEmpty()) {
      String forcastcode(_weather.getTodayForcast());
      log_i("Success to get Forcast code: %s", forcastcode.c_str());

      _composite.sendMessage((MESSAGE)forcastcode.toInt());
    }
  }

  void setNtpClock(void) {
    char      buffer[16] = {0};
    struct tm info;
    if (getLocalTime(&info)) {
      sprintf(buffer, "%02d:%02d:%02d", info.tm_hour, info.tm_min, info.tm_sec);

      _ntpTime = buffer;

      _composite.setNtpTime(_ntpTime);
      _composite.sendMessage(MESSAGE::MSG_CHECK_DATA);
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

    ThingSpeak.begin(_wifiClient);
    _weather.begin(_wifiClient);

#if defined(TEST_PERIOD)
    _serverChecker.attach(20, dataCallback);
    _forcastChecker.attach(30, forcastCallback);
#else
    _serverChecker.attach(60 * 10, dataCallback);
    _forcastChecker.attach(60 * 60, forcastCallback);
#endif

    // delay(10000);
    beginNtpClock();

    _composite.begin(12, true, 16);
    sendMessage(MESSAGE::MSG_INIT_DATA);
    log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  }

  void update(void) {
    _composite.update();

    switch (_message) {
      case MESSAGE::MSG_CHECK_CLOCK:
        setNtpClock();

        sendMessage(MESSAGE::MSG_NOTHING);
        break;
      case MESSAGE::MSG_INIT_DATA:
        setInformation();

        sendMessage(MESSAGE::MSG_CHECK_FORCAST);
        break;
      case MESSAGE::MSG_CHECK_DATA:
        setInformation();

        sendMessage(MESSAGE::MSG_NOTHING);
        break;
      case MESSAGE::MSG_CHECK_FORCAST:
        configTzTime(TIME_ZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3);
        setWeatherForcast();

        sendMessage(MESSAGE::MSG_NOTHING);
        break;
      default:
        break;
    }
  }

 private:
  Connect _wifi;
  Display _composite;
  Weather _weather;
  Ticker  _serverChecker;
  Ticker  _ntpclocker;
  Ticker  _forcastChecker;
#if defined(TS_ENABLE_SSL)
  WiFiClientSecure _client;
#else
  WiFiClient _wifiClient;
#endif

  static MESSAGE _message;

#if defined(TS_ENABLE_SSL)
  const char* _certificate;
#endif
  unsigned long _weatherStationChannelNumber;
  int           _field[8];
  int           _statusCode;
  String        _ntpTime;
};

MESSAGE WeatherDisplay::_message = MESSAGE::MSG_NOTHING;
