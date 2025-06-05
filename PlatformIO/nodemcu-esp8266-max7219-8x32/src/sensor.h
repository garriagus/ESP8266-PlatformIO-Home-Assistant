#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

void initSensor();
float leerTemperatura();
void actualizarContenido();

extern float lastTemp;
extern bool showTemp;

#endif
