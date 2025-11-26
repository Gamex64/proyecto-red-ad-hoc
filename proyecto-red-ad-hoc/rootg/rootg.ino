#include "painlessMesh.h"

// ---------------------------
// CONFIGURACIÓN DE LA MALLA
// ---------------------------
#define MESH_PREFIX     "RedAgricola"
#define MESH_PASSWORD   "cultivo123"
#define MESH_PORT       5555

painlessMesh mesh;

// ---------------------------
// CALLBACK MESH: MENSAJES RECIBIDOS (SOLO REENVÍA JSON LIMPIO)
// ---------------------------
void receivedCallback(uint32_t from, String &msg) {
  // Verificamos si el mensaje recibido comienza con '{' y termina con '}'
  // Esto filtra los mensajes internos de la malla (control, ACK, etc.).
  if (msg.startsWith("{") && msg.endsWith("}")) {
    // Si es JSON válido, lo enviamos al puerto Serial para que Node-RED lo lea.
    Serial.println(msg); 
  }
  // No hay un 'else' ni Serial.printf() aquí para garantizar la limpieza total del Serial.
}

void newConnectionCallback(uint32_t nodeId) {
  // Esto es solo para tu debug en el monitor Serial del IDE, si está abierto.
  // No afecta a Node-RED porque mesh.setDebugMsgTypes(NONE) lo ignora.
  // Pero lo dejamos si quieres ver las conexiones cuando no uses Node-RED.
  //Serial.printf("🔗 MESH CONNECTED: %u\n", nodeId);
}

// ---------------------------
// SETUP
// ---------------------------
// ---------------------------
// SETUP
// ---------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  // ANTES (Error): mesh.setDebugMsgTypes(NONE);
  // AHORA (Correcto): Usamos el prefijo de la librería para acceder al tipo NONE.
  mesh.setDebugMsgTypes(0); // Equivalente a NONE
  // Inicializa la red Mesh en modo AP (Access Point)
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT); 
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  
  Serial.println("=== ROOT GATEWAY listo para enviar JSON ===");
}

// ---------------------------
// LOOP PRINCIPAL
// ---------------------------
void loop() {
  mesh.update();
}