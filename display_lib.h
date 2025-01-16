#ifndef DISPLAY_LIB_HPP
#define DISPLAY_LIB_HPP
#include <Arduino.h>  // String sınıfı için
#include <iostream>
#include <vector>  // std::vector kullanımı için gerekli başlık dosyası
#include <Adafruit_GFX.h>

class Display {
public:
  Display();                                                                                                                                          // Constructor
  void printMessage(String message, int sizeText = 2, std::vector<int> cursorPosition = { 0, 0 }, bool clearStatus = true, const GFXfont *f = NULL);  // Varsayılan değerler ile fonksiyon
  bool init();
  void clear();
  void contrast(int light);
  void displayLightSwitch(bool status);
  void drawBitmap(int x, int y, const uint8_t *bitmap, int w, int h);  // Bitmap çizmek için işlev

private:
  // Gerekirse burada özel değişkenler tanımlayabilirsiniz
};

#endif  // DISPLAY_LIB_HPP
