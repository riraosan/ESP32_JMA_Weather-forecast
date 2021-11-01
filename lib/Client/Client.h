
#include <Arduino.h>
#include <WiFiClient.h>

class Weather {
 public:
  Weather() {}
  ~Weather() {}



 private:
  WiFiClient *_client;
};
