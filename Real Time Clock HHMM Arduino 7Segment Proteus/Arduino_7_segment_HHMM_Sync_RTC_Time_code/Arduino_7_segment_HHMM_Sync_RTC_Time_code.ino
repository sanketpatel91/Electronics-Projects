/**@file Arduino_7_segment.ino

   @mainpage 4Digit Real TIme Clock(HHMM) using 7 segment Display

   @section description Description
   Sketch demonstrating how to interface 4 digit 7 segment with Arduino Uno Board (Arduino Uno PinOut: https://diyi0t.com/wp-content/uploads/2019/08/Arduino-Uno-Pinout-1.png)
   Used Common Cathod type of 7 segment display in project

   @section circuit Circuit
   -  2Digit 7 segment displalys(Common Cathod) pins a to g are coneected to Arduino Digital pin 1 to 6.
   -  2 LEDs (Blinking at every second) in between hour and minute 2 Digit 7 segment Display.
   -  "Select Display" push button connected to Arduino pin A1.
   -  "Increment Count" push button connected to Arduino pin A2.
   -  "Finalize Time" push button connected to Arduino pin A3.
   -  DS1307 Real Time clock pins SDA & SCL are connected to Arduino pin A4 & A5.
   -  Arduino Digitalpins 12 & 13 used for Serial communication debugging.

   @section libraries Libraries
   - SoftwareSerial.h : Serial communication on other digital pins of the Arduino (https://www.arduino.cc/en/Reference/softwareSerial)
   - RTClib.h : Works with DS1307, DS3231, PCF8523, PCF8563 on multiple architectures (https://www.arduino.cc/reference/en/libraries/rtclib/)

   @section todo TODO
   - Check Proteus project for simulation

   @section author Author
   - Created by Sanket Patel on 18-Feb-2022.
   - Modified on 04-Mar-2022.
*/

// Libraries
#include <SoftwareSerial.h>
#include "RTClib.h"

// Defines
#define sa 0
#define sb 1
#define sc 2
#define sd 3
#define se 4
#define sf 5
#define sg 6

#define disp1 7
#define disp2 8
#define disp3 9
#define disp4 10

#define SRx 12
#define STx 13
#define ledPin 11 // the number of the LED pin

RTC_DS1307 rtc;
SoftwareSerial mySerial(SRx, STx);  //(Rx, Tx)

// Global Constants
int Hrs = 0;
int Min = 0;
int Time = 0;
int cnt = 0;
int disp_cnt = 0;
int hh1cnt = 0;
int hh2cnt = 0;
int mm1cnt = 0;
int mm2cnt = 0;
int Update_Mode = 0;  //To change digits
int bt_cnt_up = 0;    // digit Up Down Counter
int bt_disp_up = 0;   // Display Selection Button
int bt_fin_time = 0;  // Finalize Time

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;
const long interval = 300;           // interval at which to blink (milliseconds)

int debug = 1;        // To debug using Serial Monitor
int debug2 = 1;        // To debug using Serial Monitor

//#define F_CPU 8000000

//setup
/**
   The standard Arduino setup function used for setup and configuration tasks.
*/
void setup() {
  //Serial Comm
  pinMode(SRx, INPUT);   //Serial Rx
  pinMode(STx, OUTPUT);  //Serial Tx

  mySerial.begin(9600);
  if (debug)
  {
    mySerial.println("\n.............Real Time Clock HH MM...........");
  }
  if (! rtc.begin()) {
    mySerial.println("Couldn't find RTC");
    mySerial.flush();
    while (1)
      delay(10);
  }

  ////RTCLib I2C
  //  if (! rtc.isrunning()) {
  //    mySerial.println("RTC is NOT running!");
  ////    mySerial.println("Adjust RTC Time using Push Buttons");
  //    rtc.adjust(DateTime(2022, 02, 22, 0 , 0, 0)); // (YYYY,MM,DD,HH,MM,SS)
  //    delay(1000);
  //  }

  //Pin Configuration
  pinMode(sa, OUTPUT);
  pinMode(sb, OUTPUT);
  pinMode(sc, OUTPUT);
  pinMode(sd, OUTPUT);
  pinMode(se, OUTPUT);
  pinMode(sf, OUTPUT);
  pinMode(sg, OUTPUT);

  pinMode(ledPin, OUTPUT);   //LED

  pinMode(disp1, OUTPUT);
  pinMode(disp2, OUTPUT);
  pinMode(disp3, OUTPUT);
  pinMode(disp4, OUTPUT);

  digitalWrite(disp1, LOW);
  digitalWrite(disp2, LOW);
  digitalWrite(disp3, LOW);
  digitalWrite(disp4, LOW);

  pinMode(A0, OUTPUT);         //ResetButton
  pinMode(A1, INPUT_PULLUP);   //cnt up Button Pin
  pinMode(A2, INPUT_PULLUP);   //Display select Button Pin
  pinMode(A3, INPUT_PULLUP);   //Finalize Time Button

  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);

  digitalWrite(ledPin, LOW);

}

