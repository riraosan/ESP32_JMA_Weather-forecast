
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <WeatherCode.h>

class Weather {
 public:
  Weather();

  void   begin(WiFiClient &client);
  void   setAreaCode(uint16_t localGovernmentCode);
  String getForecast(void);
  String getTodayForcast(void);
  String getNextdayForcast(void);
  void   update();

 private:
  String   _createURL(uint16_t localGovernmentCode);
  uint16_t _localGovernmentCode;

  String _request;
  String _response;
  String _url;

  String _areaName;
  String _areaCode;
  String _todayForcast;
  String _nextdayForcast;
  String _weathers0;
  String _weathers1;

  WiFiClient _wifiClient;
  HTTPClient _httpClient;

  StaticJsonDocument<400>  _filter;
  StaticJsonDocument<2500> _doc;
};
