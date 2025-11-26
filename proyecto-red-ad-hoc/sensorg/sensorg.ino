#include <TinyGPSPlus.h>

// Crea objeto TnyGPS
TinyGPSPlus gps;

//Activa el Serial del GPS
HardwareSerial GPSserial(2);

void setup() {
  // Inicia las terminales Serial de la ESP y el GPS
  Serial.begin(115200);
  GPSserial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Iniciando GPS...");
}

void loop() {
  // Si el GPS está disponible, detecte lecturas en el Serial del GPS
  while (GPSserial.available()) {
    gps.encode(GPSserial.read());
  }

  // Si el GPS detecta una posición, envía latitud, longitud, el satélite que utiliza
  if (gps.location.isValid()) {
    Serial.print("Latitud: ");
    Serial.println(gps.location.lat(), 6);

    Serial.print("Longitud: ");
    Serial.println(gps.location.lng(), 6);

    Serial.print("Satélites: ");
    Serial.println(gps.satellites.value());

    Serial.println("--------------------------");
  } else {
    // Si no encuetra satétile, espera la señal
    Serial.println("Esperando señal GPS...");
  }

  delay(1000);
}
