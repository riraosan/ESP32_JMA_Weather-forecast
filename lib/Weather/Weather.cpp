
#include <memory>
#include <Weather.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <StreamUtils.h>
#include <esp32-hal-log.h>
#include <secrets.h>

Weather::Weather() : _url("https://www.jma.go.jp/bosai/forecast/data/forecast/__WEATHER_CODE__0.json"),
                     _codeDoc(6144) {
}

void Weather::begin(uint16_t localGovernmentCode) {
  setAreaCode(localGovernmentCode);

  if (!SPIFFS.begin()) {
    log_e("File system is not mounted");
  }
}

void Weather::setAreaCode(uint16_t localGovernmentCode) {
  _localGovernmentCode = localGovernmentCode;
}

String Weather::getJMAForecastJson(void) {
  _url.replace("__WEATHER_CODE__", String(_localGovernmentCode));

  std::unique_ptr<WiFiClientSecure> _jmaClient(new WiFiClientSecure);
  std::unique_ptr<HTTPClient>       _httpClient(new HTTPClient);

  _jmaClient->setCACert(jma_root_ca);
  _jmaClient->setHandshakeTimeout(180);
  _httpClient->setReuse(true);

  _httpClient->begin(*_jmaClient, _url.c_str());

  int httpCode = _httpClient->GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      ReadLoggingStream loggingStream(_httpClient->getStream(), Serial);

      StaticJsonDocument<500> codeFilter;
      deserializeJson(codeFilter, g_codeFilter);

      DeserializationError error = deserializeJson(_codeDoc,
                                                   loggingStream,
                                                   DeserializationOption::Filter(codeFilter));

      if (error) {
        log_e("deserializeJson() failed: %s", error.f_str());
        _httpClient->end();
        return error.f_str();
      }

      JsonObject root_0 = _codeDoc[0];  // 0 or 1
      if (!root_0.isNull()) {
        const char* publishingOffice = root_0["publishingOffice"];
        const char* reportDatetime   = root_0["reportDatetime"];

        _publishingOffice = (const char*)publishingOffice;
        _reportDatetime   = (const char*)reportDatetime;

        Serial.println("publishingOffice  " + _publishingOffice);
        Serial.println("  reportDatetime  " + _reportDatetime);

        JsonArray timeSeries = root_0["timeSeries"];

        if (!timeSeries.isNull()) {
          JsonObject  areas_0   = timeSeries[0]["areas"][0];
          const char* area_name = areas_0["area"]["name"];
          const char* area_code = areas_0["area"]["code"];

          JsonArray weatherCodes = areas_0["weatherCodes"];
          JsonArray weathers     = areas_0["weathers"];

          _areaName = (const char*)area_name;
          _areaCode = (const char*)area_code;
          Serial.println("       area_name  " + String((const char*)area_name));
          Serial.println("       area_code  " + String((const char*)area_code));

          if (!weatherCodes.isNull()) {
            _todayForecast   = (const char*)weatherCodes[0];
            _nextdayForecast = (const char*)weatherCodes[1];
            Serial.println(" weatherCodes[0]  " + String((const char*)weatherCodes[0]));
            Serial.println(" weatherCodes[1]  " + String((const char*)weatherCodes[1]));
            Serial.println(" weatherCodes[2]  " + String((const char*)weatherCodes[2]));
          }

          if (!weathers.isNull()) {
            _weathers0 = (const char*)weathers[0];
            _weathers1 = (const char*)weathers[1];

            Serial.println("     weathers[0]  " + String((const char*)weathers[0]));
            Serial.println("     weathers[1]  " + String((const char*)weathers[1]));
            Serial.println("     weathers[2]  " + String((const char*)weathers[2]));
          }
        }
      }
      _httpClient->end();
      return String("success");
    }
  }

  String error = _httpClient->errorToString(httpCode);
  _httpClient->end();

  log_e("[HTTP] GET... failed, error: %s", error.c_str());
  return error;
}

void Weather::makeJMAForecastString(void) {
  String filter(R"({"__CODE__": [true]})");
  filter.replace("__CODE__", _todayForecast);

  StaticJsonDocument<255> _forecastDoc;
  StaticJsonDocument<50>  _filter;
  deserializeJson(_filter, filter);

  File file = SPIFFS.open("/codes.json");

  if (file) {
    DeserializationError error = deserializeJson(_forecastDoc,
                                                 file,
                                                 DeserializationOption::Filter(_filter));

    if (error) {
      log_e("Failed to read codes.json.");
      file.close();
      return;
    }

    JsonArray root = _forecastDoc[_todayForecast.c_str()];

    if (!root.isNull()) {
      _iconFile   = String("/") + String((const char*)root[0]);  // "100.gif"
      _forecastJP = (const char*)root[3];                        // "晴"
      _forecastEN = (const char*)root[4];                        // "CLEAR"

      log_d("%s_%s_%s", _iconFile.c_str(), _forecastJP.c_str(), _forecastEN.c_str());
    }
  }

  file.close();
}

// weather code
String Weather::getTodayForecast(void) {
  return _todayForecast;
}

// weather code
String Weather::getNextdayForecast(void) {
  return _nextdayForecast;
}

String Weather::getWeathersJp(void) {
  return _forecastJP;
}

String Weather::getWeathersEn(void) {
  return _forecastEN;
}

String Weather::getICONFilename(void) {
  return _iconFile;
}
