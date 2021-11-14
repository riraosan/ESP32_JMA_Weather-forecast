// sample
#include <WeatherDisplay.h>

static WeatherDisplay app;

// CORE1
void setup() {
  app.begin();
}

void loop() {
  app.update();
  delay(1);
}
