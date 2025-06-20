#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"  // Aquí tenés ssid, password, ip, gateway, subnet

void conectarWiFi() {
  Serial.println("Iniciando conexión WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("✅ WiFi conectado");
  Serial.print("📡 IP asignada: ");
  Serial.println(WiFi.localIP());
}

void ota_setup() {
  conectarWiFi();  // Modularizado

  // Configurar OTA
  ArduinoOTA.setHostname("esp8266");
  ArduinoOTA.setPassword("1234");  // Debe coincidir con platformio.ini

  ArduinoOTA.onStart([]() {
    Serial.println("🔁 OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\n✅ OTA End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("⏳ OTA Progreso: %u%%\r", (progress * 100) / total);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("❌ OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Fallo de autenticación");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Fallo al comenzar");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Fallo de conexión");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Fallo al recibir");
    else if (error == OTA_END_ERROR) Serial.println("Fallo al finalizar");
  });

  ArduinoOTA.begin();
  Serial.println("📡 OTA listo para recibir actualizaciones");
}