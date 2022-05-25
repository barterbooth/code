/* DOCUMENTATION */
/* Fingerprint Sensor Datasheet: https://www.velleman.eu/downloads/29/infosheets/vma329_datasheet.pdf
 * Fingerprint Library: https://github.com/DFRobot/DFRobot_ID809
 * Display Device Datasheet: 
 * Display Library: https://github.com/4dsystems/Diablo16-Serial-Arduino-Library
*/

/* IMPORTATION DES LIBRAIRIES */
#include <Adafruit_Fingerprint.h>
#include <Diablo_Serial_4DLib.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>


/* CONSTANTES */
// Vitesses de transmission
#define FINGER_BAUD_RATE 115200
#define DISPLAY_BAUD_RATE BAUD_115200
#define DEBUG_BAUD_RATE 74880

// Broches
#define FINGER_TX 14
#define FINGER_RX 12
#define DISPLAY_TX 4
#define DISPLAY_RX 5

// Internet
#define CONNECT_MAX_REQUEST 30
#define WIFI_SSID "laboSIN"
#define WIFI_PASSWORD "sti2d_vauban"
//#define WEB_HOST "zetamap.github.io:80"

// Erreurs
#define FINGER_TIME_OUT 50
#define NO_FINGER 0
#define USER_NOT_FOUND 1
#define USER_FOUND 2
#define TO_MANY_ERROR 3

// Codes d'émulation IHM
#define NO_ACTION 0
#define FINGER_GET 1
#define FINGER_ENROOL 2
#define USER_CONNECT 3
#define USER_DISCONNECT 4
#define WATCH_CATALOG 5


/* VARIABLES */
bool screenInitialised = false;


/* OBJETS */
// Création des ports séries virtuel pour les périphériques
SoftwareSerial fingerSerial(FINGER_TX, FINGER_RX); // Port série du capteur d'empreinte digital
SoftwareSerial displaySerial(DISPLAY_TX, DISPLAY_RX); // Port série de l'écran tactile
WiFiClient webClient;

Adafruit_Fingerprint finger(&fingerSerial);
Diablo_Serial_4DLib display(&displaySerial);

void setup() {
  Serial.begin(DEBUG_BAUD_RATE);
#ifndef WEB_HOST
  String WEB_HOST = "";
  logInfo("No host specified! Please type an host ...");
  while (WEB_HOST == "") {
    if (Serial.available()) WEB_HOST = Serial.readString();
  }
  WEB_HOST.replace("\r\n", "");
  WEB_HOST.trim();
  logInfo("Selected host: " + WEB_HOST);
#endif
  logInfo("Devices Initialization ...");
  
  if (!initFingerSensor(finger, FINGER_BAUD_RATE)) failedSignal();
  if (!initDisplay(display, DISPLAY_BAUD_RATE)) failedSignal();
  if (!initWebClient(webClient, WIFI_SSID, WIFI_PASSWORD, WEB_HOST)) failedSignal();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (Serial.parseInt()) {
    case NO_ACTION: default: // No processing
      break;

    case FINGER_GET:
      logInfo("Method return: " + String(getFingerprint(finger, false)));
      break;

    case FINGER_ENROOL:
      logInfo("Method return: " + String(enrollFingerprint(finger)));
      break;

    case USER_CONNECT:
      logInfo("Method return: " + String(userConnection(finger)));
      break;

    case USER_DISCONNECT:
      logError("No implemented!");
      break;

    case WATCH_CATALOG:
      logError("No implemented!");
      break;
  }
}
