#include <Weather.h>
#include <esp32-hal-log.h>
#include <filter.h>

Weather::Weather() : _line("GET /bosai/forecast/data/forecast/__WEATHER_CODE__ HTTP/1.1\r\n"),
                     _host("Host: jma.go.jp"),
                     _user_agent("User-Agent: ESP32-PICO\r\n"),
                     _connection("Connection: close\r\n") {
  // ArduinoJson filter settings
  JsonObject filter          = _filter.createNestedObject();
  filter["publishingOffice"] = true;
  filter["reportDatetime"]   = true;

  JsonObject filter_timeSeries        = filter["timeSeries"].createNestedObject();
  filter_timeSeries["timeDefines"][0] = true;

  JsonObject filter_timeSeries_areas = filter_timeSeries["areas"].createNestedObject();

  JsonObject filter_timeSeries_areas_area         = filter_timeSeries_areas.createNestedObject("area");
  filter_timeSeries_areas_area["name"]            = true;
  filter_timeSeries_areas_area["code"]            = true;
  filter_timeSeries_areas_area["weatherCodes"][0] = true;
}

Weather::~Weather() {}

void Weather::begin(Client &client) {
  _client = &client;
}

String Weather::getForecast(uint16_t local_gov_code) {
  String code(local_gov_code);
  code.concat("0.json");
  _line.replace("__WEATHER_CODE__", code);

  _request = _line +
             _host +
             _user_agent +
             _connection + "\r\n";

  _client->print(_request);

  log_i("request\n %s", _request.c_str());

  unsigned long timeout = millis();

  while (_client->available() == 0) {
    if (millis() - timeout > 5000) {
      _client->stop();
      log_d(">>> Client Timeout ! and stop()\n");
      return "";
    }
  }

  while (_client->available() > 0) {
    _response += _client->readStringUntil('\r');
  }

  log_i("%s", _response.c_str());

  DeserializationError error = deserializeJson(_doc,
                                               _response.c_str(),
                                               _response.length(),
                                               DeserializationOption::Filter(_filter));

  if (error) {
    log_e("deserializeJson() failed: %s \n", error.f_str());
    return "";
  }

  return _response;
}

void Weather::update() {
}
