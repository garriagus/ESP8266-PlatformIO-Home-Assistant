#include "sensor.h"
#include "display.h"
#include <DHT.h>

#define DHTPIN D1
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
float lastTemp = NAN;
bool showTemp = false;

void initSensor() {
  dht.begin();
}

float leerTemperatura() {
  return dht.readTemperature();
}

void actualizarContenido() {
  if (showTemp) {
    float t = leerTemperatura();
    if (isnan(t)) {
      if (mensajeActual != "Error leyendo temp")
        mostrarMensaje("Error leyendo temp");
    } else if (isnan(lastTemp) || abs(t - lastTemp) >= 0.1) {
      lastTemp = t;
      mostrarMensaje("Interior: " + String(t, 1) + "C\xB0");
    }
  } else {
    if (mensajeActual != "Hola Mundo!")
      mostrarMensaje("Hola Mundo!");
  }
}
