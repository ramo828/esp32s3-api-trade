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

void Extra::memoryInfo() {
  // RAM bilgileri
  Serial.println("=== RAM Bilgileri ===");
  Serial.print("Toplam heap: ");
  Serial.println(ESP.getHeapSize());  // Toplam RAM
  Serial.print("Kullanılan heap: ");
  Serial.println(ESP.getHeapSize() - ESP.getFreeHeap());  // Kullanılan RAM
  Serial.print("Boş heap: ");
  Serial.println(ESP.getFreeHeap());  // Boş RAM

  // PSRAM bilgileri (ESP32'de varsa)
  if (ESP.getPsramSize() > 0) {
    Serial.println("\n=== PSRAM Bilgileri ===");
    Serial.print("Toplam PSRAM: ");
    Serial.println(ESP.getPsramSize());  // Toplam PSRAM
    Serial.print("Kullanılan PSRAM: ");
    Serial.println(ESP.getPsramSize() - ESP.getFreePsram());  // Kullanılan PSRAM
    Serial.print("Boş PSRAM: ");
    Serial.println(ESP.getFreePsram());  // Boş PSRAM
  }

  // Flaş hafıza bilgileri
  Serial.println("\n=== Flaş Hafıza Bilgileri ===");
  Serial.print("Flaş boyutu: ");
  Serial.println(ESP.getFlashChipSize());  // Toplam flaş boyutu
  Serial.print("Kullanılabilir flaş: ");
  Serial.println(ESP.getFreeSketchSpace());  // Boş flaş alanı
  Serial.print("Yüklü sketch boyutu: ");
  Serial.println(ESP.getSketchSize());  // Yüklü programın boyutu
}
