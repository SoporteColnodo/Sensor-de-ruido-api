#include <TinyGPSPlus.h>
#include "indicador.h"

TinyGPSPlus gps;

bool gpsData() {
  unsigned long startTime = millis();

  while (millis() - startTime < 10000) {  // Esperar hasta 10 segundos para obtener una fijación válida
    while (Serial2.available() > 0) {
      if (gps.encode(Serial2.read())) {
        if (gps.location.isValid()) {
          Serial.println("Conexión a GPS correcta");
          //okLed();
          return true;
        }
      }
    }
    delay(1000);  // Pequeño retardo para evitar lecturas excesivas
  }

  Serial.println("No se pudo obtener una fijación GPS válida");
  //errGPS();
  return false;
}

String returnLat() {
  if (gps.location.isValid()) {
    return String(gps.location.lat(), 6);
  } else {
    Serial.println("La fijación GPS no es válida para obtener la latitud");
    // Manejar el caso de fijación no válida, como intentar obtener una nueva fijación.
    return "0.000000";
  }
}

String returnLng() {
  if (gps.location.isValid()) {
    return String(gps.location.lng(), 6);
  } else {
    Serial.println("La fijación GPS no es válida para obtener la longitud");
    // Manejar el caso de fijación no válida, como intentar obtener una nueva fijación.
    return "0.000000";
  }
}