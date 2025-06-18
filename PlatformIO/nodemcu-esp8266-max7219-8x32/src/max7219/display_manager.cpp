#include "display_manager.h"
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// Pines del MAX7219 (ajustá si cambian)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   D5
#define DATA_PIN  D7
#define CS_PIN    D6

static MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void iniciarDisplay() {
  display.begin();
  display.setIntensity(8);       // Brillo medio por defecto
  display.displayClear();
  display.setSpeed(50);
  display.setPause(1000);
}

void mostrarMensaje(const char* mensaje) {
  Serial.println(mensaje);
  display.displayText(mensaje, PA_CENTER, display.getSpeed(), display.getPause(), PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void setBrillo(uint8_t nivel) {
  if (nivel <= 15) {
    display.setIntensity(nivel);
  }
}

bool animacionLista() {
  return display.displayAnimate();
}
