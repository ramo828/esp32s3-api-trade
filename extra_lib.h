#ifndef EXTRA_LIB
#define EXTRA_LIB
#include "my_wifi.h"
#include "display_lib.h"
#include <Arduino.h>

class Extra {
public:
  Extra();
  void wifi_setup();
  void beep();
  void memoryInfo();
  String jsonExtract(String data, String key, int limit_data=9);
private:
  Display display = Display();
  MyWifi wifi = MyWifi();
  #define BUZZER 7
};
#endif