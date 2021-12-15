
#include <Arduino.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <WeatherCode.h>

class Weather {
 public:
  Weather();

  void   begin(WiFiClient& client);
  void   setAreaCode(uint16_t localGovernmentCode);
  String getJMAForecast(void);
  void   getJMAWeathers(void);
  String getTodayForecast(void);
  String getNextdayForecast(void);
  String getWeathersJp(void);
  String getWeathersEn(void);
  String getICONFilename(void);
  void   update();

 private:
  String   _createURL(uint16_t localGovernmentCode);
  uint16_t _localGovernmentCode;

  String _request;
  String _response;
  String _url;

  String _areaName;
  String _areaCode;
  String _todayForecast;
  String _nextdayForecast;
  String _weathers0;
  String _weathers1;
  String _forecastJP;
  String _forecastEN;
  String _iconFile;

  WiFiClient _wifiClient;
  HTTPClient _httpClient;

  StaticJsonDocument<3000> _codeDoc;
  StaticJsonDocument<400>  _codeFilter;

  StaticJsonDocument<255> _forecastDoc;
  StaticJsonDocument<100> _forecastFilter;
};
