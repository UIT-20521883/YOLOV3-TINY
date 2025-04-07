#ifndef FBASE_H
#define FBASE_H

#include <Arduino.h>
#include <string.h>
#include "esp_camera.h"
#include "soc/soc.h"           
#include "soc/rtc_cntl_reg.h"  
#include "driver/rtc_io.h"
#include <FS.h>
#include <LittleFS.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>

//============== DEFINE FIREBASE ====================
#define API_KEY "AIzaSyBGfYcpY5lJeyFgBupV7sNWMZuwglagyNE"
#define USER_EMAIL "phamngocthanhthao6901@gmail.com"
#define USER_PASSWORD "060901"
#define STORAGE_BUCKET_ID "esp-32-cam-demo.appspot.com"
#define BUCKET_PHOTO "data"
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void fcsUploadCallback(FirebaseData , FCS_UploadStatusInfo );
void capturePhotoSaveLittleFS(String );
void initLittleFS();
void initCamera();
void fcsUploadCallback(FCS_UploadStatusInfo );


#endif 