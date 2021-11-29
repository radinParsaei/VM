#include "VM.h"
#include <WiFi.h>
#include <WebOTA.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

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
  runCode();
  server.send(200);
}

void setup() {
  Serial.begin(9600);
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin("<SSID>", "<PASSWD>"); // TODO: make it configurable through Serial or something
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  server.on("/upload", handleUpload);
  server.begin();
  std::vector<Value>* mem = new std::vector<Value>;
  vm.attachMem(mem);
  runCode();
}

void loop() {
  webota.handle(); // to make firmware updating easier
  server.handleClient();
}
