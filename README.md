# ESP8266httpUpdateTest

This is a simple project to test the ESP8266httpUpdate library with Github as a server.

### Json Updater

The ESP8266 check the file `update.json` in this repository using the link below.

```arduino
const char * urlVersion = "https://raw.githubusercontent.com/ricaun/ESP8266httpUpdateTest/main/update.json";
```

The resquest return something like this:

```json
{
    "version": 1.01,
    "update": "https://raw.githubusercontent.com/ricaun/ESP8266httpUpdateTest/main/update_1.01.bin"
}
```

The version value is compare with the define on the `version.ino`.

```arduino
#define VERSION 1.01
```

If the value is below the `json` file, the ESP8266 try to update the firmware using the link on the `update` variable.


### HTTPS

To make the `https` links to work with the ESP8266 the `client` was set like below.

```arduino
WiFiClientSecure client;
client.setInsecure();
```

---

Do you like this library? Please [star this project on GitHub](https://github.com/ricaun/ESP8266httpUpdateTest/stargazers)!