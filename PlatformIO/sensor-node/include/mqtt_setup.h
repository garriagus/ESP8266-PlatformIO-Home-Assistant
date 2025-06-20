#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Declaración de variables externas
extern WiFiClient espClient;
extern PubSubClient client;

// Declaración de funciones
void mqtt_setup();
void reconnect_mqtt();
void mqtt_loop();

// Funciones para publicar datos separados
void publicarDatosSensorDHT(float temp, float hum, int ldr);
void publicarDatosSensorIR(uint32_t irCode);

#endif
