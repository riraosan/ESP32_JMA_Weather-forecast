
#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Task.h>

#if defined(ARDUINO_ARCH_ESP32)
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <AutoConnect.h>
using WiFiWebServer = WebServer;
#else
#error Only for ESP32
#endif

class Connect : public Task {
 public:
  Connect();
  void begin(void);
  void begin(const char* SSID, const char* PASSWORD);
  void startWiFi(void);

 private:
  void run(void* data);

  WiFiWebServer     _server;
  AutoConnectConfig _config;
  AutoConnect       _portal;

  String   _content;
  String   _hostName;
  String   _apName;
  uint16_t _httpPort;
};
