#include <Arduino.h>
#include <Connect.h>
#include <secrets.h>
#include <esp32-hal-log.h>

Connect::Connect() : _portal(_server),
                     _hostName(F("atom_display")),
                     _apName(F("ATOM_DISP-G-AP")),
                     _httpPort(80) {
  _content = String(R"(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>

Place the root page with the sketch application.&ensp; __AC_LINK__

</body>
</html>)");
}

void Connect::startWiFi(void) {
  setTaskName("AutoConnect");
  setTaskSize(4096 * 1);
  setTaskPriority(2);
  setCore(0);
#if defined(TEST_PERIOD)
  begin(SECRET_SSID, SECRET_PASS);
#else
  begin();
#endif
  start(nullptr);
}

void Connect::begin(void) {
  begin("", "");
}

void Connect::begin(const char* SSID, const char* PASSWORD) {
  _server.on("/", [&]() {
    _content.replace("__AC_LINK__", String(AUTOCONNECT_LINK(COG_16)));
    _server.send(200, "text/html", _content);
  });

  _config.autoReconnect = true;
  _config.ota           = AC_OTA_BUILTIN;
  _config.apid          = _apName;

  _portal.config(_config);

  bool result = false;

  if (String(SSID).isEmpty() || String(PASSWORD).isEmpty()) {
    result = _portal.begin();
  } else {
    result = _portal.begin(SSID, PASSWORD);
  }

  if (result) {
    log_i("WiFi connected: %s", WiFi.localIP().toString().c_str());

    if (MDNS.begin(_hostName.c_str())) {
      MDNS.addService("http", "tcp", _httpPort);
      log_i("HTTP Server ready! Open http://%s.local/ in your browser\n", _hostName.c_str());
    } else
      log_e("Error setting up MDNS responder");
  } else {
    log_e("ESP32 can't connect to AP.");
    ESP.restart();
  }
}

void Connect::run(void* data) {
  for (;;) {
    _portal.handleClient();
    delay(1);
  }
}
