#include <RTClib.h>
#include "indicador.h"

RTC_DS1307 DS1307_RTC;

void RTCSet() {
  if (!DS1307_RTC.begin()) {
    Serial.println("No existe RTC");
    errLed();
    while (1);
  } else {
      DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // En caso de descalibración del RTC y conectado al PC
      okLed();
  }

  DateTime now = DS1307_RTC.now();

  // Verificar si la hora almacenada en el RTC es válida
  if (now.year() < 2022) {
    Serial.println("Tiempo Invalido, actualizando fecha y hora");
    DS1307_RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}