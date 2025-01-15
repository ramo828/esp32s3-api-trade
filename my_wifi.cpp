#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "my_wifi.h"

char *ssid = "";
char *password = "";

// Yapıcı (constructor) tanımlanması gerekebilir, ama şu anda boş bir yapıcı kullanılabilir.
MyWifi::MyWifi(char *SSID, char *PASSWORD) {
  ssid = SSID;
  password = PASSWORD;
  Serial.begin(115200);
}
bool MyWifi::init() {
  WiFi.begin(ssid, password);

  // Bağlanma denemesi için maksimum bekleme süresi
  unsigned long startAttemptTime = millis();
  const unsigned long connectionTimeout = 20000; // 20 saniye

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime >= connectionTimeout) {
      Serial.println("WiFi bağlantısı zaman aşımına uğradı!");
      return false; // Zaman aşımı durumunda false döner
    }
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  return true;
}
