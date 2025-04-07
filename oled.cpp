#include "oled.hpp"

// =============== OLED FUNCTION ==================
void resetOLED(Adafruit_SSD1306 display)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);  
}

void helloOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print HELLO on OLED");
  resetOLED(display);
  display.setCursor(XHello, YHello);
  display.print("HELLO");
  display.display();
  delay(3000);
}

void errorJson1OLED(Adafruit_SSD1306 display)
{
  Serial.println("Print LOI JSON on OLED");
  resetOLED(display);
  display.setCursor(XStart, YStart);
  display.print("LOI\nJSON 1");
  display.display();
  delay(3000);
}

void errorJson2OLED(Adafruit_SSD1306 display)
{
  Serial.println("Print LOI JSON on OLED");
  resetOLED(display);
  display.setCursor(XStart, YStart);
  display.print("LOI\nJSON 2");
  display.display();  
  delay(3000);
}

void startOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print 'XIN MOI DIEM DANH' on OLED");
  resetOLED(display);
  display.setCursor(XStart, YStart);
  display.print(" XIN MOI\n DIEM DANH");
  display.display();
  delay(3000);
}

void passOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print 'DIEM DANH THANH CONG' on OLED");
  resetOLED(display);
  display.setCursor(XPass, YPass);
  display.print("DIEM DANH\nTHANH CONG");
  display.display();
  delay(5000);
}

void failOLED(Adafruit_SSD1306 display)
{ 
  Serial.println("Print 'DIEM DANH THAT BAI' on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("DIEM DANH\n THAT BAI");
  display.display();
  delay(5000);
}

void waitOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print VUI LONG DOI on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("VUI LONG\n CHO DOI");
  display.display();
  delay(3000);
}

void noWiFiOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print NO WIFI on OLED");
  resetOLED(display);
  display.setCursor(XNoWF, YNoWF);
  display.print(" NO WIFI");
  display.display();
  delay(5000);
} 

void connectingWiFiOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print DANG KET NOI WIFI on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("DANG KET\n NOI WIFI");
  display.display();
  delay(500);
}

void cxdOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print DANG KET NOI WIFI on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("CHUA XAC\n DINH SV");
  display.display();
  delay(500);
}

void AvailOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print CO NGUOI DIEM DANH on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("CO NGUOI\n DIEM DANH");
  display.display();
  delay(500);
}

void NAOLED(Adafruit_SSD1306 display)
{
  Serial.println("Print KO CO NGUOI DIEM DANH on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("KHONG AI\n DIEM DANH");
  display.display();
  delay(500);
}

void chupAnhOLED(Adafruit_SSD1306 display)
{
  for (int i = 5;i >= 0; i--){
    resetOLED(display);
    Serial.write("Chup anh trong ");
    Serial.write(i);
    Serial.println("s");
    display.setCursor(XFail, YFail);
    display.print("CHUP ANH\n TRONG ");
    display.print(i);
    display.print("s");
    display.display();
    delay(1000);
  }
}

void MSSVOLED(Adafruit_SSD1306 display, String mssv)
{ 
  Serial.println("Print MSSV on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("XIN CHAO\n ");
  display.print(mssv);
  display.display();
}

void lopOLED(Adafruit_SSD1306 display, String lop)
{ 
  Serial.println("Print Lop on OLED");
  resetOLED(display);
  display.setCursor(XFail, YFail);
  display.print("LOP\n ");
  display.print(lop);
  display.display();
  delay(5000);
}
