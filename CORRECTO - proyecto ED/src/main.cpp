// Librerías necesarias
#include "mail.h"
#include <WiFi.h>
// include .h del mail PARA MEZCLAR tiene que estar el prototipo de como se llama la funcion



// Definición de pines
#define BUTTON_PIN 4    // Pin del botón
#define RED_LED_PIN 12  // Pin del LED rojo
#define GREEN_LED_PIN 13 // Pin del LED verde



// Configuración del WiFi
const char* ssid = "UA-Alumnos";
const char* password = "41umn05WLC";

// Inicialización
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
#if 0
  // Conexión a la red WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }
  Serial.println("Conectado al WiFi");
#endif

}
// Bucle principal
void loop() {

  if (digitalRead(BUTTON_PIN) == LOW) { // Botón presionado
    Serial.println("tecla apretada");
    // Generar número aleatorio
    // Genera un número entre 0 y 1

    if (random(2)) {
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, LOW);
      if(sendMail())
        Serial.println("Se ha enviado el mail");
      else
        Serial.println("Error");
      

    } else {
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, HIGH);
      // llamo funcion SEND MAIL o comomla haya llamado en el include 
      
    }

    delay(5000); // Esperar un segundo antes de leer el botón nuevamente
  } else {
    Serial.println("tecla no apretada");
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    delay(500);
  }
}