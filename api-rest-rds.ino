#include "RTC_Utils.hpp"
#include <TinyGPSPlus.h>
#include "config.h"
#include "indicador.h"
#include "Wifi_Utils.hpp"
#include "SD_Utils.hpp"
#include "API.hpp"
#include <SoftwareSerial.h>

//variables inicializar librerias y seriales
//HardwareSerial Serial2(2);
TinyGPSPlus gps;

// variables
//bool mostrarIndicador = true;

const int SENSOR_PIN = 15;
double db = 0.0;   // decibel
double latitud = 0.0;   // latitud
double longitud = 0.0;  // longitud
unsigned long previousMillis = 0;
const long interval = 5000;  // Intervalo de tiempo en milisegundos (5 segundos)
String ssid = "";
String password = "";

DateTime fecha;

void setup() {
  RTCSet();
  SDSet();
  Serial.begin(9600);  
  Serial2.begin(9600); 
  Serial.println("Sensor ruido");
  Serial.println("iniciando...");
  delay(2000);
  alertsSetup();
  fecha = DS1307_RTC.now();
  uint32_t milisegundosEnHora = fecha.minute() * 60000 + fecha.second() * 1000;
  maxciclos = (hora - milisegundosEnHora) / intervalSD;

 
}

void loop() {

ssid = LeerSSID();
password = LeerPassword();
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ruidoData();
    generarLatLng(); // muestra latitud longitud   
    Serial.print("db: ");
    Serial.println(db);
    Serial.print("Latitud: ");
    Serial.println(latitud, 5);
    Serial.print("Longitud: ");
    Serial.println(longitud, 5);  
    okLed();
     
  }else{
Serial.println("Error: GPS no proporciona valores válidos para la API.");
errGPS();    
  }

if (currentMillis - previousMillisSD >= intervalSD) {
static int contador = 0; 
static double promediodb = 0.0;
static String nombreArchivo;

previousMillisSD = currentMillis;
DateTime fecha = DS1307_RTC.now();
 Serial.print("Maxciclos: ");
Serial.println(maxciclos);
if (latitud != 0.0 && longitud != 0.0 && db != 0.0 ) {
nombreArchivo = String(fecha.day()) + "-" + String(fecha.month()) + "-" + String(fecha.year()) +  "-" + String(fecha.hour()) + ".csv";
crearArchivo(nombreArchivo);
guardaInformacionCSV(latitud, longitud, db, fecha, nombreArchivo, idDispositivo);
okLed();

//temporal mayo
ConnectWiFi_STA(false,ssid,password);
CrearRegistro(latitud, longitud, db, LoginUser(User, Password));
WiFi.disconnect(true);
okLed();
//temporal mayo

contador++;
if (contador >= maxciclos) {
promediodb = calcularPromediodb(nombreArchivo,maxciclos,"db");// se lee archivo y calculan promedios
if(ConnectWiFi_STA(false,ssid,password) && promediodb > 0.0 ){
CrearRegistro(latitud, longitud, promediodb, LoginUser(User, Password));
WiFi.disconnect(true);
okLed();//indicador
}else{
crearArchivo("informacion_manual_aa.csv");
guardaInformacionCSV(latitud, longitud, promediodb, fecha, "informacion_manual_aa.csv", idDispositivo);
errLed();

}
contador = 0;
}
okLed();
} else {
Serial.println("Error: GPS no proporciona valores válidos para la API.");
errGPS();
currentAttempts++;
if (currentAttempts >= maxAttempts) {
Serial.println("Se superó el número máximo de intentos. Reiniciando.");
while (true) {
ESP.restart();
}
}
}
}
okLed();
delay(5000); 
}


void generarLatLng() {
while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      latitud = gps.location.lat();
      longitud = gps.location.lng();     
    }else{
      errGPS();
    }
  }  
}

void ruidoData() {
  const int sampleWindow = 10;
  unsigned int sample;
  unsigned long startMillis = millis();
  float peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 4095;
  // Tomar muestras durante un intervalo de tiempo
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SENSOR_PIN);
    if (sample < 4095) {
      if (sample > signalMax) {
        signalMax = sample;
      } else if (sample < signalMin) {
        signalMin = sample;
      }
    }
  }
  // Calcular la amplitud de pico a pico
  peakToPeak = signalMax - signalMin;

  // Mapear la amplitud a un valor de decibelios
  db = map(peakToPeak, 20, 4000, 90, 49.5);
  if (db < 49.5) {
    db = 49.5;
  }
}
