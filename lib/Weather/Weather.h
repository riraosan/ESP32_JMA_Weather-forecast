
#include <Arduino.h>
#include <WeatherCode.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

// R"()" = Raw String Literals(C++)
constexpr char g_codeFilter[] = R"(
[
  {
    "publishingOffice": true,
    "reportDatetime": true,
    "timeSeries": [
      {
        "timeDefines": true,
        "areas": [
          {
            "area": true,
            "weatherCodes": true,
            "weathers": true,
            "winds": false,
            "waves": false
          }
        ]
      }
    ]
  }
])";

class Weather {
public:
  Weather();

  void   begin(uint16_t localGovernmentCode);
  void   setAreaCode(uint16_t localGovernmentCode);
  String getJMAForecastJson(void);
  void   makeJMAForecastString(void);
  String getTodayForecast(void);
  String getNextdayForecast(void);
  String getWeathersJp(void);
  String getWeathersEn(void);
  String getICONFilename(void);
  void   update();

private:
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
  String _publishingOffice;
  String _reportDatetime;

  DynamicJsonDocument _codeDoc;
};
