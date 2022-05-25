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

uint32_t getFingerprint(Adafruit_Fingerprint &sensor, bool same) {
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
        SlogInfo("Image converted");
        printMessage("Analyse terminée !");
        // Success
        delay(2000);
        return ; // TODO: return fingerprint hash
      } else if (p == FINGERPRINT_FEATUREFAIL || p == FINGERPRINT_INVALIDIMAGE || p == FINGERPRINT_IMAGEMESS) {
        logError("Could not find fingerprint features");
        printMessage("L'empreinte digitale n'est pas lisible ! \nNettoyez votre doigt ou choisissez-en un autre puis réessayez.");
        break;
      } else logError(p == FINGERPRINT_PACKETRECIEVEERR ? "Communication error" : "Unknown error");
    }
  }

  // None valid
  delay(5000);
  return 0;


  
  // OK success!
  p = sensor.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  if (same) {
      // OK converted!
    Serial.print("Creating model");
  
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Serial.println("Prints matched!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      Serial.println("Fingerprints did not match");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }
  }
  
  return 0;
}

uint32_t enrollFingerprint(Adafruit_Fingerprint &sensor) {
  uint32_t fingerprint = getFingerprint(sensor, false);
  if (fingerprint != NO_FINGER) {

    
  }
  return 0;
}

String userConnection(Adafruit_Fingerprint &sensor) {
  return "";
}

String userRegister() {
  
}
