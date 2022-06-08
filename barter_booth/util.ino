#include <ESP8266WiFi.h>

// Logging pour l'information
void logInfo(const String text) { logInfo(text, false); }
void logInfo(const String text, bool separator) {
  if (Serial) {
    if (text != "") Serial.println("[I] " + text);  
    if (separator) Serial.println("[I] ========================================");
  }
}

// Logging pour l'erreur
void logError(const String text) { logError(text, false); }
void logError(const String text, bool separator) {
  if (Serial) {
    if (text != "") Serial.println("[E] " + text);   
    if (separator) Serial.println("[E] ========================================");
  }
}

void failedSignal() {
  logError("Devices Initialization failed!");
  while (1) delay(1);
}

char* clientRequest(byte packetType, byte *content) {
  
  
  return NULL;
}
