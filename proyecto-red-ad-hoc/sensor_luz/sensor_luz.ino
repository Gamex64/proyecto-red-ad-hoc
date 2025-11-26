#include <Wire.h>
#include <BH1750.h>

// --- CONFIGURACIÓN DE LA RED AD-HOC Y MQTT ---
// El ID de nodo es CRÍTICO, debe ser único para cada dispositivo
const long NODE_ID = 189247256; // ¡CAMBIA ESTE ID! Debe ser diferente a 189247261

// --- CONFIGURACIÓN DEL SENSOR BH1750 ---
BH1750 lightMeter;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Iniciar la comunicación I2C para el sensor
  Wire.begin();
  
  // Iniciar el sensor BH1750. 
  // BH1750::CONTINUOUS_HIGH_RES_MODE es el modo predeterminado, mide continuamente.
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 iniciado correctamente.");
  } else {
    Serial.println("Error al iniciar BH1750. Verifica las conexiones I2C.");
  }
}

void loop() {
  // --- 1. Lectura de Luminosidad (LUX) ---
  float lux = lightMeter.readLightLevel();

  // Verifica si la lectura es válida. BH1750::ERROR_FLOAT es -1.0
  if (lux >= 0.0) {
    // --- 2. Construcción del JSON ---
    // El formato JSON debe ser idéntico al anterior para que Node-RED lo procese
    // {"nodo":189247262, "lux":150.5}

    String jsonPayload = "{";
    jsonPayload += "\"nodo\":";
    jsonPayload += NODE_ID;
    
    jsonPayload += ",\"lux\":";
    jsonPayload += String(lux, 1); // 1 decimal de precisión para LUX
    
    jsonPayload += "}";

    // --- 3. Envío por Serial ---
    Serial.println(jsonPayload);
  } else {
    Serial.println("Error de lectura del sensor BH1750.");
  }

  // Esperar 10 segundos antes de la siguiente lectura
  delay(10000); 
}