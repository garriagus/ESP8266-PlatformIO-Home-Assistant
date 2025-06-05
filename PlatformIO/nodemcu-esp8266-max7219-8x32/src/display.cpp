#include "display.h"
#include <MD_Parola.h>
#include <MD_MAX72XX.h>

#define DATA_PIN  D6
#define CLK_PIN   D5
#define CS_PIN    D8
#define MAX_DEVICES 4

MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
String mensajeActual = "";

void initDisplay() {
  P.begin();
  P.setIntensity(5);
  P.displayClear();
}

void mostrarMensaje(const String& msg) {
  if (mensajeActual != msg) {
    mensajeActual = msg;
    P.displayClear();
    P.displayText(msg.c_str(), PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}

void actualizarDisplay() {
  if (P.displayAnimate()) {
    P.displayReset();
    // se delega en sensor.cpp
    extern void actualizarContenido();
    actualizarContenido();
  }
}
