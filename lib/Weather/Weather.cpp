#include <Weather.h>
#include <esp32-hal-log.h>
#include <filter.h>

Weather::Weather() : _url("http://www.jma.go.jp/bosai/forecast/data/forecast/__WEATHER_CODE__0.json") {
  deserializeJson(_codeFilter, codeFilter);
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

      DeserializationError error = deserializeJson(_codeDoc,
                                                   _response.c_str(),
                                                   _response.length(),
                                                   DeserializationOption::Filter(_codeFilter));

      if (error) {
        log_e("deserializeJson() failed: %s", error.f_str());
        _httpClient.end();
        return error.f_str();
      }

      JsonObject root_0 = _codeDoc[0];
      if (!root_0.isNull()) {
        const char* publishingOffice = root_0["publishingOffice"];
        const char* reportDatetime   = root_0["reportDatetime"];

        log_i("publishingOffice  %s", publishingOffice);
        log_i("  reportDatetime  %s", reportDatetime);

        JsonArray root_0_timeSeries = root_0["timeSeries"];

        if (!root_0_timeSeries.isNull()) {
          const char* area_name = root_0_timeSeries[0]["areas"][0]["area"]["name"];
          const char* area_code = root_0_timeSeries[0]["areas"][0]["area"]["code"];

          JsonArray weatherCodes = root_0_timeSeries[0]["areas"][0]["weatherCodes"];
          JsonArray weathers     = root_0_timeSeries[0]["areas"][0]["weathers"];

          if (!weatherCodes.isNull() && !weathers.isNull()) {
            _areaName        = (const char*)area_name;
            _areaCode        = (const char*)area_code;
            _todayForecast   = (const char*)weatherCodes[0];
            _nextdayForecast = (const char*)weatherCodes[1];
            _weathers0       = (const char*)weathers[0];
            _weathers1       = (const char*)weathers[1];

            log_i("       area_name  %s", _areaName.c_str());
            log_i("       area_code  %s", _areaCode.c_str());
            log_i(" weatherCodes[0]  %s", _todayForecast.c_str());
            log_i(" weatherCodes[1]  %s", _nextdayForecast.c_str());
            log_i("     weathers[0]  %s", _weathers0.c_str());
            log_i("     weathers[1]  %s", _weathers1.c_str());
          }
        }
      }

      _httpClient.end();
      return "success";
    }
  }

  String error = _httpClient.errorToString(httpCode);

  _httpClient.end();

  log_e("[HTTP] GET... failed, error: %s", error.c_str());
  return error;
}

String Weather::getTodayForecast(void) {
  return _todayForecast;
}

String Weather::getNextdayForecast(void) {
  return _nextdayForecast;
}
String Weather::getForecastJp(void) {
  return _forecastJP;
}

String Weather::getForecastEn(void) {
  return _forecastEN;
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
