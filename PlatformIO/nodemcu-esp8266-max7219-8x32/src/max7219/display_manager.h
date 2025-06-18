#pragma once
#include <Arduino.h>

void iniciarDisplay();
void mostrarMensaje(const char* mensaje);
void setBrillo(uint8_t nivel);
bool animacionLista();  // Para usar display.displayAnimate() externamente
