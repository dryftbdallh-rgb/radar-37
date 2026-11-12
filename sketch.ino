#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int btnScanPin = 4;  
const int buzzerPin = 18;  

void setup() {
  Serial.begin(115200);
  
  // تشغيل الشاشة على أطراف 8 و 9
  Wire.begin(8, 9);
  
  pinMode(btnScanPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // تشغيل الشاشة
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // شاشة الترحيب
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("RADAR 37");
  display.display();
  delay(2000); // استنى ثانيتين
  
  // تفعيل الواي فاي بعد شاشة الترحيب عشان ميعطلش الشاشة
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  // مسح أولي
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Press button");
  display.println("to Scan...");
  display.display();
}

void loop() {
  // لو الزر اتضغط
  if (digitalRead(btnScanPin) == LOW) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Scanning...");
    display.display();
    
    // مسح الشبكات
    int n = WiFi.scanNetworks();
    
    display.clearDisplay();
    display.setCursor(0, 0);
    
    if (n == 0) {
      display.println("No networks found");
    } else {
      display.print(n);
      display.println(" Networks found:");
      
      // عرض أول 4 شبكات بس عشان الشاشة تكفي
      for (int i = 0; i < n && i < 4; ++i) {
        display.print(i + 1);
        display.print(": ");
        display.print(WiFi.SSID(i));
        display.print(" (");
        display.print(WiFi.RSSI(i));
        display.println(")");
      }
      // صوت تنبيه لما يلاقي شبكات
      tone(buzzerPin, 1000, 200);
    }
    display.display();
    delay(3000); // استنى 3 ثواني عشان تقرأ
  }
}
