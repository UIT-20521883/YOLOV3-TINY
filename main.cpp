#include "oled.hpp"
#include "fbase.hpp"
#include <Wire.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

// ============= DEVICE ID DECLARATION =================
const int DEVICE_ID = 14;
String PhotoName;

//================= WIFI DECLARATION ====================
const char* ssid = ".";
const char* password = "123456789"; 
 
//================ SERVER DECLARATION (FOR HTTP) ====================
String serverName = "http://192.168.131.74:5000";

String post_deviceID = serverName + "/confirm";
String get_classID = serverName + "/confirm1";
String post_namePhoto = serverName + "/confirm2"; 
String get_virtual = serverName + "/confirm3";
String get_result = serverName + "/confirm4";

//================ I2C DECLARATION ==================================
TwoWire I2Cbus = TwoWire(0);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);

//=============== JSON DECLARATION ==================================
String SEND_ID, SEND_PHOTONAME, RCV_VIR, RCV_CLASSID, RCV_RESULT;
StaticJsonDocument<200> send_id_json, send_photoname_json, rcv_classid_json, rcv_vir_json, rcv_result_json;
unsigned long lastTime = 0;
unsigned long WiFiDelay = 20000;

//=============== FIREBASE DECLARATION =============================
boolean takeNewPhoto = true;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;
String PhotoURL; 
bool taskCompleted = false; 

//=============== SENSOR DECLARATION ===========================-=
int cambien = 12; 

void check_cb(){
  delay(300);
  while(digitalRead(cambien) == 1){
    Serial.println("CHUA CO AI DIEM DANH");
    NAOLED(oled);
    delay(300);
  }
  AvailOLED(oled);
}

void POST1(){
  // >>>>> POST ID_DEVICE TO THE SERVER
  send_id_json["id_device"] = DEVICE_ID;
  serializeJson (send_id_json, SEND_ID);

  //for debug
  Serial.println(SEND_ID);

  if (WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client, post_deviceID.c_str());
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(SEND_ID);
    if (httpResponseCode > 0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end(); 
  } else {
    Serial.println("WiFi Disconnected");
    noWiFiOLED(oled);
    while(true){};
  }
  startOLED(oled);
  delay(3000);
}

void GET1(){
  // >>>>>> GET ID CLASS FROM THE SERVER
  if (WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client, get_classID.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      RCV_CLASSID = http.getString();
      Serial.println(RCV_CLASSID);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    noWiFiOLED(oled);
    while(true){};
  }

  //for debug
  Serial.println(RCV_CLASSID);
  
  DeserializationError error_get_classID = deserializeJson(rcv_classid_json, RCV_CLASSID);
  RCV_CLASSID = "";
  if (error_get_classID) {
    Serial.print("deserializeJson() for classID failed: "); 
    errorJson1OLED(oled);
    delay(3000);
    goto BatDau;
    Serial.println(error_get_classID.c_str());
  }

  const char* ID_Class1 = rcv_classid_json["MaLMH"];
  String ID_Class = String(ID_Class1);
  Serial.println(ID_Class);
  lopOLED(oled, ID_Class);

}

void POST2(){
    //>>>>>>>>>> TAKE PHOTO FROM ESP32 SEND TO FIREBASE
  
  //startOLED();
  if (takeNewPhoto) {
    // String numberStr = String(DEVICE_ID);
    // String nameFile = "/" + numberStr + ".jpg";
    // PhotoName = numberStr;
    
    // String p1 = "/" + ID_Class;
    // String p2 = "_" + String(DEVICE_ID);
    // String nameFile = p1 + p2 + ".jpg";

    String nameFile = "/" + ID_Class + "_" + String(DEVICE_ID) + ".jpg";
    PhotoName = ID_Class + "_" + String(DEVICE_ID);
        
    chupAnhOLED(oled);
    capturePhotoSaveLittleFS(nameFile);
    waitOLED(oled);
    //count++;
    takeNewPhoto = false;
    if (Firebase.ready() && !taskCompleted) {
      Serial.print("Uploading picture... ");
      if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, nameFile, mem_storage_type_flash, BUCKET_PHOTO + nameFile, "image/jpeg", fcsUploadCallback)) {
        Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
      } else {
        Serial.println(fbdo.errorReason());
      }
    }
  }

  takeNewPhoto = true;
  send_photoname_json["photo_name"] = PhotoName;
  PhotoName = "";
  serializeJson (send_photoname_json, SEND_PHOTONAME);

  //for debug
  Serial.println(SEND_PHOTONAME);

  if (WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client, post_namePhoto.c_str());
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(SEND_PHOTONAME);
    if (httpResponseCode > 0){ 
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    noWiFiOLED(oled);
    while(true){};
  } 

  delay(8000);

}

