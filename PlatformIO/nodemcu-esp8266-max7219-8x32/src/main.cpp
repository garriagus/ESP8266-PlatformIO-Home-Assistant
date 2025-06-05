#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <DHT.h>

#define DATA_PIN  D6
#define CLK_PIN   D5
#define CS_PIN    D8
#define MAX_DEVICES 4

#define DHTPIN    D1
#define DHTTYPE   DHT22

#define BUTTON_PIN D2

MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
DHT dht(DHTPIN, DHTTYPE);

bool showTemp = false;
int lastButtonState = HIGH;
int buttonState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

String mensajeActual = "";
float lastTemp = NAN;

// Prototipo de función
void mostrarMensaje(const String& msg);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  P.begin();
  P.setIntensity(5);
  P.displayClear();

  // Mostrar mensaje inicial
  mostrarMensaje("Hola Mundo!");
}

void mostrarMensaje(const String& msg) {
  if (mensajeActual != msg) {
    mensajeActual = msg;
    P.displayClear();
    P.displayText(mensajeActual.c_str(), PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}

void loop() {
  // Botón con antirrebote
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        showTemp = !showTemp;
        lastTemp = NAN; // Forzar actualización

        if (showTemp) {
          // Mostrar temperatura inmediatamente al cambiar modo
          float t = dht.readTemperature();
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

  // Avanza animación y detecta final
  if (P.displayAnimate()) {
    P.displayReset();

    if (showTemp) {
      // Actualiza temperatura solo si cambió y al terminar scroll
      float t = dht.readTemperature();
      if (isnan(t)) {
        if (mensajeActual != "Error leyendo temp") {
          mostrarMensaje("Error leyendo temp");
        }
      } else {
        if (isnan(lastTemp) || abs(t - lastTemp) >= 0.1) {
          lastTemp = t;
          mostrarMensaje("Interior: " + String(t, 1) + "C\xB0");
        }
      }
    } else {
      // Si no muestra temp, se queda en "Hola Mundo!"
      if (mensajeActual != "Hola Mundo!") {
        mostrarMensaje("Hola Mundo!");
      }
    }
  }
}
