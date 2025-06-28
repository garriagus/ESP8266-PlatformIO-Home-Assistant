#include <Arduino.h>
#include <DHT.h>
#include <ArduinoOTA.h>
#include "ota_setup.h"
#include "max7219/display_manager.h"

// Configuración del DHT22
#define DHTPIN D1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Pines para botones y relé
#define RELAY_PIN D2
#define BUTTON_D0 D0
#define BUTTON_D3 D3
#define BUTTON_D4 D4

// Estado actual del relé
bool releEncendido = false;

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 5000;  // Actualizar cada 5 segundos

void setup() {
  Serial.begin(115200);
  delay(100);

  // Inicialización de pines
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Apagado al inicio

  pinMode(BUTTON_D0, INPUT_PULLUP);
  pinMode(BUTTON_D3, INPUT_PULLUP);
  pinMode(BUTTON_D4, INPUT_PULLUP);

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

  // Lectura de botones (pulldown lógico: se activa al pasar a LOW)
  if (digitalRead(BUTTON_D0) == LOW) {
    Serial.println("Boton D0 presionado");
    delay(300);  // Antirebote simple
  }

  if (digitalRead(BUTTON_D3) == LOW) {
    Serial.println("Boton D3 presionado");
    delay(300);  // Antirebote
  }

  if (digitalRead(BUTTON_D4) == LOW) {
    Serial.println("Boton D4 presionado");

    // Cambiar estado del relé
    releEncendido = !releEncendido;
    digitalWrite(RELAY_PIN, releEncendido ? HIGH : LOW);
    delay(300);  // Antirebote
  }

  static bool waiting = true;
  if (animacionLista()) {
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
