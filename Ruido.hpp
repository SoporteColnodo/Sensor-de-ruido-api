const int SENSOR_PIN = 15;

String ruidoData(){
  const int sampleWindow = 10;
  unsigned int sample;
  unsigned long startMillis = millis();
  float peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 4095;
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
  peakToPeak = signalMax - signalMin;
  int db = map(peakToPeak, 20, 4000, 90, 49.5);
  if (db < 49.5) {
    db = 49.5;
  }
  return String(db);
}