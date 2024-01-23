#include <SPI.h>
#include <SdFat.h>
#include "indicador.h"

#define SPI_SPEED_SD 4000000 // 4MHz
#define CS 5

SdFat SD;
File dataFile;

bool CreateCSV(String createcsv[])
{
  if (!SD.begin(CS, SPI_SPEED_SD))
  {
    Serial.println("Error al inicializar la tarjeta SD.");
    errLed();
    while (1)
      ;
    return false;
  }
  else
  {
    for (int i = 0; i < 1; i++)
    {
      delay(100);
      if (!SD.exists(createcsv[i]))
      {
        dataFile = SD.open(createcsv[i], FILE_WRITE);
        if (dataFile)
        {
          dataFile.println("Medida de ruido, unidad de la medida de ruido , Lat, Lng , Fecha , Hora ");
          Serial.print("Archivo ");
          Serial.print(createcsv[i]);
          Serial.println(" creado.");
          dataFile.close(); // Cierra el archivo después de escribir.
          okLed();
          return true; // Sale de la función indicando éxito.
        }
        else
        {
          Serial.println("Error al abrir el archivo para creación.");
          errLed();
          return false; // Sale de la función indicando fallo.
        }
      }
    }
  }
  return true; // Si el archivo ya existe, también indica fallo.
}

void calculoGeneralCSV(String db, String lat, String lng, DateTime fecha, String archivo)
{
  if (!SD.begin(CS, SPI_SPEED_SD))
  {
    Serial.println("Error al inicializar la tarjeta SD.");
    errLed();
  }
  else
  {
    dataFile = SD.open(archivo, FILE_WRITE);
    if (dataFile)
    {
      okLed();
      dataFile.print(db);
      dataFile.print(","); // csv separado por comas
      dataFile.print("dB");
      dataFile.print(",");
      dataFile.print(lat);
      dataFile.print(",");
      dataFile.print(lng);
      dataFile.print(",");
      dataFile.print(fecha.year(), DEC);
      dataFile.print('/');
      dataFile.print(fecha.month(), DEC);
      dataFile.print('/');
      dataFile.print(fecha.day(), DEC);
      dataFile.print(",");
      dataFile.print(fecha.hour(), DEC);
      dataFile.print(':');
      dataFile.print(fecha.minute(), DEC);
      dataFile.print(':');
      dataFile.print(fecha.second(), DEC);
      dataFile.println();
      dataFile.close();
      Serial.println("Información guardada en la tarjeta SD.");
    }
    else
    {
      Serial.println("Error al abrir el archivo para escritura.");
      errLed();
    }
    dataFile.close();
  }
}