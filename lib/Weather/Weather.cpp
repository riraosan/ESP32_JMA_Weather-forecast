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

void Weather::begin(WiFiClient& client) {
  _wifiClient = client;
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

  _httpClient.setReuse(false);
  _httpClient.begin(_wifiClient, url);

  int httpCode = _httpClient.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      _response = _httpClient.getString();

      // log_i("Response\n%s", _response.c_str());

      DeserializationError error = deserializeJson(_doc,
                                                   _response.c_str(),
                                                   _response.length(),
                                                   DeserializationOption::Filter(_filter));

      if (error) {
        log_e("deserializeJson() failed: %s", error.f_str());
        _httpClient.end();
        return error.f_str();
      }

      JsonObject  root_0           = _doc[0];
      const char* publishingOffice = root_0["publishingOffice"];
      const char* reportDatetime   = root_0["reportDatetime"];

      log_i("publishingOffice %s", publishingOffice);
      log_i("reportDatetime   %s", reportDatetime);

      JsonArray   root_0_timeSeries = root_0["timeSeries"];
      const char* area_name         = root_0_timeSeries[0]["areas"][0]["area"]["name"];
      const char* area_code         = root_0_timeSeries[0]["areas"][0]["area"]["code"];
      log_i("area_name %s", area_name);
      log_i("area_code %s", area_code);

      JsonArray weatherCodes = root_0_timeSeries[0]["areas"][0]["weatherCodes"];
      JsonArray weathers     = root_0_timeSeries[0]["areas"][0]["weathers"];
      if (weatherCodes[0] != nullptr) {
        log_i("weatherCodes %s", (const char*)weatherCodes[0]);
        log_i("weatherCodes %s", (const char*)weatherCodes[1]);

        _todayForcast   = (const char*)weatherCodes[0];
        _nextdayForcast = (const char*)weatherCodes[1];
      }

      if (weathers[0] != nullptr) {
        log_i("weathers[0] %s", (const char*)weathers[0]);
        log_i("weathers[1] %s", (const char*)weathers[1]);
      }

      _httpClient.end();
      return "";
    }
  }

  String error = _httpClient.errorToString(httpCode);

  _httpClient.end();

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
