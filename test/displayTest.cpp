
#include <Display.h>

static Display _disp;

void display_test_000(void){
  _disp.begin();
}

void display_test_001(void) {
  _disp.setYMD("test");
  _disp.setNtpTime("00:00:00");
  _disp.sendMessage(MESSAGE::MSG_DISPLAY_CLOCK);
  _disp.update();
}

void display_test_002(void) {
  _disp.setWeatherInfo(11.1, 22.2, 33.3, "00:00:00");
  _disp.sendMessage(MESSAGE::MSG_DISPLAY_DATA);

  _disp.update();
}

void display_test_003(void) {
  _disp.setWeatherForecast("/101.gif", "晴れ", "CLEAR");
  _disp.sendMessage(MESSAGE::MSG_DISPLAY_FORECAST);

  _disp.update();
}
