#include <EEPROM.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "RTClib.h"


RTC_DS3231 rtc;



char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };




const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);  

byte arrow[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b10001, 0b01010, 0b00100 };
const byte ROWS = 4;  //four rows
const byte COLS = 4;  //three columns
char keys[ROWS][COLS] = {
  { '7', '8', '9', 'A' },
  { '4', '5', '6', 'B' },
  { '1', '2', '3', 'C' },
  { '*', '0', '#', 'D' }
};
// byte rowPins[ROWS] = { 5, 6, 7, 4 };
// byte colPins[COLS] = { 3, 2, 1, 0 };


byte rowPins[ROWS] = { 0, 1, 2, 3 };
byte colPins[COLS] = { 4, 5, 6, 7 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String year, month, day, hour, minute, second, input_sec, input_min = "00", input_hr = "00", input_year, input_month, input_day;
int set_year, set_month, set_day, set_hour = 0, set_minute = 0, set_second = 0;
String state = "initial";
int next1, next2, count1 = 0, count2;
char digit1, digit2, digit3, digit4;
int j = 0;
long lasttime = 0, last = 0;
String date1, date2;
char date[15] = { '0', '0', '/', '0', '0', '/', '0', '0', '0', '0', '0' };
char tempDate[10] = {};
char Time[10] = { '0', '1', ':', '0', '0', '0' };
char tempTime[10] = {};
int led_buzzer = A0, output = A1;

void setup() {
  EEPROM.begin();   
  // Serial.begin(9600);
  pinMode(led_buzzer, OUTPUT);
  pinMode(output, OUTPUT);
  lcd.begin(16, 2);

#ifndef ESP8266
  while (!Serial)
    ;
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  rtc.adjust(DateTime(2022, 1, 22, 0, 31, 53));
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  eepromRead();
  convert();
  lcd.createChar(1, arrow);
}

void loop() {
  char key = keypad.getKey();
  DateTime now = rtc.now();


  if (key) {
    lcd.setCursor(0, 1);
    lcd.print(key);
    cancel(key);
    if (key == 'A') {
      if (state == "initial" || state == "time saved") {
        next1 = 0;
        next2 = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Date:               ");
        lcd.setCursor(6, 0);
        lcd.write(1);
        lcd.setCursor(next1 + 6, 1);
        for (int k = 0; k <= 9; k++) {
          lcd.print(date[k]);
        }
        state = "input date";
      } else if (state == "input date") {
        lcd.setCursor(next1 + 5, 0);
        lcd.print("  ");
        switch (next1) {
          case 0:
            // tempDate[next1] = key;
            tempDate[0] = date[0];
            tempDate[1] = date[1];
            next1 = next1 + 3;
            lcd.setCursor(6, 0);
            lcd.print("  ");
            break;
          case 1:
            tempDate[next1] = date[next1];
            next1 = next1 + 2;
            lcd.setCursor(6, 0);
            lcd.print("  ");
            break;
          case 2:
            next1 = next1 + 1;
            break;
          case 3:
            lcd.setCursor(9, 0);
            lcd.print("  ");
            tempDate[3] = date[3];
            tempDate[4] = date[4];
            next1 = next1 + 3;
            break;
          case 4:
            lcd.setCursor(9, 0);
            lcd.print("  ");
            tempDate[next1] = date[next1];
            next1 = next1 + 2;
            break;
          case 5:
            next1 = next1 + 1;
            break;
          case 6:
            tempDate[6] = date[6];
            tempDate[7] = date[7];
            tempDate[8] = date[8];
            tempDate[9] = date[9];
            lcd.setCursor(12, 0);
            lcd.print("  ");
            next1 = next1 + 4;
            break;
          case 7:
            tempDate[7] = date[7];
            tempDate[8] = date[8];
            tempDate[9] = date[9];
            lcd.setCursor(12, 0);
            lcd.print("  ");
            next1 = next1 + 3;
            break;
          case 8:
            tempDate[8] = date[8];
            tempDate[9] = date[9];
            lcd.setCursor(13, 0);
            lcd.print("  ");
            next1 = next1 + 2;
            break;
          case 9:
            tempDate[next1] = date[next1];
            lcd.setCursor(14, 0);
            lcd.print("  ");
            next1 = next1 + 1;
            break;
          case 10:
            for (int k = 0; k < 15; k++) {
              Serial.print(tempDate[k]);
              if (k != 2 && k != 5 && k != 10) date[k] = tempDate[k];
              next1 = 0;
              // state = "date saved";
              Serial.print(",");
            }
            Serial.println();
            Serial.println();
            for (int k = 0; k < 15; k++) {
              Serial.print(date[k]);
              Serial.print(",");
            }  ////////////////////////////////////////////////////////////////////////time starts here
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Time:               ");
            lcd.setCursor(6, 0);
            lcd.write(1);
            lcd.setCursor(next2 + 6, 1);
            for (int k = 0; k <= 4; k++) {
              lcd.print(Time[k]);
            }
            if (count2 == 0) {
              lcd.setCursor(11, 1);
              lcd.print("AM");
            }
            if (count2 == 1) {
              lcd.setCursor(11, 1);
              lcd.print("PM");
            }
            state = "input time";
            if (state == "input time") {
            }
            return;
            break;
        }
      } else if (state == "input time") {
        switch (next2) {
          case 0:
            tempTime[0] = Time[0];
            tempTime[1] = Time[1];
            next2 = next2 + 3;
            lcd.setCursor(6, 0);
            lcd.print("  ");
            break;
          case 1:
            tempTime[next2] = Time[next2];
            next2 = next2 + 2;
            lcd.setCursor(6, 0);
            lcd.print("  ");
            break;
          case 2:
            next2 = next2 + 1;
            lcd.setCursor(6, 0);
            lcd.print("  ");
            break;
          case 3:
            lcd.setCursor(9, 0);
            lcd.print("  ");
            tempTime[3] = Time[3];
            tempTime[4] = Time[4];
            next2 = next2 + 2;
            break;
          case 4:
            lcd.setCursor(9, 0);
            lcd.print("  ");
            tempTime[next2] = Time[next2];
            next2 = next2 + 1;
            break;
          case 5:  //////////////////////////////////////////////////////////////////  Saved!
            next2 = next2 + 1;
            for (int k = 0; k <= 12; k++) {
              if (k != 2 && k != 5) date[k] = tempDate[k];
            }
            for (int k = 0; k <= 6; k++) {
              if (k != 2) Time[k] = tempTime[k];
            }
            lcd.setCursor(10, 0);
            lcd.print("  ");
            lcd.clear();
            lcd.setCursor(5, 0);
            lcd.print("Saved!");
            // lcd.setCursor(0, 0);
            // for (int k = 0; k <= 12; k++) {
            //   lcd.print(date[k]);
            // }
            j = 0;
            input_hr = "";
            input_min = "";
            input_day = "";
            input_month = "";
            input_year = "";
            for (int l = 0; l < 2; l++) {
              input_hr += Time[l];
              input_min += Time[l + 3];
            }
            // char date[15] = { '0', '0', '/', '0', '0', '/', '0', '0', '0', '0', '0' };
            for (int l = 0; l < 2; l++) {  //copy day and month from input_day and input_month to the main comparing variable
              input_day += date[l];
              input_month += date[l + 3];
            }
            for (int l = 6; l < 10; l++) {  //copy year from input_year to the main comparing variable
              input_year += date[l];
            }

            set_year = input_year.toInt();
            set_month = input_month.toInt();
            set_day = input_day.toInt();
            set_hour = input_hr.toInt();
            set_minute = input_min.toInt();
            Serial.println();

            if (count2 == 1) {
              set_hour = set_hour + 12;
              Time[5] = '1';
            } else Time[5] = '0';
            Serial.print("count2:  ");
            Serial.print(Time[5]);

            if (set_hour == 12 && count2 == 0) set_hour = 0;
            Serial.print(set_day);
            Serial.print("/");
            Serial.print(set_month);
            Serial.print("/");
            Serial.println(set_year);

            Serial.print(set_hour);
            Serial.print(":");
            Serial.println(set_minute);
            eepromWrite();

            delay(1600);
            lcd.clear();
            state = "time saved";
            break;
        }
      }
    } else if (isDigit(key) && state == "input date") {
      switch (next1) {
        case 0:
          if (key <= '3') {
            tempDate[next1] = key;
            lcd.setCursor(next1 + 6, 1);
            lcd.print(key);
            next1 = next1 + 1;
            digit1 = key;
          } else {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          }
          break;
        case 1:
          if (digit1 == '3' && key > '1') {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          } else {
            tempDate[next1] = key;
            lcd.setCursor(next1 + 6, 1);
            lcd.print(key);
            next1 = next1 + 2;
            lcd.setCursor(7, 0);
            lcd.print(" ");
          }
          break;
        case 3:
          if (key <= '1') {
            tempDate[next1] = key;
            lcd.setCursor(next1 + 6, 1);
            lcd.print(key);
            next1 = next1 + 1;
            digit1 = key;
          } else {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          }
          break;
        case 4:
          if (digit1 == '1' && key > '2') {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          } else {
            tempDate[next1] = key;
            lcd.setCursor(next1 + 6, 1);
            lcd.print(key);
            next1 = next1 + 2;
            lcd.setCursor(10, 0);
            lcd.print(" ");
          }
          break;
        case 6:
          if (key <= '2') {
            tempDate[next1] = key;
            lcd.setCursor(next1 + 6, 1);
            lcd.print(key);
            next1 = next1 + 1;
          } else {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          }
          break;
        case 7:
          tempDate[next1] = key;
          lcd.setCursor(next1 + 6, 1);
          lcd.print(key);
          next1 = next1 + 1;
          break;
        case 8:
          tempDate[next1] = key;
          lcd.setCursor(next1 + 6, 1);
          lcd.print(key);
          next1 = next1 + 1;
          break;
        case 9:
          tempDate[next1] = key;
          lcd.setCursor(next1 + 6, 1);
          lcd.print(key);
          next1 = next1 + 1;
          break;
      }
      // next1 = next1 + 1;
    } else if (isDigit(key) && state == "input time") {  ///////////////////////////////////////////////////// input time digits starts
      switch (next2) {
        case 0:
          if (key <= '1') {
            tempTime[next2] = key;
            lcd.setCursor(next2 + 6, 1);
            lcd.print(key);
            next2 = next2 + 1;
            digit1 = key;
          } else {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          }
          break;
        case 1:
          if (digit1 == '1' && key > '2') {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          } else {
            tempTime[next2] = key;
            lcd.setCursor(next2 + 6, 1);
            lcd.print(key);
            next2 = next2 + 2;
            lcd.setCursor(7, 0);
            lcd.print(" ");
          }
          break;
        case 3:
          if (key <= '5') {
            tempTime[next2] = key;
            lcd.setCursor(next2 + 6, 1);
            lcd.print(key);
            next2 = next2 + 1;
          } else {
            digitalWrite(led_buzzer, HIGH);
            delay(200);
            digitalWrite(led_buzzer, LOW);
          }
          break;
        case 4:
          tempTime[next2] = key;
          lcd.setCursor(next2 + 6, 1);
          lcd.print(key);
          next2 = next2 + 1;
          // lcd.setCursor(10, 0);
          // lcd.print(" ");
          break;
        case 5:
          count2 = !count2;
          if (count2 == 0) {
            lcd.setCursor(11, 1);
            lcd.print("AM");
            // count2 = 1;
            return;
          } else if (count2 == 1) {
            lcd.setCursor(11, 1);
            lcd.print("PM");
            // count2 = 0;
            return;
          }
          break;
      }
    }  /////////////////////////////////////////////////////////////////////////////////////////////// input time digits ends
    if (next1 < 11 && next1 != 2 && next1 != 5 && next1 != 10 && state == "input date") {
      lcd.setCursor(next1 + 5, 0);
      lcd.print(" ");
      lcd.write(1);
      lcd.setCursor(next1 + 6, 1);
      // if (key != 'A') lcd.print(key);
    } else if (next2 < 6 && next2 != 2 && next2 != 6 && state == "input time") {
      lcd.setCursor(next2 + 5, 0);
      lcd.print(" ");
      lcd.write(1);
      lcd.setCursor(next2 + 6, 1);
      // if (key != 'A') lcd.print(key);
    }
  }
  if (state == "initial" || state == "time saved") {
    checkDate();
    checkTime();
  }
  dis();
  alarm();
}



void checkTime() {
  String AmPm;
  DateTime now = rtc.now();
  int hour1 = (int)now.hour();
  if (hour1 > 12) {
    hour1 = hour1 - 12;
    AmPm = " PM";
  } else AmPm = " AM";
  if (hour1 < 10) hour = "0" + (String)hour1;
  else hour = (String)hour1;

  if ((int)now.minute() < 10) minute = "0" + (String)now.minute();
  else minute = now.minute();

  if ((int)now.second() < 10) second = "0" + (String)now.second();
  else second = now.second();

  String time = hour + ":" + minute + ":" + second;
  lcd.setCursor(2, 0);
  lcd.print(time);
  lcd.print(AmPm);
}


void checkDate() {
  DateTime now = rtc.now();
  int year1 = (int)now.year();
  int month1 = (int)now.month();
  int day1 = now.day();
  if (year1 < 10) year = "0" + (String)now.year();
  else year = (String)year1;
  if (month1 < 10) month = "0" + (String)now.month();
  else month = (String)month1;
  if (day1 < 10) day = "0" + (String)day1;
  else day = (String)day1;

  String date = day + "/" + month + "/" + year;
  lcd.setCursor(2, 1);
  lcd.print(date);
}
