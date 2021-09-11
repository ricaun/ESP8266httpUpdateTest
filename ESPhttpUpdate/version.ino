/*
  version.ino
  created 31 07 2021
  by Luiz H. Cassettari
*/

#if ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiClient.h>
#endif
#if ESP32
#include <HTTPClient.h>
#endif

#include <ArduinoJson.h>

#define VERSION 1.01

const char * urlPool = "https://raw.githubusercontent.com/ricaun/ESP8266httpUpdateTest/main/update.json";

void DecodeJsonVersion(String input)
{
  DynamicJsonDocument doc(256);
  deserializeJson(doc, input);

  const char* update = doc["update"];
  double version = doc["version"];

  Serial.println("Version: " + String(VERSION));
  if (version > VERSION)
  {
    Serial.println("Update to Version: " + String(version));
    Serial.println("Update: " + String(update));
    ESPhttpUpdateUpdate(update);
  }
}

void UpdateVersion()
{
  String input = httpGetString(urlPool);
  if (input == "") return;
  DecodeJsonVersion(input);
}

String httpGetString(String URL)
{
  String payload = "";
  
  WiFiClientSecure client;
  client.setInsecure();
  
  HTTPClient http;
  if (http.begin(client, URL))
  {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
      payload = http.getString();
    }
    else
    {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  return payload;
}

// -------------------------------------------------------------------- //


void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}


void ESPhttpUpdateUpdate(const char* http)
{
  WiFiClientSecure client;
  client.setInsecure();

  // The line below is optional. It can be used to blink the LED on the board during flashing
  // The LED will be on during download of one buffer of data from the network. The LED will
  // be off during writing that buffer to flash
  // On a good connection the LED should flash regularly. On a bad connection the LED will be
  // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
  // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

  // Add optional callback notifiers
  ESPhttpUpdate.onStart(update_started);
  ESPhttpUpdate.onEnd(update_finished);
  ESPhttpUpdate.onProgress(update_progress);
  ESPhttpUpdate.onError(update_error);

  t_httpUpdate_return ret = ESPhttpUpdate.update(client, http);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}
