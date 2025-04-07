

# SmartDesk_ESP32CAM_Final
* Mã nguồn đồ án bàn học thông minh (ESP32-CAM firmware): [Click đây](https://github.com/tuandat2206/SmartDesk_ESP32CAM_Final/tree/main)

* Mã nguồn gồm: 

1. Các file oled.hpp, oled.cpp: mã nguồn gồm những hàm hiển thị nội dung lên oled như:
  + "HELLO"
  + "KHONG AI DIEM DANH"
  + "CO NGUOI DIEM DANH"
  + "XIN MOI DIEM DANH"
  + "CHUP ANH TRONG (5->0)s"
  + "VUI LONG CHO DOI"
  + "DIEM DANH THAT BAI"
  + "DIEM DANH THANH CONG"
  + "NO WIFI"
  + "DANG KET NOI WIFI"
  + "LOI JSON 1"
  + "LOI JSON 2"

2. Các file fbase.h, fbase.cpp: mã nguồn gồm những hàm liên quan đến Firebase

3. main.cpp: chứa chương trình chính được thực thi trên ESP32-CAM

* Để code chạy trên PlatformIO hoặc Arduino thì cần cài đặt thêm những thư viện dưới đây:
    + mobizt/Firebase Arduino Client Library for ESP8266 and ESP32@^4.4.10
	+ adafruit/Adafruit SSD1306@^2.5.9
	+ bblanchon/ArduinoJson@^7.0.0
	+ rupakpoddar/ESP32 Firebase@^1.0.0
	+ adafruit/Adafruit GFX Library@^1.11.9
	+ adafruit/Adafruit BusIO@^1.14.5
	+ mobizt/Firebase ESP32 Client@^4.4.10

**Lưu ý**: Nếu nạp code thông qua Arduino thì cần đổi đuôi file thành *.ino
* Liên hệ về source code: Trần Tuấn Đạt - 20520438 - 20520438@gm.uit.edu.vn
