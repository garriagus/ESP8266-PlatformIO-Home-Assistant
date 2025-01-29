#include <Arduino.h>

c#include <SPI.h>
#include <Ethernet.h>

// Configuración de Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // Dirección MAC del Ethernet Shield
IPAddress ip(192, 168, 0, 100);                     // Dirección IP fija
EthernetServer server(80);                          // Puerto del servidor HTTP

void setup() {
  // Inicializar comunicación serial para depuración
  Serial.begin(9600);

  // Inicializar Ethernet
  Ethernet.init(10); // Pin CS para el W5100
  Ethernet.begin(mac, ip);

  // Iniciar el servidor
  server.begin();
  Serial.print("Servidor iniciado en IP: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Escuchar por clientes entrantes
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Nuevo cliente conectado");

    // Esperar a que el cliente envíe datos
    while (client.connected()) {
      if (client.available()) {
        client.read(); // Leer los datos del cliente (no los usamos aquí)

        // Enviar respuesta HTTP al cliente
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        client.println("<!DOCTYPE html>");
        client.println("<html>");
        client.println("<head><title>Servidor Arduino</title></head>");
        client.println("<body>");
        client.println("<h1>Bienvenido a mi servidor Arduino</h1>");
        client.println("<p>Este mensaje es servido por un Arduino Uno con Ethernet Shield.</p>");
        client.println("</body></html>");

        break; // Salir del bucle después de enviar la respuesta
      }
    }

    // Cerrar la conexión con el cliente
    delay(1);
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
