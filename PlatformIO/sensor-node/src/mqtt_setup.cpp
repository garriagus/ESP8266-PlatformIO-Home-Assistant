#include "mqtt_setup.h"
#include "secrets.h"  // Para mqtt_user, mqtt_pass, mqtt_server

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect_mqtt()
{
  while (!client.connected())
  {
    Serial.print("Conectando a MQTT...");
    if (client.connect("esp8266_client", mqtt_user, mqtt_pass))
    {
      Serial.println("✅ Conectado al broker MQTT");
    }
    else
    {
      Serial.print("❌ Falló conexión, rc=");
      Serial.print(client.state());
      Serial.println(" - Reintentando en 5 segundos");
      delay(5000);
    }
  }
}

void mqtt_setup()
{
  client.setServer(mqtt_server, 1883);
}

void mqtt_loop()
{
  if (!client.connected())
  {
    reconnect_mqtt();
  }
  client.loop();
}

// Publica temperatura, humedad y luz en mensajes separados
void publicarDatosSensorDHT(float temp, float hum, int ldr)
{
  if (!client.connected()) return;

  char buffer[16];

  dtostrf(temp, 4, 2, buffer);
  client.publish("sensores/temperatura", buffer);

  dtostrf(hum, 4, 2, buffer);
  client.publish("sensores/humedad", buffer);

  snprintf(buffer, sizeof(buffer), "%d", ldr);
  client.publish("sensores/luz", buffer);
}

// Publica solo el código IR (si es distinto de cero)
void publicarDatosSensorIR(uint32_t irCode)
{
  if (!client.connected()) return;
  if (irCode == 0) return;  // No publicar código 0

  char buffer[16];
  snprintf(buffer, sizeof(buffer), "0x%X", irCode);
  client.publish("sensores/infrarrojo", buffer);
}
