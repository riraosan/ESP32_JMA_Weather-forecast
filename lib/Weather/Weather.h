
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <WeatherCode.h>

class Weather {
 public:
  Weather();
  ~Weather();

  void   begin(WiFiClient &client);
  String getForecast(uint16_t local_gov_code);
  String createRequest(uint16_t local_gov_code);
  String getTodayForcast(void);
  String getNextdayForcast(void);
  void   update();

 private:
  String _createRequest(uint16_t local_gov_code);
  String _createURL(uint16_t local_gov_code);

  String _request;
  String _response;

  String _url;
  String _line;
  String _host;
  String _user_agent;
  String _connection;

  String _areaCode;
  String _todayForcast;
  String _nextdayForcast;

  WiFiClient _wifiClient;
  HTTPClient _httpClient;

  StaticJsonDocument<400>  _filter;
  StaticJsonDocument<2500> _doc;
};
