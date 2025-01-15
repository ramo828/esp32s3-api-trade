#include <ArduinoJson.h>
#include <vector>
#include "display_lib.hpp"
#include "image.h"
#include "my_wifi.h"
#include "api.h"
#include <Fonts/FreeMono12pt7b.h>  // Başka bir font
#include "custom_fonts.h"

Display d = Display();
MyWifi wifi = MyWifi();
Api api = Api();
StaticJsonDocument<200> dataDoc;  // 200 byte'lık bir bellek tahsisi
StaticJsonDocument<200> doc;      // 200 byte'lık bir bellek tahsisi
std::vector<int> cursorPosition = { 20, 20 };
std::vector<int> agiz = { 0, 0 };
std::vector<int> qas = { 0, 0 };
float difference = 0.0;
#define BUZZER 7

void wifi_setup() {
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
      d.printMessage(message + temp, 1, { 20, 30 }, true);
      counter++;
    } else {
      break;
    }
  }

  d.printMessage("Wifi baglandi", 1, cursorPosition, true);
}

void setup() {
  d.init();
  d.clear();
  wifi_setup();
  pinMode(BUZZER, OUTPUT);
}

void beep() {
  tone(BUZZER, 1000);
  delay(100);
  noTone(BUZZER);
}
void loop() {
  d.clear();  // OLED ekranı temizliyor, ancak her döngüde çağrılması gerekmeyebilir.

  String dataApi = "";
  float usd = 0.0000;

  delay(100);
  String url = api.getCurrencyUrl("bitcoin");
  String data = api.fetchData(url);

  if (data.length() > 9) {
    DeserializationError error = deserializeJson(doc, data);

    if (error) {
      Serial.println(error.f_str());
      return;  // Hata varsa, döngüyü sonlandırıyoruz.
    }

    dataApi = String(doc["data"]);
    Serial.println(data);

    deserializeJson(dataDoc, dataApi);
    usd = String(dataDoc["priceUsd"]).toFloat();

    // Farklı bir değer geldiyse beep sinyali ver
    if (difference != usd) {
      difference = usd;  // Yeni değeri kaydet
      beep();            // Beep sinyali
      Serial.println("Farklı bir değer alındı. Yeni değer: " + String(difference));
    }

    // OLED ekrana veriyi yazdır
    d.printMessage(String(usd, 4), 1, { 15, 62 }, true, &FreeMono12pt7b);
    d.printMessage("XRP anliq", 0, { 15, 15 }, false, &Pacifico_Regular10pt7b);
    d.printMessage("Qiymet", 0, { 25, 34 }, false, &Pacifico_Regular10pt7b);
  }

  Serial.println(dataApi);

  delay(300);  // Döngünün hızını yavaşlat
}