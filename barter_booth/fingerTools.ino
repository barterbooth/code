#include <Adafruit_Fingerprint.h>

void printFingerInfos(Adafruit_Fingerprint &sensor) {
  sensor.getTemplateCount();
  logInfo("Status: 0x" + String(sensor.status_reg, HEX));
  logInfo("Sys ID: 0x" + String(sensor.system_id, HEX));
  logInfo("Capacity: " + String(sensor.capacity));
  logInfo("Security level: " + String(sensor.security_level));
  logInfo("Device address: 0x" + String(sensor.device_addr, HEX));
  logInfo("Packet size: " + String(sensor.packet_len) + " bytes");
  logInfo("Baud rate: " + String(sensor.baud_rate) + " bits/s");
  logInfo("Saved templates: " + String(sensor.templateCount));
}

uint8_t reads[256];
byte[256] getFingerData(Adafruit_Fingerprint &sensor) {
  memset(reads, 0xff, 256); 

  logInfo("Attempting to get template");
  finger.getModel();
  logIngo("Received " + String(sensor.swSerial->readBytes(reads, 256)) + " bytes");
  return reads;
}

byte[256] getFingerprint(Adafruit_Fingerprint &sensor, bool same) {
  printMessage("Veuillez poser " + String(same ? "le même" : "un") + " doigt sur la surface vitré clignotante.");

  uint8_t i, p = 0;
  logInfo("Waiting for valid finger to enroll");
  Serial.print("[I] ");
  for (i=0; i<FINGER_TIME_OUT; i++) {
    p = sensor.getImage();
    Serial.print(p == FINGERPRINT_OK ? "Image taken\n" : p == FINGERPRINT_NOFINGER ? "." : p == FINGERPRINT_PACKETRECIEVEERR ? "Communication error " : p == FINGERPRINT_IMAGEFAIL ? "Imaging error" : "Unknown error");
    if (p == FINGERPRINT_OK) {
      printMessage("Empreinte digital trouvée! \nRetirez votre doigt ...");
      delay(1000);
      do { p = sensor.getImage(); }
      while (p != FINGERPRINT_NOFINGER);
      // OK success!
      break;
    }
  }
  
  // Test if time out
  if (i == FINGER_TIME_OUT) {
    if (p == FINGERPRINT_NOFINGER) {
      logError("FINGER_GET_TIME_OUT: no finger");
      printMessage("Aucune empreinte digitale trouvée ! \nVeuillez réessayer ...");
    } else {
      logError("FINGER_GET_TIME_OUT: to many error");
      printMessage("Une erreur fatal s'est produite ! \nVeuillez réessayer ou contacter un administrateur.");
    }
  
  } else {
    // Convert image
    printMessage("Analyse ...");
    logInfo("Converting image ...");
    for (i=0; i<FINGER_TIME_OUT; i++) {
      p = sensor.image2Tz(same ? 2 : 1);
      if (p == FINGERPRINT_OK) {
        logInfo("Image converted");
        printMessage("Analyse terminée !");
        // Success
        delay(1000);
        return getFingerData(sensor); // TODO: return fingerprint hash
      } else if (p == FINGERPRINT_FEATUREFAIL || p == FINGERPRINT_INVALIDIMAGE || p == FINGERPRINT_IMAGEMESS) {
        logError("Could not find fingerprint features");
        printMessage("L'empreinte digitale n'est pas lisible ! \nNettoyez votre doigt ou choisissez-en un autre puis réessayez.");
        break;
      } else logError(p == FINGERPRINT_PACKETRECIEVEERR ? "Communication error" : "Unknown error");
    }

    if (i == FINGER_TIME_OUT) {
      logError("FINGER_GET_TIME_OUT: to many error");
      printMessage("Une erreur fatal s'est produite ! \nVeuillez réessayer ou contacter un administrateur.");
    }
  }

  // None valid
  delay(5000);
  return NULL;
}

byte[256] enrollFingerprint(Adafruit_Fingerprint &sensor) {
  byte[256] fingerprint = getFingerprint(sensor, false);
  
  if (fingerprint != NO_FINGER) {
    fingerprint = getFingerprint(sensor, true);
    if (fingerprint != NO_FINGER) {
      uint8_t p, i;
      for (i=0; i<FINGER_TIME_OUT; i++) {
        p = finger.createModel();
        if (p == FINGERPRINT_OK) {
          logInfo("Prints matched!");
          printMessage("Utilisateur créé ! Vous pouvez retirer votre doigt.");
          delay(2000);
          return fingerprint;
        } else if (p == FINGERPRINT_ENROLLMISMATCH) {
          logError("Fingerprints did not match");
          printMessage("Les empreintes ne correspondent pas ! \nVeuillez réessayer ...");
          break;
        } else logError(p == FINGERPRINT_PACKETRECIEVEERR ? "Communication error" : "Unknown error");
      }
      
      if (i == FINGER_TIME_OUT) {
        logError("FINGER_GET_TIME_OUT: to many error");
        printMessage("Une erreur fatal s'est produite ! \nVeuillez réessayer ou contacter un administrateur.");
      }    
    }
  }
  
  // None valid
  delay(5000);
  return NULL;
}

String userConnection(Adafruit_Fingerprint &sensor) {
  return "";
}

String userRegister() {
  
}
