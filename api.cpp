#include <WiFi.h>
#include <HTTPClient.h>
#include "api.h"

String Api::getCurrencyUrl(char* crypto_currency) {
  String myApiUrl = "https://api.coincap.io/v2/assets/" + String(crypto_currency);

  return myApiUrl;
}
// API'den Verileri Çeken Fonksiyon
String Api::fetchData(String apiUrl) {
  Serial.println(apiUrl);
  HTTPClient http;

  // Wi-Fi bağlantısını kontrol et
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi bağlantısı mevcut değil. Lütfen Wi-Fi bağlantısını kontrol edin.");
    return "Error: No Wi-Fi connection";
  }

  // Zaman aşımını ayarla
  http.setTimeout(15000);  // 15 saniye

  // URL başlatma
  if (!http.begin(apiUrl)) {
    Serial.println("HTTP istemci başlatılamadı. URL formatını kontrol edin.");
    return "Error: HTTP client initialization failed";
  }

  // Gerekli HTTP header'ları ekliyoruz
  http.addHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36 v2");
  http.addHeader("Host", "api.coincap.io");
  http.addHeader("Accept", "*/*");
  http.addHeader("Accept-Encoding", "gzip, deflate, br");

  // GET isteğini gönder
  int httpResponseCode = http.GET();
  Serial.print("HTTP Yanıt Kodu: ");
  Serial.println(httpResponseCode);

  // Yanıt kodlarına göre hata yönetimi
  if (httpResponseCode > 0) {
    if (httpResponseCode == 200) {
      String response = http.getString();
      Serial.println("API Cevabı:");
      Serial.println(response);
      http.end();
      return response;
    } else if (httpResponseCode == 401) {
      Serial.println("Hata: Yetkilendirme hatası (401). API anahtarınızı kontrol edin.");
      http.end();
      return "Error: Unauthorized (401)";
    } else if (httpResponseCode == 403) {
      Serial.println("Hata: Erişim engellendi (403). API erişiminizi kontrol edin.");
      http.end();
      return "Error: Forbidden (403)";
    } else if (httpResponseCode == 404) {
      Serial.println("Hata: İstek yapılan kaynak bulunamadı (404). URL'yi kontrol edin.");
      http.end();
      return "Error: Not Found (404)";
    } else if (httpResponseCode == 500) {
      Serial.println("Hata: Sunucu hatası (500). API sağlayıcısı ile iletişime geçin.");
      http.end();
      return "Error: Internal Server Error (500)";
    } else {
      Serial.println("Hata: Beklenmeyen bir HTTP yanıt kodu alındı.");
      http.end();
      return "Error: Unexpected HTTP response (" + String(httpResponseCode) + ")";
    }
  } else {
    // HTTP isteği başarısız olduğunda
    switch (httpResponseCode) {
      case HTTPC_ERROR_CONNECTION_REFUSED:
        Serial.println("Hata: Bağlantı reddedildi. Sunucu erişilebilir mi?");
        break;
      case HTTPC_ERROR_SEND_HEADER_FAILED:
        Serial.println("Hata: HTTP başlıkları gönderilemedi. Header yapılandırmasını kontrol edin.");
        break;
      case HTTPC_ERROR_SEND_PAYLOAD_FAILED:
        Serial.println("Hata: HTTP verisi gönderilemedi. Payload'ı kontrol edin.");
        break;
      case HTTPC_ERROR_NOT_CONNECTED:
        Serial.println("Hata: Sunucuya bağlantı kurulamadı. URL'yi veya Wi-Fi bağlantısını kontrol edin.");
        break;
      case HTTPC_ERROR_CONNECTION_LOST:
        Serial.println("Hata: Bağlantı sırasında kopma meydana geldi. İnternet bağlantınızı kontrol edin.");
        break;
      case HTTPC_ERROR_NO_HTTP_SERVER:
        Serial.println("Hata: HTTP sunucusu bulunamadı. URL'yi kontrol edin.");
        break;
      default:
        Serial.println("Hata: HTTP bağlantısı sırasında bilinmeyen bir hata oluştu.");
        break;
    }
    http.end();
    return "Error: HTTP request failed with code " + String(httpResponseCode);
  }

  http.end();  // HTTP bağlantısını kapat
  return "Error: Unknown error occurred";
}