void GET2(){
  // >>>>>>>>>>>> GET VIRTUAL ROUTE >>>>>>>>>>>>>>>>>
  bool isResult;
  do {
    if (WiFi.status() == WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, get_virtual.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        RCV_VIR = http.getString();
        Serial.println(RCV_VIR);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
      noWiFiOLED(oled);
      while(true){};
    }

    DeserializationError error_get_vir = deserializeJson(rcv_vir_json, RCV_VIR);
    RCV_VIR = "";
    if (error_get_vir) {
      Serial.print("deserializeJson() failed: "); 
      errorJson1OLED(oled);
      delay(3000);
      Serial.println(error_get_vir.c_str());
    }
    isResult = rcv_vir_json["flag"];
    //rcv_vir_json = "";
    Serial.print("isResult: ");
    Serial.println(isResult);
    delay(3000);
  } while (!isResult);

  isResult = false;
}

void GET3(){
  // >>>>>>>>>>>>>>> GET RESULT >>>>>>>>>>>>>>>>>>>>>
  if (WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client, get_result.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      RCV_RESULT = http.getString();
      Serial.println(RCV_RESULT);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    noWiFiOLED(oled);
    while(true){};
  }

  Serial.println(RCV_RESULT);
  DeserializationError error_get_result = deserializeJson(rcv_result_json, RCV_RESULT);
  RCV_RESULT = "";
  if (error_get_result) {
    Serial.print("deserializeJson() for classID failed: "); 
    errorJson1OLED(oled);
    goto BatDau;
    delay(3000);
    Serial.println(error_get_result.c_str());
  }

  bool attendance = rcv_result_json["attendance"];
  const char* ID_SV = rcv_result_json["mssv"];
  int classDelay = rcv_result_json["delay"];

  if (attendance){
    passOLED(oled);
    MSSVOLED(oled, String(ID_SV));
    delay(1000*classDelay);
  } else if (!attendance) {
    failOLED(oled);
    delay(10000);
  } else {
    Serial.println("Loi json 2");
    errorJson2OLED(oled);
  }
}


// >>>>>>>>>>>> MAIN PROGRAM >>>>>>>>>>>>>>>>>
void setup(){
  Serial.begin(115200);
  I2Cbus.begin(I2C_SDA, I2C_SCL, 100000);
  Serial.println("Initialize display");
  pinMode(cambien, INPUT);

  // ==== CHECK OLED ====
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.printf("SSD1306 OLED display failed to initalize.\nCheck that display [SDA, SCL] is connected to pin [%d and %d]n", I2C_SDA, I2C_SCL);
    while (true);
  }

  helloOLED(oled);

  // ====== CHECK WIFI CONNECTION ======
  lastTime = millis();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    connectingWiFiOLED(oled);
    if (millis() - lastTime > WiFiDelay){
      noWiFiOLED(oled);
      while(true){};
    }
  }

  Serial.print("\nConnected WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  initLittleFS();
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  initCamera();

  configF.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  configF.token_status_callback = tokenStatusCallback;

  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  BatDau:
  check_cb();
  POST1();
  GET1();
  POST2();
  GET2();
  GET3();
}