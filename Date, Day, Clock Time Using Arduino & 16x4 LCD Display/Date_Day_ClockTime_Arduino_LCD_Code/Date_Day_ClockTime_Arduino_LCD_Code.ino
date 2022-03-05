#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "RTClib.h"

#define SRx 0
#define STx 1

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
RTC_DS1307 rtc;
SoftwareSerial mySerial(SRx, STx);  //(Rx, Tx)

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// Global Constants
int Hrs = 0;
int Min = 0;
int Sec = 0;
int Year = 0;
int Mon = 0;
int Day = 0;
int Time = 0;
int cnt = 0;
int disp_cnt = 0;
int hh1cnt = 0;
int hh2cnt = 0;
int mm1cnt = 0;
int mm2cnt = 0;
int Update_Mode = 0;   //To change digits
int bt_cnt_up = 0;    // digit Up Down Counter
int bt_disp_up = 0;   // Display Selection Button
int bt_fin_time = 0;  // Finalize Time
int debug = 1;        // To debug using Serial Monitor
int debug2 = 1;        // To debug using Serial Monitor


void setup() {
  lcd.begin(16, 4);
  lcd.print("Real TIme clock using DS1307");
  delay(100);
  lcd.clear();

  //Serial Comm
  pinMode(SRx, INPUT);   //Serial Rx
  pinMode(STx, OUTPUT);  //Serial Tx

  mySerial.begin(9600);
  if (debug)
  {
    mySerial.println("\n.............LCD Display...........");  
  }
  if (! rtc.begin()){
    mySerial.println("Couldn't find RTC");
    mySerial.flush();
    while (1)
      delay(10);  
  }
}

void loop() {
  
  DateTime now = rtc.now();
  if (now.hour() > 24 or now.minute() > 60 or now.second() >60)   //RTC not initialized 
  {
    if (debug2)
    {
      mySerial.println("Set Clock Time");
      debug2 = 0;
    }
    Hrs = 0;
    Min = 0;
    Sec = 0; 
  }
  else
  {
    Hrs = now.hour();
    Min = now.minute();
    Sec = now.second();
    Year = now.year();
    Mon = now.month();
    Day = now.day();
  }     
  lcd.setCursor(0, 0);
  lcd.print("Date Day Time");
  lcd.setCursor(0, 1);
  lcd.print(Day);
  lcd.print("-" );
  if (Mon < 10)
  {
    lcd.print('0');
  }
  lcd.print(Mon);
  lcd.print("-" );
  lcd.print(Year);
  lcd.setCursor(0, 2);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.setCursor(0, 3);
  if (Hrs < 10)
  {
    lcd.print('0');
  }
  lcd.print(Hrs);
  lcd.print(":" );
  if (Min < 10)
  {
    lcd.print('0');
  }
  lcd.print(Min);
  lcd.print(":" );
    if (Sec < 10)
  {
    lcd.print('0');
  }
  lcd.print(Sec);


  
}
