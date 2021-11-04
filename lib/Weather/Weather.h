
#include <Arduino.h>
#include <Client.h>
#include <ArduinoJson.h>
#include <WeatherCode.h>

class Weather {
 public:
  Weather();
  ~Weather();

  void begin(Client &client);
  String getForecast(uint16_t local_gov_code);
  void   update();

 private:

  String _request;
  String _response;

  String _line;
  String _uri;
  String _host;
  String _user_agent;
  String _connection;

  String _areaCode;
  String _todayWCode;
  String _nextWCode;

  Client *_client;

  StaticJsonDocument<224>  _filter;
  StaticJsonDocument<1536> _doc;
};
