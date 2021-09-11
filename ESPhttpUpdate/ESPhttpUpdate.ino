/*
  ESPhttpUpdate.ino
  by Luiz H. Cassettari
*/

#include <ESP8266WiFi.h>

const char* ssid          = "";
const char* password      = "";

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();
  wifi_setup();
}

void wifi_setup() {
  Serial.println("Connecting to: " + String(ssid));
  WiFi.mode(WIFI_STA); // Setup ESP in client mode

  if (ssid == "")
    WiFi.begin();
  else
    WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.println("Local IP address: " + WiFi.localIP().toString());
}

void loop() {
  if (runEvery(5000)) {
    UpdateVersion();
  }
}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
