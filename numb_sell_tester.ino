#include <vector>
#include "display_lib.h"
#include "image.h"
#include "api.h"
#include <Fonts/FreeMono12pt7b.h>  // Başka bir font
#include "custom_fonts.h"
#include "extra_lib.h"

Display d = Display();
Api api = Api();
Extra extra = Extra();

unsigned long lastDataTime = 0;             // Veri alındığı zaman
const unsigned long idleTimeout = 3000;     // 3 saniye boyunca veri alınmazsa
const unsigned long idleTimeoutOff = 5000;  // 3 saniye boyunca veri alınmazsa

float difference = 0.0;

void setup() {
  d.init();
  d.clear();
  extra.wifi_setup();
  extra.memoryInfo();
  sleep(2);
}

void loop() {
  d.clear();  // OLED ekranı temizliyor, ancak her döngüde çağrılması gerekmeyebilir.

  String dataApi = "";
  float usd = 0.0000;

  delay(100);
  String url = api.getCurrencyUrl("bitcoin");
  String data = api.fetchData(url);

  dataApi = extra.jsonExtract(data, "data");
  Serial.println(data);
  Serial.println(dataApi);

  usd = String(extra.jsonExtract(dataApi, "priceUsd")).toFloat();
  // Farklı bir değer geldiyse beep sinyali ver
  if (difference != usd) {
    lastDataTime = millis();  // Veri alındığında zamanı sıfırlıyoruz
    d.displayLightSwitch(true);
    d.contrast(255);
    difference = usd;  // Yeni değeri kaydet
    extra.beep();      // Beep sinyali
    Serial.println("Farklı bir değer alındı. Yeni değer: " + String(difference));
  } else {
    if (millis() - lastDataTime > idleTimeout) {
      d.contrast(1);
      if (millis() - lastDataTime > idleTimeoutOff) {
        d.displayLightSwitch(false);
      }
    }
  }
  // OLED ekrana veriyi yazdır
  d.printMessage(String(usd, 4), 1, { 10, 62 }, true, &FreeMono12pt7b);
  d.printMessage("BTC anliq", 0, { 15, 15 }, false, &Pacifico_Regular10pt7b);
  d.printMessage("Qiymet", 0, { 25, 34 }, false, &Pacifico_Regular10pt7b);
  Serial.println(dataApi);

  delay(300);  // Döngünün hızını yavaşlat
}