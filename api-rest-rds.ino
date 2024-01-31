#include "RTC_Utils.hpp"
#include "config.h"
#include "indicador.h"
#include "Wifi_Utils.hpp"
#include "GPS_Utils.hpp"
#include "Ruido.hpp"
#include "SD_Utils.hpp"
#include "API.hpp"

int maxAttempts = 100;     // Número máximo de intentos antes de abandonar
int currentAttempts = 0;  // Número de intentos actual

void setup() {
  RTCSet();
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(500);
  alertsSetup();
  CreateCSV(createcsv);
  gpsData();
}

void loop() {
  unsigned long currentMillis = millis();
  String decibeles = "";
  String latitud = "";
  String longitud = "";

  // Verifica si ha pasado una hora desde el último envío (cada hora)
  unsigned long startAPI = millis();  // Tiempo de inicio de ruidoData
  if (currentMillis - previousMillisAPI >= intervalAPI) {
    // Actualiza el tiempo del último envío
    previousMillisAPI = currentMillis;

    unsigned long startRuidoData = millis();  // Tiempo de inicio de ruidoData
    decibeles = ruidoData();
    unsigned long endRuidoData = millis();  // Tiempo de fin de ruidoData
    Serial.print("Tiempo de inicio de ruidoData: ");
    Serial.println(startRuidoData);
    Serial.print("Tiempo de fin de ruidoData: ");
    Serial.println(endRuidoData);

    unsigned long startReturnLat = millis();  // Tiempo de inicio de returnLat
    latitud = returnLat();
    unsigned long endReturnLat = millis();  // Tiempo de fin de returnLat
    Serial.print("Tiempo de inicio de returnLat: ");
    Serial.println(startReturnLat);
    Serial.print("Tiempo de fin de returnLat: ");
    Serial.println(endReturnLat);

    unsigned long startReturnLng = millis();  // Tiempo de inicio de returnLng
    longitud = returnLng();
    unsigned long endReturnLng = millis();  // Tiempo de fin de returnLng
    Serial.print("Tiempo de inicio de returnLng: ");
    Serial.println(startReturnLng);
    Serial.print("Tiempo de fin de returnLng: ");
    Serial.println(endReturnLng);

    if (decibeles != "0" && latitud != "0.000000" && longitud != "0.000000") {
      unsigned long startAPIConnect = millis();  // Tiempo de inicio de ConnectWiFi_STA
      ConnectWiFi_STA();
      unsigned long endAPIConnect = millis();  // Tiempo de fin de ConnectWiFi_STA
      Serial.print("Tiempo de inicio de ConnectWiFi_STA: ");
      Serial.println(startAPIConnect);
      Serial.print("Tiempo de fin de ConnectWiFi_STA: ");
      Serial.println(endAPIConnect);

      unsigned long startAPICreate = millis();  // Tiempo de inicio de Create
      Create(decibeles, latitud, longitud, LoginUser(User, Password));
      unsigned long endAPICreate = millis();  // Tiempo de fin de Create
      Serial.print("Tiempo de inicio de Create: ");
      Serial.println(startAPICreate);
      Serial.print("Tiempo de fin de Create: ");
      Serial.println(endAPICreate);

      unsigned long startAPIDisconnect = millis();  // Tiempo de inicio de WiFi.disconnect
      WiFi.disconnect(true);
      delay(100);
      unsigned long endAPIDisconnect = millis();  // Tiempo de fin de WiFi.disconnect
      Serial.print("Tiempo de inicio de WiFi.disconnect: ");
      Serial.println(startAPIDisconnect);
      Serial.print("Tiempo de fin de WiFi.disconnect: ");
      Serial.println(endAPIDisconnect);

      unsigned long startAPIOkLed = millis();  // Tiempo de inicio de okLed
      okLed();
      unsigned long endAPIOkLed = millis();  // Tiempo de fin de okLed
      Serial.print("Tiempo de inicio de okLed: ");
      Serial.println(startAPIOkLed);
      Serial.print("Tiempo de fin de okLed: ");
      Serial.println(endAPIOkLed);
    } else {
      Serial.println("El GPS arroja valores 'INVALID' que no adecuados para el proceso");
      unsigned long startAPIErrGPS = millis();  // Tiempo de inicio de errGPS
      errGPS();
      unsigned long endAPIErrGPS = millis();  // Tiempo de fin de errGPS
      Serial.print("Tiempo de inicio de errGPS: ");
      Serial.println(startAPIErrGPS);
      Serial.print("Tiempo de fin de errGPS: ");
      Serial.println(endAPIErrGPS);

      // Puedes aumentar el contador de intentos y salir si se supera el máximo
      currentAttempts++;
      if (currentAttempts >= maxAttempts) {
        Serial.println("Se superó el número máximo de intentos. Abandonando.");
        unsigned long startAPIErrLed = millis();  // Tiempo de inicio de errLed
        errLed();
        unsigned long endAPIErrLed = millis();  // Tiempo de fin de errLed
        Serial.print("Tiempo de inicio de errLed: ");
        Serial.println(startAPIErrLed);
        Serial.print("Tiempo de fin de errLed: ");
        Serial.println(endAPIErrLed);
        while (true) {
          ESP.restart();
          //while (1);
          //return;
        }
      }
      return;
    }
  }

  // Verifica si ha pasado 10 minutos desde el último envío (cada 10 minutos)
  unsigned long startSD = millis();  // Tiempo de inicio de calculoGeneralCSV
  if (currentMillis - previousMillisSD >= intervalSD) {
    // Actualiza el tiempo del último envío
    previousMillisSD = currentMillis;

    DateTime fecha = DS1307_RTC.now();          // RTC
    unsigned long startRuidoDataSD = millis();  // Tiempo de inicio de ruidoData (SD)
    decibeles = ruidoData();
    unsigned long endRuidoDataSD = millis();  // Tiempo de fin de ruidoData (SD)
   Serial.print("Tiempo de inicio de ruidoData (SD): ");
    Serial.println(startRuidoDataSD);
    Serial.print("Tiempo de fin de ruidoData (SD): ");
    Serial.println(endRuidoDataSD);

    unsigned long startReturnLatSD = millis();  // Tiempo de inicio de returnLat (SD)
    latitud = returnLat();
    unsigned long endReturnLatSD = millis();  // Tiempo de fin de returnLat (SD)
    Serial.print("Tiempo de inicio de returnLat (SD): ");
    Serial.println(startReturnLatSD);
    Serial.print("Tiempo de fin de returnLat (SD): ");
    Serial.println(endReturnLatSD);

    unsigned long startReturnLngSD = millis();  // Tiempo de inicio de returnLng (SD)
    longitud = returnLng();
    unsigned long endReturnLngSD = millis();  // Tiempo de fin de returnLng (SD)
    Serial.print("Tiempo de inicio de returnLng (SD): ");
    Serial.println(startReturnLngSD);
    Serial.print("Tiempo de fin de returnLng (SD): ");
    Serial.println(endReturnLngSD);

    if (decibeles != "0" && latitud != "0.000000" && longitud != "0.000000") {
      unsigned long startSDCalc = millis();  // Tiempo de inicio de calculoGeneralCSV
      calculoGeneralCSV(decibeles, latitud, longitud, fecha, createcsv[0]);
      unsigned long endSDCalc = millis();  // Tiempo de fin de calculoGeneralCSV
      Serial.print("Tiempo de inicio de calculoGeneralCSV: ");
      Serial.println(startSDCalc);
      Serial.print("Tiempo de fin de calculoGeneralCSV: ");
      Serial.println(endSDCalc);

      unsigned long startSDOkLed = millis();  // Tiempo de inicio de okLed (SD)
      okLed();
      unsigned long endSDOkLed = millis();  // Tiempo de fin de okLed (SD)
      Serial.print("Tiempo de inicio de okLed (SD): ");
      Serial.println(startSDOkLed);
      Serial.print("Tiempo de fin de okLed (SD): ");
      Serial.println(endSDOkLed);
    } else {
      Serial.println("El GPS arroja valores 'INVALID' que no adecuados para el proceso");
      unsigned long startSDErrGPS = millis();  // Tiempo de inicio de errGPS (SD)
      errGPS();
      unsigned long endSDErrGPS = millis();  // Tiempo de fin de errGPS (SD)
      Serial.print("Tiempo de inicio de errGPS (SD): ");
      Serial.println(startSDErrGPS);
      Serial.print("Tiempo de fin de errGPS (SD): ");
      Serial.println(endSDErrGPS);

      // Puedes aumentar el contador de intentos y salir si se supera el máximo
      currentAttempts++;
      if (currentAttempts >= maxAttempts) {
        Serial.println("Se superó el número máximo de intentos. Abandonando.");
        unsigned long startSDErrLed = millis();  // Tiempo de inicio de errLed (SD)
        errLed();
        unsigned long endSDErrLed = millis();  // Tiempo de fin de errLed (SD)
        Serial.print("Tiempo de inicio de errLed (SD): ");
        Serial.println(startSDErrLed);
        Serial.print("Tiempo de fin de errLed (SD): ");
        Serial.println(endSDErrLed);
        while (true) {
          //while (1);
          ESP.restart();
          //return;
        }
      }
      return;
    }
  }
  // Reinicia el contador de intentos si llegamos aquí
  currentAttempts = 0;
}
