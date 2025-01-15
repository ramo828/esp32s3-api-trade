#include "display_lib.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include <pgmspace.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C  // Doğru I2C adresini buraya yazın

// İstediğiniz SDA ve SCL pinlerini tanımlayın
#define SDA_PIN 10  // SDA pinini 21 yapalım
#define SCL_PIN 11  // SCL pinini 22 yapalım

// Pinleri özelleştirilmiş TwoWire nesnesiyle tanımlıyoruz
TwoWire myWire = TwoWire(0);  // 0, I2C port numarasını belirtir

// Adafruit SSD1306 ekranını özelleştirilmiş Wire nesnesiyle başlatıyoruz
Adafruit_SSD1306 oled_display(SCREEN_WIDTH, SCREEN_HEIGHT, &myWire, OLED_RESET);

Display::Display() {
  Serial.begin(115200);  // Seri haberleşmeyi başlat
  Serial.println("Display nesnesi oluşturuluyor...");
  oled_display.clearDisplay();
  oled_display.display();
  delay(2000);
  oled_display.clearDisplay();
  Serial.println("Display nesnesi oluşturuldu.");
}

bool Display::init() {
  // I2C'yi başlatıyoruz ve özel SDA ve SCL pinlerini ayarlıyoruz
  myWire.begin(SDA_PIN, SCL_PIN);  // Burada SDA ve SCL pinlerini belirtiyoruz

  // OLED ekran başlatma
  if (!oled_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED ekran başlatılamadı. Sistem yeniden başlatılıyor.");
    delay(1000);  // Hata mesajını okumak için gecikme
    return false;
  }
  return true;
}

void Display::printMessage(String message, int sizeText, std::vector<int> cursorPosition, bool clearStatus, const GFXfont *f) {
  if (cursorPosition.size() != 2) {
    Serial.println("Hatalı cursorPosition boyutu. Varsayılan değer kullanılacak.");
    cursorPosition = { 0, 0 };  // Varsayılan değer
  }
  if (clearStatus)
    oled_display.clearDisplay();
  oled_display.setTextSize(sizeText);
  oled_display.setFont(f);  // Örnek: Sans Bold, 12pt
  oled_display.setTextColor(SSD1306_WHITE);
  oled_display.setCursor(cursorPosition[0], cursorPosition[1]);
  oled_display.println(message);
  oled_display.display();
}

void Display::drawBitmap(int x, int y, const uint8_t *bitmap, int w, int h) {
  oled_display.drawBitmap(x, y, bitmap, w, h, SSD1306_WHITE);
  oled_display.display();
}

void Display::clear() {
  oled_display.clearDisplay();
}
