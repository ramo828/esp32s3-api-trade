#include "extra_lib.h"
#include <Arduino.h>
#include <ArduinoJson.h>

Extra::Extra() {
  pinMode(BUZZER, OUTPUT);
}

void Extra::wifi_setup() {
  char dot = '.';
  String message = "Wifi axtarirlir";
  String temp = "";
  int counter = 0;
  while (true) {
    if (!wifi.init()) {

      if (counter == 3) {
        temp = "";
        counter = 0;
      }
      temp += String(dot);
      display.printMessage(message + temp, 1, { 20, 30 }, true);
      counter++;
    } else {
      break;
    }
  }

  display.printMessage("Wifi baglandi", 1, { 20, 20 }, true);
}

void Extra::beep() {
  tone(BUZZER, 1000);
  delay(100);
  noTone(BUZZER);
}

String Extra::jsonExtract(String data, String key, int limit_data) {
  StaticJsonDocument<2000> doc;  // 200 byte'lık bir bellek tahsisi

  if (data.length() > limit_data) {
    DeserializationError error = deserializeJson(doc, data);

    if (error) {
      Serial.println(error.f_str());
      return error.f_str();  // Hata varsa, döngüyü sonlandırıyoruz.
    }

    return String(doc[key]);
  }
}