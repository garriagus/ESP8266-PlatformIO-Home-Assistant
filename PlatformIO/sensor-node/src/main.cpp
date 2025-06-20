#include <Arduino.h>
#include <DHT.h>
#include <ArduinoOTA.h>
#include <IRremote.h>
#include "ota_setup.h"
#include "mqtt_setup.h"

// ----- PINS -----
#define DHTPIN 4
#define DHTTYPE DHT22
#define LDRPIN A0
#define IR_RECEIVE_PIN 14

// ----- Objetos -----
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 5000;

void setup()
{
  Serial.begin(115200);
  delay(100);

  dht.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  ota_setup();   // Conexión WiFi + OTA
  mqtt_setup();  // Configura MQTT pero no conecta aún

  delay(1000);
  Serial.println("Sistema iniciado");
}

void loop()
{
  ArduinoOTA.handle();
  mqtt_loop();   // Mantiene conexión MQTT y reconecta si es necesario

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= updateInterval)
  {
    lastUpdate = currentMillis;

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    int ldrValue = analogRead(LDRPIN);

    if (isnan(humidity) || isnan(temperature))
    {
      Serial.println("Error al leer del sensor DHT!");
    }
    else
    {
      Serial.print("Temperatura: ");
      Serial.print(temperature);
      Serial.print(" °C\t");
      Serial.print("Humedad: ");
      Serial.print(humidity);
      Serial.println(" %");
      Serial.print("Luz (LDR): ");
      Serial.println(ldrValue);

      // Publica solo datos de sensores DHT y LDR
      publicarDatosSensorDHT(temperature, humidity, ldrValue);
    }
  }

  if (IrReceiver.decode())
  {
    uint32_t irCode = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Código IR recibido: 0x");
    Serial.println(irCode, HEX);

    // Publica solo datos IR
    publicarDatosSensorIR(irCode);

    IrReceiver.resume();
  }
}
