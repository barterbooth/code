#include <Adafruit_Fingerprint.h>
#include <Diablo_Serial_4DLib.h>
#include <ESP8266WiFi.h>

bool initFingerSensor(Adafruit_Fingerprint &finger, uint32_t baudrate) {
  logInfo("", true);
  logInfo("Fingerprint sensor initialisation ...");
  
  finger.begin(baudrate);
  delay(10);
  if (finger.verifyPassword()) logInfo("Found fingerprint sensor!");
  else logError("Password don't match! Try to bypass password ...");

  logInfo(" ");
  logInfo("Reading sensor parameters ...");
  if (finger.getParameters() != FINGERPRINT_OK) {
    logError("Failed to read parameters, password bypass failed! Device not connected!", true);
    return false;
  }
  printFingerInfos(finger);

  logInfo(" ");
  logInfo("Initialization success.", true);
  delay(100);
  return true; // Initialization success
}

bool initDisplay(Diablo_Serial_4DLib &display, uint32_t baudrate) {
  // functions: https://github.com/4dsystems/Diablo16-Serial-Arduino-Library/blob/master/src/Diablo_Serial_4DLib.h
  logInfo("", true);
  logInfo("Display initialisation ...");
  
  display.setbaudWait(baudrate);
  logInfo(" ");
  logInfo("Set command time out to 5000 ms ...");
  display.TimeLimit4D = 5000; // Commands time out (ms)
  logInfo("Set callback function ...");
  display.Callback4D = errorCallback;
  logInfo("Cleaning and filling display ...");
  display.gfx_Cls();
  display.gfx_BGcolour(0);

  logInfo(" ");
  printDisplayInfos(display);
  
  logInfo(" ");
  logInfo("Initialization success.", true);
  screenInitialised = true;
  delay(100);
  return true; // Initialization success
}

bool initWebClient(WiFiClient &client, const String ssid, const String password, const String host) {
  logInfo("", true);
  logInfo("Web Client initialisation ...");

  logInfo(" ");
  logInfo("Connecting to '" + ssid + "' with password: " + password);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("[I] ");
  
  // Try to connect to wifi
  for (int i=0; i<CONNECT_MAX_REQUEST; i++) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    } else goto associed;
  }
  Serial.println();
  logError("Connection time out! No answer after " + String(CONNECT_MAX_REQUEST) + " attemps.");
  logInfo(" ");
  logError("Initialization failed!", true);
  return false;
  associed:
  Serial.println();
  logInfo("Succefull connected to Wi-Fi: " + ssid);
  logInfo("Local IP address: " + WiFi.localIP().toString());
  
  // Try to connect to host
  logInfo(" ");
  logInfo("Testing connection ...");
  logInfo("Connecting to " + host.substring(0, host.indexOf(":")) + ":" + (host.indexOf(":") == -1 ? "80" : host.substring(host.indexOf(":")+1)));
  Serial.print("[I] ");
  for (int i=0; i<TCP_DEFAULT_KEEPALIVE_COUNT; i++) {
    if (!client.connect(host.substring(0, host.indexOf(":")), host.indexOf(":") == -1 ? 80 : host.substring(host.indexOf(":")+1).toInt())) {
      Serial.print(".");
      delay(5000);
    } else goto connected;
  }
  Serial.println();
  logError("Connection at host failed! No answer after " + String(TCP_DEFAULT_KEEPALIVE_COUNT) + " requests.");
  logInfo(" ");
  logError("Initialization failed!", true);
  return false;
  connected:
  Serial.println();
  logInfo("Host connected! Sending data to server ...");
  client.println("hello from ESP8266");
  // wait for data to be available
  unsigned long timeout = millis();
  while (!client.available()) {
    if (millis() - timeout > 10000) {
      logError(">>> Client Timeout !");
      client.stop();
      logInfo("");
      logError("Initialization failed!", true);
      return false;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  logInfo("Receiving data from remote server ...");
  // not testing 'client.connected()' since we do not need to send data here
  Serial.print("[I] Received data: ");
  while (client.available()) Serial.print(static_cast<char>(client.read()));
  // Close the connection
  logInfo("Closing connection ...");
  client.stop();

  logInfo(" ");
  logInfo("Initialization success.", true);
  delay(100);
  return true; // Initialization success
}
