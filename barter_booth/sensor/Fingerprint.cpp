#include <FingerPrint.h>

Fingerprint::Fingerprint(uint8_t rx, uint8_t tx, uint32_t baudRate) { Fingerprint(rx, tx, baudRate, 0x0000000); }
Fingerprint::Fingerprint(uint8_t rx, uint8_t tx, uint32_t baudRate, uint32_t password) {
  _serial = new SoftwareSerial(rx, tx);
  _sensor = new Adafruit_Fingerprint(_serial, password);

  _finfo = "INFO: %s", _ferror = "ERROR: %s";
  _logging = false;
  
  _slot = 1, _i = 0, _p = 0, _error = FINGERPRINT_OK;
  _baudrate = baudRate;
  memset(_reads, 0xff, 256); 
}

bool Fingerprint::initSensor() {
  logInfo("", true);
  logInfo("Fingerprint sensor initialisation ...");

  _serial.setTimeout(2000);
  _sensor.begin(baudrate);
  delay(10);
  if (finger.verifyPassword()) logInfo("Found fingerprint sensor!");
  else {
    logError("Password don't match or sensor not connected!", true);
    return false;
  }

  logInfo(" ");
  logInfo("Reading sensor parameters ...");
  if (finger.getParameters() != FINGERPRINT_OK) {
    logError("Failed to read parameters! Device not connected.", true);
    return false;
  }
  printInfos();

  logInfo(" ");
  logInfo("Initialization success.", true);
  delay(100);
  return true; // Initialization success
}

void Fingerprint::printInfos() {
  _sensor.getTemplateCount();
  logInfo("Status: 0x" + String(_sensor.status_reg, HEX));
  logInfo("Sys ID: 0x" + String(_sensor.system_id, HEX));
  logInfo("Capacity: " + String(_sensor.capacity));
  logInfo("Security level: " + String(_sensor.security_level));
  logInfo("Device address: 0x" + String(_sensor.device_addr, HEX));
  logInfo("Packet size: " + String(_sensor.packet_len) + " bytes");
  logInfo("Sensor Baud rate: " + String(_sensor.baud_rate) + " bits/s");
  logInfo("Internal baud rate: " + String(_baudRate) + " bits/s");
  logInfo("Saved templates: " + String(_sensor.templateCount));
}

uint8_t Fingerprint::getFingerprint() {
  logInfo("Getting image ...");

  for (_i=0; i<FINGERPRINT_TESTTIMEOUT; _i++) {
    _p = _sensor.getImage();
    logInfo(_p == FINGERPRINT_OK ? "Image taken" : _p == FINGERPRINT_NOFINGER ? "." : _p == FINGERPRINT_PACKETRECIEVEERR ? "Communication error " : _p == FINGERPRINT_IMAGEFAIL ? "Imaging error" : "Unknown error");
    if (_p == FINGERPRINT_OK) {
      for (uint two=1; two<3; two++) {
        _p = fingerAnalysis(two);
        if (_p != FINGERPRINT_OK) break;
      }
      return _p;
    }
  }
  
  if (_p == FINGERPRINT_NOFINGER) {
    logError("FINGER_GET_TIME_OUT: no finger");
    return _p;
  } else {
    logError("FINGER_GET_TIME_OUT: to many error");
    return FINGERPRINT_TOMANYERROR;    
  }
}

void Fingerprint::waitNoFinger() {
  while (_p != FINGERPRINT_NOFINGER) _p = _sensor.getImage();
}

uint8_t Fingerprint::fingerAnalysis(uint8_t slot) {
  logInfo("Converting image ...");

  for (_i=0; _i<FINGERPRINT_TESTTIMEOUT; _i++) {
    _p = _sensor.image2Tz(slot);
    if (_p == FINGERPRINT_OK) {
      logInfo("Image converted");
      return _p;
    } else if (_p == FINGERPRINT_FEATUREFAIL || _p == FINGERPRINT_INVALIDIMAGE || _p == FINGERPRINT_IMAGEMESS) {
      logError("Could not find fingerprint features");
      return FINGERPRINT_IMAGEMESS;
    } else logError(_p == FINGERPRINT_PACKETRECIEVEERR ? "Communication error" : "Unknown error");
  }
  logError("FINGER_GET_TIME_OUT: to many error");
  return FINGERPRINT_TOMANYERROR;
}

uint8_t Fingerprint::createModel() {
  logInfo("Creating model ...")

  for (_i=0; _i<FINGERPRINT_TESTTIMEOUT; _i++) {
    _p = _sensor.createModel();
    if (_p == FINGERPRINT_OK) {
      logInfo("Prints matched!");
      return _p;
    } else if (_p == FINGERPRINT_ENROLLMISMATCH) {
      logError("Fingerprints did not match");
      return _p;
    } else logError(_p == FINGERPRINT_PACKETRECIEVEERR ? "Communication error" : "Unknown error");
  }
  logError("FINGER_GET_TIME_OUT: to many error");
}

uint8_t Fingerprint::enrollFingerprint() {
  
}

byte[256] Fingerprint::getModelData() {
  memset(_reads, 0xff, 256); 

  logInfo("Attempting to get template");
  _sensor.getModel();
  logIngo("Received " + String(_serial.readBytes(reads, 256)) + " bytes / 256 bytes");
  return _reads;
}


/* ########## Logging methods ########## */


void Fingerprint::enableLogging(bool enable) {
  logInfo("Logging " + String(enabled ? "enabled" : "disabled"))
  _logging = enable;
}

void Fingerprint::setLoggingFormat(const String info, const String error) {
  _finfo = info;
  _ferror = error;
}

void Fingerprint::logInfo(const String text) { logInfo(text, false); }
void Fingerprint::logInfo(const String text, bool separator) {
  if (_logging && Serial) {
    if (text != "") Serial.println(_finfo % text);
    if (separator) Serial.println(_finfo % "========================================");
  }
}

void Fingerprint::logError(const String text) { logError(text, false); }
void Fingerprint::logError(const String text, bool separator) {
  if (_logging && Serial) {
    if (text != "") Serial.println(_ferror % text);
    if (separator) Serial.println(_ferror % "========================================");
  }
}
