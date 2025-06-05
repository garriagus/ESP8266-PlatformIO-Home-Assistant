#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void initDisplay();
void mostrarMensaje(const String& msg);
void actualizarDisplay();

extern String mensajeActual;

#endif
