#include "button.h"
#include "sensor.h"
#include "display.h"
#include <Arduino.h>

#define BUTTON_PIN D2

int lastButtonState = HIGH;
int buttonState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void initButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void handleButton() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        showTemp = !showTemp;
        lastTemp = NAN;

        if (showTemp) {
          float t = leerTemperatura();
          if (isnan(t)) {
            mostrarMensaje("Error leyendo temp");
          } else {
            mostrarMensaje("Interior: " + String(t, 1) + "C\xB0");
            lastTemp = t;
          }
        } else {
          mostrarMensaje("Hola Mundo!");
        }
      }
    }
  }
  lastButtonState = reading;
}
