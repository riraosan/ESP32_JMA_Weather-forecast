#include <Weather.h>
#include <esp32-hal-log.h>
#include <filter.h>

Weather::Weather() : _url("http://www.jma.go.jp/bosai/forecast/data/forecast/__WEATHER_CODE__0.json") {
  deserializeJson(_filter, filter);
}

void Weather::begin(WiFiClient& client) {
  _wifiClient = client;
}

void Weather::setAreaCode(uint16_t localGovernmentCode) {
  _localGovernmentCode = localGovernmentCode;
}

String Weather::getForecast(void) {
  String url;

  if (_localGovernmentCode) {
    url = _createURL(_localGovernmentCode);
  } else {
    return url.c_str();
  }

  _httpClient.setReuse(true);
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

      log_i("publishingOffice  %s", publishingOffice);
      log_i("  reportDatetime  %s", reportDatetime);

      JsonArray   root_0_timeSeries = root_0["timeSeries"];
      const char* area_name         = root_0_timeSeries[0]["areas"][0]["area"]["name"];
      const char* area_code         = root_0_timeSeries[0]["areas"][0]["area"]["code"];

      JsonArray weatherCodes = root_0_timeSeries[0]["areas"][0]["weatherCodes"];
      JsonArray weathers     = root_0_timeSeries[0]["areas"][0]["weathers"];

      if (root_0_timeSeries[0] != nullptr) {
        _areaName       = (const char*)area_name;
        _areaCode       = (const char*)area_code;
        _todayForcast   = (const char*)weatherCodes[0];
        _nextdayForcast = (const char*)weatherCodes[1];
        _weathers0      = (const char*)weathers[0];
        _weathers1      = (const char*)weathers[1];

        log_i("       area_name  %s", _areaName.c_str());
        log_i("       area_code  %s", _areaCode.c_str());
        log_i(" weatherCodes[0]  %s", _todayForcast.c_str());
        log_i(" weatherCodes[1]  %s", _nextdayForcast.c_str());
        log_i("     weathers[0]  %s", _weathers0.c_str());
        log_i("     weathers[1]  %s", _weathers1.c_str());
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

String Weather::_createURL(uint16_t localGovernmentCode) {
  String code(localGovernmentCode);
  _url.replace("__WEATHER_CODE__", code);

  log_d("%s", _url.c_str());
  return _url;
}

void Weather::update() {
  delay(1);
}
