/**
 *
 *  @file   WeatherDisplay.h
 *  @author fiatpanda750@gmail.com
 *  @version    0.0.1
 *  @date   2021-11-30
 *  @copyright  MIT license.
 */

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
