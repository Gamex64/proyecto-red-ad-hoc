#include "painlessMesh.h"

// ----------------- CONFIGURACIÓN MESH -----------------
#define MESH_PREFIX     "RedAgricola"
#define MESH_PASSWORD   "cultivo123"
#define MESH_PORT       5555

painlessMesh mesh;

// ----------------- CONFIGURACIÓN DEL SENSOR DE HUMEDAD DEL SUELO -----------------
// Pin Analógico conectado al A0 del módulo. (Ejemplo: GPIO 34 para ESP32)
const int SOIL_MOISTURE_PIN = 34; 

// Calibración del Sensor (AJUSTAR ESTOS VALORES)
// Estos valores definen el 0% (seco) y 100% (húmedo) de la humedad.
const int AIR_DRY_VALUE = 3800;  // Valor ADC típico en aire seco (0% humedad)
const int WATER_WET_VALUE = 1200; // Valor ADC típico sumergido en agua (100% humedad)


// ----------------- FUNCIONES DE ENVÍO DE DATOS -----------------
void sendSensorData() {
  // 1. Lectura del sensor (Valor ADC crudo)
  int rawSoilMoisture = analogRead(SOIL_MOISTURE_PIN);

  // 2. Conversión a Porcentaje (0-100%)
  // Se usa map con valores invertidos porque a menor ADC, mayor humedad.
  int moisturePercent = map(rawSoilMoisture, AIR_DRY_VALUE, WATER_WET_VALUE, 0, 100);
  
  // Aseguramos que el porcentaje esté dentro de los límites
  moisturePercent = constrain(moisturePercent, 0, 100);

  uint32_t nodeId = mesh.getNodeId(); // Obtener el ID del nodo

  // *** CONSTRUCCIÓN DEL JSON ***
  // Incluye el ID del nodo, el porcentaje procesado y el valor crudo para debug.
  String payload = "{\"nodo\":" + String(nodeId) + 
                   ",\"soil\":" + String(moisturePercent) + 
                   ",\"soil_raw\":" + String(rawSoilMoisture) + "}";

  mesh.sendBroadcast(payload);
  Serial.println("📤 Enviado a la red mesh: " + payload);
}

// ----------------- CALLBACKS DE MESH -----------------
void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("📩 Mensaje recibido de nodo %u: %s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("🔗 Nuevo nodo conectado: %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.println("🔄 Cambios en la red mesh detectados");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("🕒 Tiempo ajustado por la red: %d\n", offset);
}

// ----------------- SETUP -----------------
unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  delay(3000); 
  Serial.println("\n=== Nodo Sensor Humedad del Suelo iniciado ===");

  // Configuración del pin analógico
  pinMode(SOIL_MOISTURE_PIN, INPUT); 
  
  // Inicializa la red mesh en modo STA (solo nodo)
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_STA);

  // Callbacks
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  Serial.println("✅ Red mesh inicializada, buscando nodo root...");
}

// ----------------- LOOP -----------------
void loop() {
  mesh.update();

  // Enviar datos cada 60 segundos
  if (millis() - lastSend > 7000) {
    sendSensorData();
    lastSend = millis();
  }
}