#ifndef __FINGERPRINT_H__
#define __FINGERPRINT_H__

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

//Add new error
#define FINGERPRINT_TOMANYERROR 0xFD
#define FINGERPRINT_TESTTIMEOUT 50


class Fingerprint {
  public:
    Fingerprint(uint8_t rx, uint8_t tx, uint32_t baudRate, uint32_t password = 0x0000000);

    bool initSensor();
    void printInfos();
    uint8_t getFingerprint();
    void waitNoFinger();
    uint8_t fingerAnalysis(uint8_t slot);
    uint8_t createModel();
    uint8_t enrollFingerprint();
    byte[256] getModelData();
    void enableLogging(bool enable);
    void setLoggingFormat(const String info, const String error);

  private:
    const SoftwareSerial _serial;
    const Adafruit_Fingerprint _sensor;

    String _finfo,
                 _ferror;
    bool _logging;
    uint8_t _i, 
            _p,
            _error;
    uint32_t _baudRate;
    byte[256] _reads;

    void logInfo(const String text);
    void logInfo(const String text, bool separator);
    void logError(const String text);
    void logError(const String text, bool separator);
};

#endif //__FINGERPRINT_H__
