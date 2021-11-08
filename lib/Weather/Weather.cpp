#include <Weather.h>
#include <esp32-hal-log.h>
#include <filter.h>

Weather::Weather() : _url("http://www.jma.go.jp/bosai/forecast/data/forecast/__WEATHER_CODE__"),
                     _line("GET /bosai/forecast/data/forecast/__WEATHER_CODE__ HTTP/1.1\r\n"),
                     _host("www.jma.go.jp"),
                     _user_agent("User-Agent: ESP32-PICO\r\n"),
                     _connection("Connection: close\r\n") {
    deserializeJson(_filter, filter);
}

Weather::~Weather() {}

void Weather::begin(HTTPClient& client) {
  _client = &client;
}

String Weather::createRequest(uint16_t local_gov_code) {
  String code(local_gov_code);
  code += "0.json";
  _line.replace("__WEATHER_CODE__", code);

  _request = _line +
             "Host: " + _host + "\r\n" +
             _user_agent +
             _connection + "\r\n";

  log_i("%s", _request.c_str());

  return _request;
}
String Weather::_createURL(uint16_t local_gov_code) {
  String code(local_gov_code);
  code += "0.json";
  _url.replace("__WEATHER_CODE__", code);

  log_i("%s", _url.c_str());

  return _url;
}

String Weather::getForecast(uint16_t local_gov_code) {
  String url(_createURL(local_gov_code));

  _client->setReuse(false);
  _client->begin(url);

  int httpCode = _client->GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      _response = _client->getString();

      log_i("Response\n%s", _response.c_str());

      DeserializationError error = deserializeJson(_doc,
                                                   _response.c_str(),
                                                   _response.length(),
                                                   DeserializationOption::Filter(_filter));

      if (error) {
        log_e("deserializeJson() failed: %s", error.f_str());
        _client->end();
        return error.f_str();
      }

      JsonObject  root_0                  = _doc[0];
      const char* root_0_publishingOffice = root_0["publishingOffice"];
      const char* root_0_reportDatetime   = root_0["reportDatetime"];

      log_i("publishingOffice %s", root_0_publishingOffice);
      // log_i("reportDatetime   %s", root_0_reportDatetime);

      JsonArray root_0_timeSeries = root_0["timeSeries"];

      JsonArray   root_0_timeSeries_0_timeDefines   = root_0_timeSeries[0]["timeDefines"];
      const char* root_0_timeSeries_0_timeDefines_0 = root_0_timeSeries_0_timeDefines[0];
      const char* root_0_timeSeries_0_timeDefines_1 = root_0_timeSeries_0_timeDefines[1];
      const char* root_0_timeSeries_0_timeDefines_2 = root_0_timeSeries_0_timeDefines[2];

      // log_i("timeDefines_0 %s", root_0_timeSeries_0_timeDefines_0);
      // log_i("timeDefines_1 %s", root_0_timeSeries_0_timeDefines_1);
      // log_i("timeDefines_2 %s", root_0_timeSeries_0_timeDefines_2);

      const char* root_0_timeSeries_0_areas_0_area_name = root_0_timeSeries[0]["areas"][0]["area"]["name"];
      const char* root_0_timeSeries_0_areas_0_area_code = root_0_timeSeries[0]["areas"][0]["area"]["code"];

      // log_i("area_name %s", root_0_timeSeries_0_areas_0_area_name);
      // log_i("area_code %s", root_0_timeSeries_0_areas_0_area_code);

      JsonArray root_0_timeSeries_0_areas_0_weatherCodes = root_0_timeSeries[0]["areas"][0]["weatherCodes"];

      log_i("weatherCodes %s", (const char*)root_0_timeSeries_0_areas_0_weatherCodes[0]);
      log_i("weatherCodes %s", (const char*)root_0_timeSeries_0_areas_0_weatherCodes[1]);
      // log_i("weatherCodes %s", (const char*)root_0_timeSeries_0_areas_0_weatherCodes[2]);

      _todayForcast   = (const char*)root_0_timeSeries_0_areas_0_weatherCodes[0];
      _nextdayForcast = (const char*)root_0_timeSeries_0_areas_0_weatherCodes[1];

      _client->end();
      return _response;
    }
  }

  String error = _client->errorToString(httpCode);

  _client->end();

  log_e("[HTTP] GET... failed, error: %s", error.c_str());
  return error;
}

String Weather::getTodayForcast(void) {
  return _todayForcast;
}

String Weather::getNextdayForcast(void) {
  return _nextdayForcast;
}

void Weather::update() {
}
