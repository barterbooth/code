#include <Diablo_Serial_4DLib.h>

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

const char *Errors[] = {"OK\0", "Timeout\0", "NAK\0", "Length\0", "Invalid\0"} ;
void errorCallback(int ErrCode, unsigned char Errorbyte) {
  if (screenInitialised) {
    logError("", true);
    logError("Display Serial reports error " + String(Errors[ErrCode]) + (ErrCode == Err4D_NAK ? ", returned data:  " + String(Errorbyte) : ""), true);
  
  } else if (ErrCode != Err4D_OK) {
    logInfo(" ");
    logError("Display Serial reports error " + String(Errors[ErrCode]) + (ErrCode == Err4D_NAK ? ", returned data:  " + String(Errorbyte) : ""));
    logError("Failed to initialise screen! Device not connected.", true);
    failedSignal();
  }
}

void printDisplayInfos(Diablo_Serial_4DLib &display) {
  char model[20];
  display.sys_GetModel(model); 
  logInfo("Display model: " + String(model));
  logInfo("SPE2 Version: " + String(display.sys_GetVersion(), HEX));
  logInfo("PmmC Version: " + String(display.sys_GetPmmC(), HEX));
  
}

void printMessage(String message) {
  logInfo("", true);
  logInfo(message, true);
}
