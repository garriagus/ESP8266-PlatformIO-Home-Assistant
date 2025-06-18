#include <Arduino.h>
#include <DHT.h>
#include <ArduinoOTA.h>
#include "ota_setup.h"
#include "max7219/display_manager.h"  // <- Nuevo módulo de display

// Configuración del DHT22
#define DHTPIN D1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 5000;  // Actualizar cada 5 segundos

void setup() {
  Serial.begin(115200);
  delay(100);

  dht.begin();
  iniciarDisplay();  // Inicializa matriz LED

  mostrarMensaje("Iniciando...");
  delay(1000);

  mostrarMensaje("Conectando a la red...");
  ota_setup();  // Configura WiFi y OTA
  mostrarMensaje("WiFi conectado");
  delay(1000);
}

void loop() {
  ArduinoOTA.handle();

  static bool waiting = true;
  if (animacionLista()) {  // La animación terminó
    if (waiting || millis() - lastUpdate > updateInterval) {
      float temp = dht.readTemperature();
      float hum = dht.readHumidity();

      if (isnan(temp) || isnan(hum)) {
        mostrarMensaje("Error lectura DHT");
      } else {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "Temp: %.1f C Hum: %.1f %%", temp, hum);
        mostrarMensaje(buffer);
      }

      lastUpdate = millis();
      waiting = false;
    }
  }
}
