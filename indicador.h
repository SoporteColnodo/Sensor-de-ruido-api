#ifndef INDICADOR_H
#define INDICADOR_H

#define indicadorPinOk 13
#define indicadorPinError 27
#define indicadorPinGPS 25


void alertsSetup(){
  pinMode(indicadorPinOk, OUTPUT);
  pinMode(indicadorPinError, OUTPUT);
  pinMode(indicadorPinGPS, OUTPUT);
}

void okLed() {
  digitalWrite(indicadorPinOk, HIGH);
  digitalWrite(indicadorPinError, LOW);
  digitalWrite(indicadorPinGPS, LOW);
}

void errLed() {
  digitalWrite(indicadorPinError, HIGH);
  digitalWrite(indicadorPinOk, LOW);
  digitalWrite(indicadorPinGPS, LOW);
}

void errGPS() {
  digitalWrite(indicadorPinGPS, HIGH);//Cuando GPS no trasmite datos
  digitalWrite(indicadorPinOk, LOW);
  digitalWrite(indicadorPinError, LOW);
}

#endif // INDICADOR_H