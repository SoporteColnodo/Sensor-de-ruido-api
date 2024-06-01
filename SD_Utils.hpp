#include <SPI.h>
#include <SdFat.h>
#include "indicador.h"

#define SPI_SPEED_SD 4000000 // 4MHz
#define CS 5

SdFat SD;
File dataFile;

void SDSet(){
  if (!SD.begin(CS, SPI_SPEED_SD))
  {
    Serial.println("Error al inicializar la tarjeta SD.");
    errLed();
    while (1);
  }
}

bool crearArchivo(String archivo)
{
  if (!SD.begin(CS, SPI_SPEED_SD))
  {
    Serial.println("Error al inicializar la tarjeta SD.");
    errLed();
    return false;
  }else {
    delay(100);
  if (SD.exists(archivo))
  {    

  dataFile = SD.open(archivo, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println("Medida de ruido, Lat, Lng , idDispositivo, FechaEpoch, Fecha , Hora ");
    dataFile.close(); // Cierra el archivo después de escribir.
    Serial.println("Archivo creado: " + archivo);
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
  return true; // Si el archivo ya existe, también indica fallo.
}//end bool



//guardad datos en csv 
void guardaInformacionCSV(double lat, double lng, double db, DateTime fecha, String archivo, String idDispositivo)
{
  if (!SD.begin(CS, SPI_SPEED_SD))
  {
    Serial.println("Error al inicializar la tarjeta SD.");
    errLed();
    return;
  }
else{

  // Abrir el archivo original en modo escritura
  dataFile = SD.open(archivo, FILE_WRITE);
  if (!dataFile)
  {


      // Calculamos el epoch time a partir del objeto DateTime
      uint32_t epochTime = fecha.unixtime();
  
  okLed();
  // Escribir los datos en el archivo original
  dataFile.print("Ruido-");
  dataFile.print(epochTime);
  dataFile.print(",");
  dataFile.print("ruido");
  dataFile.print(",");
  dataFile.print(db);
  dataFile.print(","); // csv separado por comas
  dataFile.print(lat, 6);
  dataFile.print(",");
  dataFile.print(lng, 6);
  dataFile.print(",");
  dataFile.print(idDispositivo);
  dataFile.print(",");
  dataFile.print(epochTime); // Guardamos el epoch time
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

  // Cerrar el archivo original
  dataFile.close();
  Serial.println("Información guardada en el archivo: " + archivo);
  okLed();
}else
{
    Serial.println("Error al abrir el archivo para escritura.");
    errLed();
    return;
  }
  dataFile.close();
}
}

double calcularPromediodb(String archivo, int ciclos, String db) {
  File dataFile = SD.open(archivo);

  if (dataFile) {
    Serial.println("Calculando...");
    String encabezados;
    String valoresdb;
    float sum = 0.0;
    int count = 0;
    int cuentaFila = 0;

    while (dataFile.available()) {
      encabezados = dataFile.readStringUntil('\n');
      cuentaFila++;

      if (cuentaFila >= 2 && cuentaFila <= ciclos+1) {
        size_t commaIndex = encabezados.indexOf(',');

        if (commaIndex != -1) {
          if (db == "db") {
            valoresdb = encabezados.substring(0, commaIndex);
          } else {
            size_t secondCommaIndex = encabezados.indexOf(',', commaIndex + 1);
            
            if (secondCommaIndex != -1) {
              valoresdb = encabezados.substring(commaIndex + 1, secondCommaIndex);
            } else {
              valoresdb = encabezados.substring(commaIndex + 1);
            }
          }

          Serial.println("valoresdb: ");
          Serial.print(valoresdb);

          float dB = valoresdb.toFloat();
          sum += dB;
          Serial.println(" suma: ");
          Serial.print(sum);
          count++;
        } else {
          Serial.println("La línea no contiene una coma.");
        }
      }

      if (cuentaFila == ciclos+1) {
        break;
      }
    }
    dataFile.close();

    if (count > 0) {
      double average = sum / count;
      Serial.print("Promedio de 'dB' del archivo CSV: ");
     char buffer[10];
      snprintf(buffer, sizeof(buffer), "%.2f", average);
      Serial.println(buffer);
      return atof(buffer);
    } else {
      Serial.println("No se encontraron valores en el rango de filas en la columna 'dB' ");
    }
  } else {
    Serial.println("Error al abrir el archivo CSV.");
    errLed();
  }
  return 0.0;
}

double calcularModaLocalizacion(String archivo,int ciclos, String db) {
  File dataFile = SD.open(archivo);

  if (dataFile) {
    Serial.println("Calculando...");
    String encabezados;
    String valoresdb;
    float sum = 0.0;
    int count = 0;
    int cuentaFila = 0;

    while (dataFile.available()) {
      encabezados = dataFile.readStringUntil('\n');
      cuentaFila++;

      if (cuentaFila >= 2 && cuentaFila <= ciclos+1) {
        size_t commaIndex = encabezados.indexOf(',');

        if (commaIndex != -1) {
          if (db == "db") {
            valoresdb = encabezados.substring(0, commaIndex);
          } else {
            size_t secondCommaIndex = encabezados.indexOf(',', commaIndex + 1);
            
            if (secondCommaIndex != -1) {
              valoresdb = encabezados.substring(commaIndex + 1, secondCommaIndex);
            } else {
              valoresdb = encabezados.substring(commaIndex + 1);
            }
          }

          Serial.println("valoresdb: ");
          Serial.print(valoresdb);

          float dB = valoresdb.toFloat();
          sum += dB;
          Serial.println(" suma: ");
          Serial.print(sum);
          count++;
        } else {
          Serial.println("La línea no contiene una coma.");
        }
      }

      if (cuentaFila == ciclos+1) {
        break;
      }
    }
    dataFile.close();

    if (count > 0) {
      double average = sum / count;
      Serial.print("Promedio de 'dB' del archivo CSV: ");
      Serial.println(average, 2);
      return average;
    } else {
      Serial.println("No se encontraron valores en el rango de filas en la columna 'dB' ");
    }
  } else {
    Serial.println("Error al abrir el archivo CSV.");
    errLed();
  }
  return 0.0;
}

void logErrorWifi(DateTime fecha, String archivo)
{
  if (!SD.begin(CS, SPI_SPEED_SD))
  {
    Serial.println("Error al inicializar la tarjeta SD.");
    errLed();
    return;
  }
  else
  {

  // Abrir el archivo para escritura
  dataFile = SD.open(archivo, FILE_WRITE);
  if (!dataFile)
  {
    

  // Escribir la información en el archivo
  dataFile.print("Hubo un error al conectarse a la red WIFI");
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

  // Cerrar el archivo
  dataFile.close();

  Serial.println("Error registrado en el archivo: " + archivo);
  okLed();
}else{

  Serial.println("Error al abrir el archivo para escritura.");
    errLed();
    return;
  }
  dataFile.close();
  }
}





String LeerSSID()
{
    File dataFile = SD.open("config.txt");
    if(dataFile)
    {
        // Leer el SSID
        String ssidLinea = dataFile.readStringUntil('\n');
        int primerComillaSSID = ssidLinea.indexOf('"') + 1;
        int segundaComillaSSID = ssidLinea.indexOf('"', primerComillaSSID);
        String ssid = ssidLinea.substring(primerComillaSSID, segundaComillaSSID);
        // Cerrar el archivo
        dataFile.close();
        return ssid;
    }
    else
    {
        Serial.println("Error al abrir el archivo para lectura.");
        errLed();
        return ""; // Retornar una cadena vacía en caso de error
    }
}

String LeerPassword()
{
    File dataFile = SD.open("config.txt");
    if(dataFile)
    {
        // Saltar la línea del SSID
        dataFile.readStringUntil('\n');
        // Leer la contraseña
        String passwordLinea = dataFile.readStringUntil('\n');
        int primerComillaPass = passwordLinea.indexOf('"') + 1;
        int segundaComillaPass = passwordLinea.indexOf('"', primerComillaPass);
        String password = passwordLinea.substring(primerComillaPass, segundaComillaPass);
        // Cerrar el archivo
        dataFile.close();
        // Retornar la contraseña
        return password;
    }
    else
    {
        Serial.println("Error al abrir el archivo para lectura.");
        errLed();
        return ""; // Retornar una cadena vacía en caso de error
    }
}