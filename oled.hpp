#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <String.h>

//================= DEFINE WITH OLED ==============

#define I2C_SDA 15
#define I2C_SCL 14

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C

#define TEXT_SIZE 2
#define TEXT_COLOR WHITE

#define XHello 128/3
#define YHello 30
#define XNoWF 128/8
#define YNoWF 30
#define XStart 128/8
#define YStart 128/10
#define XWait 4
#define YWait 64/3
#define XPass 128/10
#define YPass 64/4
#define XFail 128/10
#define YFail 64/4

void resetOLED(Adafruit_SSD1306 );
void helloOLED(Adafruit_SSD1306 );
void errorJson1OLED(Adafruit_SSD1306 );
void errorJson2OLED(Adafruit_SSD1306 );
void startOLED(Adafruit_SSD1306 );
void passOLED(Adafruit_SSD1306 );
void failOLED(Adafruit_SSD1306 );
void waitOLED(Adafruit_SSD1306 );
void noWiFiOLED(Adafruit_SSD1306 );
void connectingWiFiOLED(Adafruit_SSD1306 );
void cxdOLED(Adafruit_SSD1306 );
void AvailOLED(Adafruit_SSD1306 );
void NAOLED(Adafruit_SSD1306 );
void chupAnhOLED(Adafruit_SSD1306 );
void MSSVOLED(Adafruit_SSD1306 , String );
void lopOLED(Adafruit_SSD1306 , String );

#endif