/**
   7 segment Pin configuration
   Logic for all 7 segment pins to display digit 0-9.
*/
void show_num(int cnt) {
  switch (cnt)
  {
    case 0:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, HIGH);
      digitalWrite(se, HIGH);
      digitalWrite(sf, HIGH);
      digitalWrite(sg, LOW);
      break;

    case 1:
      digitalWrite(sa, LOW);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, LOW);
      digitalWrite(se, LOW);
      digitalWrite(sf, LOW);
      digitalWrite(sg, LOW);
      break;

    case 2:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, LOW);
      digitalWrite(sd, HIGH);
      digitalWrite(se, HIGH);
      digitalWrite(sf, LOW);
      digitalWrite(sg, HIGH);
      break;

    case 3:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, HIGH);
      digitalWrite(se, LOW);
      digitalWrite(sf, LOW);
      digitalWrite(sg, HIGH);
      break;

    case 4:
      digitalWrite(sa, LOW);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, LOW);
      digitalWrite(se, LOW);
      digitalWrite(sf, HIGH);
      digitalWrite(sg, HIGH);
      break;

    case 5:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, LOW);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, HIGH);
      digitalWrite(se, LOW);
      digitalWrite(sf, HIGH);
      digitalWrite(sg, HIGH);
      break;

    case 6:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, LOW);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, HIGH);
      digitalWrite(se, HIGH);
      digitalWrite(sf, HIGH);
      digitalWrite(sg, HIGH);
      break;

    case 7:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, LOW);
      digitalWrite(se, LOW);
      digitalWrite(sf, LOW);
      digitalWrite(sg, LOW);
      break;

    case 8:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, HIGH);
      digitalWrite(se, HIGH);
      digitalWrite(sf, HIGH);
      digitalWrite(sg, HIGH);
      break;

    case 9:
      digitalWrite(sa, HIGH);
      digitalWrite(sb, HIGH);
      digitalWrite(sc, HIGH);
      digitalWrite(sd, HIGH);
      digitalWrite(se, LOW);
      digitalWrite(sf, HIGH);
      digitalWrite(sg, HIGH);
      break;
  }

}


/**
   turn Off All 7 segment display at once
*/
void disp_Off() {
  digitalWrite(disp1, LOW);
  digitalWrite(disp2, LOW);
  digitalWrite(disp3, LOW);
  digitalWrite(disp4, LOW);
  //  delay(100);
}

