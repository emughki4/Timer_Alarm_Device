void eepromWrite() {
  /* EEPROM.write(10, set_day);
  EEPROM.write(11, set_month);
  EEPROM.write(12, date1.toInt());
  EEPROM.write(13, date2.toInt());
  EEPROM.write(15, set_minute);
  EEPROM.write(16, set_hour);
  for (int k = 10; k <= 13; k++) {
    Serial.println(EEPROM.read(k));
  }*/
  for (int k = 2; k <= sizeof(date); k++) {  //Saving date to the eeprom
    if (k != 4 && k != 7) {
      EEPROM.write(k, date[k - 2]);
    }
  }

  for (int k = 0; k <= 5; k++) {  //Saving date to the eeprom
    if (k != 2) {                 //} && k != 5) {
      EEPROM.write(k + 50, Time[k]);
    }
  }
}

void eepromRead() {
  for (int k = 2; k <= sizeof(date); k++) {
    if (k != 4 && k != 7) {
      date[k - 2] = EEPROM.read(k);
    }
    Serial.print(date[k - 2]);
  }
  Serial.println();

  for (int k = 0; k <= 5; k++) {  //Saving date to the eeprom
    if (k != 2) {
      Time[k] = EEPROM.read(k + 50);
    }
    Serial.print(Time[k]);
  }
  if (Time[5] == '1') count2 = 1;
  else count2 = 0;
  Serial.println();
}

void convert() {
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
  if (set_hour == 12 && count2 == 0) set_hour = 0;
  Serial.println();
}