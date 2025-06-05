#include <Arduino.h>
#include "display.h"
#include "sensor.h"
#include "button.h"

void setup() {
  Serial.begin(115200);
  initDisplay();
  initSensor();
  initButton();
  mostrarMensaje("Hola Mundo!");
}

void loop() {
  handleButton();
  actualizarDisplay();
}
