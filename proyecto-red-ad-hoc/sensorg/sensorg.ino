#include <TinyGPSPlus.h>

TinyGPSPlus gps;

HardwareSerial GPSserial(2);

void setup() {
  Serial.begin(115200);
  GPSserial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Iniciando GPS...");
}

void loop() {
  while (GPSserial.available()) {
    gps.encode(GPSserial.read());
  }

  if (gps.location.isValid()) {
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());

    Serial.println("--------------------------");
  } else {
    Serial.println("Esperando señal GPS...");
  }

  delay(1000);
}
