#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "../credentials.h"

// Configuración de WiFi
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Configuración de MQTT
const char* mqtt_server = MQTT_HOST; 
const char* mqtt_user = MQTT_USERNAME;    
const char* mqtt_pass = MQTT_PASSWORD;  
const char* topic = "home/welcome";

// Configuración de IP estática
IPAddress local_IP(192, 168, 0, 200);      // Cambia esto por la IP deseada
IPAddress gateway(192, 168, 0, 1);         // Cambia según tu red
IPAddress subnet(255, 255, 255, 0);        // Máscara de red
IPAddress dns(192, 168, 0, 1);             // Servidor DNS

// Instancias WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  // Conectar a WiFi
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("Falló la configuración de IP estática");
  }
  WiFi.begin(ssid, password);

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConexión WiFi establecida.");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Reconectar a MQTT
  while (!client.connected()) {
    Serial.print("Conectando al servidor MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado.");
      client.publish(topic, "¡Bienvenido a MQTT!", false);  // Enviar mensaje
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos.");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Conectar a WiFi
  setup_wifi();

  // Configurar servidor MQTT
  client.setServer(mqtt_server, 1883);

  // Conectar a MQTT
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