/**
   The standard Arduino loop function used for repeating tasks.
*/
void loop() {

  //Every second LED Blinking 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis; // save the last time you blinked the LED
    if (ledState == LOW) {
      ledState = HIGH;
    }
    else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState); // set the LED with the ledState of the variable:
  }  

  digitalWrite(A0, LOW); //Reset Button Low

  bt_cnt_up = digitalRead(A2) ;   //Button for digit count
  bt_disp_up = digitalRead(A1) ;  //Button for display change
  bt_fin_time = digitalRead(A3) ; //Button for Finalize Time

  //Display Selection to increse digits
  if (bt_disp_up == HIGH) {
    Update_Mode = 1;
    disp_Off();
    disp_cnt = disp_cnt + 1;
    delay(200);
    if (disp_cnt == 4)
    {
      disp_cnt = 0;
    }
    if (debug)
    {
      mySerial.println("\nDisplay Selected :");
      mySerial.println(disp_cnt);
    }
  }

  //Display digit Increment
  if (bt_cnt_up == HIGH) {
    Update_Mode = 1;

    switch (disp_cnt)
    {
      case 0:
        hh1cnt += 1;
        break;
      case 1:
        hh2cnt += 1;
        break;
      case 2:
        mm1cnt += 1;
        break;
      case 3:
        mm2cnt += 1;
        break;
    }
    delay(200);

    if (hh1cnt == 3) {
      hh1cnt = 0;
    }
    if ((hh1cnt == 2) & (hh2cnt == 5)) {
      hh2cnt = 0;
    }
    if ((hh1cnt < 2) & (hh2cnt == 10)) {
      hh2cnt = 0;
    }
    if (mm1cnt == 6) {
      mm1cnt = 0;
    }
    if (mm2cnt == 10) {
      mm2cnt = 0;
    }
    if (debug)
    {
      mySerial.println("\nSelected Time H H M M :");
      mySerial.print(hh1cnt);
      mySerial.print(" ");
      mySerial.print(hh2cnt);
      mySerial.print(" ");
      mySerial.print(mm1cnt);
      mySerial.print(" ");
      mySerial.print(mm2cnt);
      mySerial.println("\n");
    }
  }

  //Display Digits for Update Mode
  if (Update_Mode == 1) {
    digitalWrite(disp4, HIGH);
    show_num(hh1cnt % 10);
    delay(10);
    digitalWrite(disp4, LOW);

    digitalWrite(disp3, HIGH);
    show_num(hh2cnt % 10);
    delay(10);
    digitalWrite(disp3, LOW);

    digitalWrite(disp2, HIGH);
    show_num(mm1cnt % 10);
    delay(10);
    digitalWrite(disp2, LOW);

    digitalWrite(disp1, HIGH);
    show_num(mm2cnt % 10);
    delay(10);
    digitalWrite(disp1, LOW);
  }

  if (bt_fin_time == HIGH) {
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    DateTime now = rtc.now();
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), (hh1cnt * 10) + hh2cnt , (mm1cnt * 10) + mm2cnt, 0)); // (YYYY,MM,DD,HH,MM,SS)
    //    delay(100);
    disp_Off();
    delay(50);

    Hrs = (hh1cnt * 10) + hh2cnt;
    Min = (mm1cnt * 10) + mm2cnt;
    Time = Hrs * 100 + Min;

    if (debug)
    {
      mySerial.println("\nRTC DS1307 Time Set : HHMM ");
      mySerial.println(Time);
    }
    Update_Mode = 0;
  }

  // Default code No update button selected
  if (Update_Mode == 0) {    
  //For Synchronized with current Time in Proteus
    DateTime now = rtc.now();
  //mySerial.println(now.hour(), DEC);  //    mySerial.println(now.minute(), DEC);

    if (now.hour() > 24 or now.minute() > 60)   //RTC not initialized
    {
      if (debug2)
      {
        mySerial.println("Set Clock Time using Push Buttons.");
        mySerial.println("Default 00:00");
        debug2 = 0;
      }
      Hrs = 0;
      Min = 0;
    }
    else
    {
      Hrs = now.hour();
      Min = now.minute();
      if (debug2)
      {
        mySerial.print("Clock Started with Time ");
        mySerial.print(Hrs);
        mySerial.print(":");
        mySerial.print(Min);
        mySerial.println("\n");
        debug2 = 0;
      }
    }

    if (Min == 60)
    {
      Min = 0;
      Hrs = Hrs + 1;
    }
//    if (Hrs == 13)
//    {
//      Hrs = 1;
//    }
    Time = Hrs * 100 + Min;

    digitalWrite(disp1, HIGH);
    show_num(Time % 10);
    delay(10);
    digitalWrite(disp1, LOW);
  //delay(10);

    digitalWrite(disp2, HIGH);
    show_num(((Time / 10)) % 10);
    delay(10);
    digitalWrite(disp2, LOW);
  //delay(10);

    digitalWrite(disp3, HIGH);
    show_num(((Time / 100)) % 10);
    delay(10);
    digitalWrite(disp3, LOW);
  //delay(10);

    digitalWrite(disp4, HIGH);
    show_num(int(Time / 1000));
    delay(10);
    digitalWrite(disp4, LOW);
  //delay(10);

    Min = Min + 1;
  }

}









































  //  if (buttonState != lastButtonState )
  //  {
  //    if (buttonState == HIGH)
  //    {
  //      mySerial.println(" => Reset Button Pressed ");
  //      Time = 0;
  //      Hrs = 0;
  //      Min = 0;
  //
  //      digitalWrite(disp1, HIGH);
  //      show_num(Time % 10);
  //      delay(10);
  //      digitalWrite(disp1, LOW);
  //
  //      digitalWrite(disp2, HIGH);
  //      show_num(((Time / 10)) % 10);
  //      delay(10);
  //      digitalWrite(disp2, LOW);
  //
  //      digitalWrite(disp3, HIGH);
  //      show_num(((Time / 100)) % 10);
  //      delay(10);
  //      digitalWrite(disp3, LOW);
  //
  //      digitalWrite(disp4, HIGH);
  //      show_num(int(Time / 1000));
  //      delay(10);
  //      digitalWrite(disp4, LOW);
  //
  //      digitalWrite(A0,LOW);
  //     }
  //  }
  //  lastButtonState = buttonState;

  //  Time = Hrs * 100 + Min;
  //
  //  digitalWrite(disp1, HIGH);
  //  show_num(Time % 10);
  //  delay(10);
  //  digitalWrite(disp1, LOW);
  //
  //  digitalWrite(disp2, HIGH);
  //  show_num(((Time / 10)) % 10);
  //  delay(10);
  //  digitalWrite(disp2, LOW);
  //
  //  digitalWrite(disp3, HIGH);
  //  show_num(((Time / 100)) % 10);
  //  delay(10);
  //  digitalWrite(disp3, LOW);
  //
  //  digitalWrite(disp4, HIGH);
  //  show_num(int(Time / 1000));
  //  delay(10);
  //  digitalWrite(disp4, LOW);
  //
  //  Min = Min + 1;
