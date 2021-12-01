#include "VM.h"
#include <WiFi.h>
#include <WebOTA.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <pthread.h>

WebServer server(80);

#undef min
#undef max

#ifdef ARDUINO_SAM_DUE
extern "C" int _gettimeofday() {
    return 0;
}
extern "C" void exit(int) {
  while(1);
}
extern "C" void _exit(int) {
  while(1);
}
extern "C" void _kill(int) {}
extern "C" void _getpid(int) {}
#endif

//uncomment these lines if target is STM32
//extern "C" int _gettimeofday() {
//    return 0;
//}

VM vm;
TaskHandle_t Task1;

void runCode() {
  const size_t CAPACITY = JSON_ARRAY_SIZE(100);
  StaticJsonDocument<CAPACITY> doc;
  File f = SPIFFS.open("/main");
  deserializeJson(doc, f.readString());
  f.close();
  JsonArray array = doc.as<JsonArray>();
  std::vector<Value> values;
  bool nullFound = false;
  //[true, false, 'hello', null, 10, null, null] -> True, False, hello, 10, null
  for(JsonVariant v : array) {
    if (v.is<String>()) {
      if (nullFound) {
        values.push_back(Value(v.as<char*>()).toNum());
      } else {
        values.push_back(Value(v.as<char*>()));
      }
    } else if (v.is<bool>()) {
      values.push_back(v.as<bool>());
    } else if (v.as<String>() == "null") {
      if (nullFound) {
        nullFound = false;
        values.push_back(null);
      }
      nullFound = true;
    } else if (v.is<double>()) {
      nullFound = false;
      values.push_back(v.as<double>());
    }
  }
  vm.run(values);
}

void handleUpload() {
  File f = SPIFFS.open("/main", "w");
  f.print(server.arg(0));
  f.close();
  server.send(200);
  ESP.restart();
}

void updateFirmware() {
  File f = SPIFFS.open("/updateFirmware", "w");
  f.print("");
  f.close();
  server.send(200);
  ESP.restart();
}

void commandLine() {
  if (Serial.peek() == '\r') {
    Serial.read();
    String data = "";
    while (Serial.available() == 0);
    for (byte i = 0; i < 3; i++) {
      while (Serial.available() > 0) {
        data += (char) Serial.read();
        long long t = millis() + 50;
        while (t > millis() && Serial.available() == 0); 
      }
      long long t = millis() + 50;
      while (t > millis() && Serial.available() == 0); 
    }
    if (data.startsWith("WiFi connect")) {
      data.replace("WiFi connect", "");
      String ssid = data.substring(0, data.indexOf("\t"));
      ssid.trim();
      String passwd = data.substring(data.indexOf("\t") + 1, data.length() - 1);
      passwd.trim();
      File ssidFile = SPIFFS.open("/ssid", "w");
      ssidFile.print(ssid);
      ssidFile.close();
      File passwdFile = SPIFFS.open("/passwd", "w");
      passwdFile.print(passwd);
      passwdFile.close();
    } else if (data.startsWith("WiFi create")) {
      data.replace("WiFi create", "");
      String ssid = data.substring(0, data.indexOf("\t"));
      ssid.trim();
      String passwd = data.substring(data.indexOf("\t") + 1, data.length() - 1);
      passwd.trim();
      File ssidFile = SPIFFS.open("/apssid", "w");
      ssidFile.print(ssid);
      ssidFile.close();
      File passwdFile = SPIFFS.open("/appasswd", "w");
      passwdFile.print(passwd);
      passwdFile.close();
    } else if (data.startsWith("WiFi disconnect")) {
      SPIFFS.remove("/passwd");
      SPIFFS.remove("/ssid");
    } else if (data.startsWith("WiFi stopAP")) {
      SPIFFS.remove("/appasswd");
      SPIFFS.remove("/apssid");
    } else if (data.startsWith("Upload")) {
      File f = SPIFFS.open("/main", "w");
      f.print(data.substring(6));
      f.close();
      ESP.restart();
    }
  }
}

void setup() {
  Serial.begin(9600);
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  int start = millis();
  while (millis() - start < 120) {
    commandLine();
  }
  bool wifiEnabled = false;
  if (SPIFFS.exists("/ssid") && SPIFFS.exists("/passwd") && SPIFFS.exists("/apssid") && SPIFFS.exists("/appasswd")) {
    WiFi.mode(WIFI_MODE_APSTA);
  } else if (SPIFFS.exists("/apssid") && SPIFFS.exists("/appasswd")) {
    WiFi.mode(WIFI_AP);
  } else if (SPIFFS.exists("/ssid") && SPIFFS.exists("/passwd")) {
    WiFi.mode(WIFI_STA);
  } else {
    WiFi.mode(WIFI_OFF);
  }
  if (SPIFFS.exists("/ssid") && SPIFFS.exists("/passwd")) {
    File ssid = SPIFFS.open("/ssid");
    File passwd = SPIFFS.open("/passwd");
    String ssid_ = ssid.readString();
    String passwd_ = passwd.readString();
    WiFi.begin(ssid_.c_str(), passwd_.c_str());
    ssid.close();
    passwd.close();
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      int start = millis();
      while (millis() - start < 500) {
        commandLine();
      }
      Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    wifiEnabled = true;
  }
  if (SPIFFS.exists("/apssid") && SPIFFS.exists("/appasswd")) {
    File ssid = SPIFFS.open("/apssid");
    File passwd = SPIFFS.open("/appasswd");
    String ssid_ = ssid.readString();
    String passwd_ = passwd.readString();
    WiFi.softAP(ssid_.c_str(), passwd_.c_str());
    ssid.close();
    passwd.close();
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    wifiEnabled = true;
  }
  server.on("/upload", handleUpload);
  server.on("/updateFirmware", updateFirmware);
  if (SPIFFS.exists("/updateFirmware")) {
    SPIFFS.remove("/updateFirmware");
    while (true) {
      webota.handle(); // to make firmware updating easier
    }
  }
  std::vector<Value>* mem = new std::vector<Value>;
  vm.attachMem(mem);
  if (wifiEnabled) {
    server.begin();
    xTaskCreatePinnedToCore(
        _loop,   /* Task function. */
        "",     /* name of task. */
        10000,       /* Stack size of task */
        NULL,        /* parameter of the task */
        0,           /* priority of the task */
        &Task1,      /* Task handle to keep track of created task */
        0);
  }
  if (SPIFFS.exists("/main")) runCode();
}

void _loop(void *) {
  while (true) {
    // webota.handle(); // to make firmware updating easier
    server.handleClient();
  }
}

void loop() {}
