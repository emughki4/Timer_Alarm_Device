void alarm() {
  DateTime now = rtc.now();
  if (set_year == now.year()) {
    if (set_month == now.month()) {
      if (set_day == now.day()) {
        if (set_hour == now.hour()) {
          if (set_minute == now.minute()) {
            if (j == 0) {
              lcd.clear();
              j = 1;
              count1 = true;
              lasttime = millis();
              lcd.setCursor(2, 0);
              lcd.print("Ringing!!!");
              digitalWrite(output, HIGH);
              state = "ringing";
            }
          }
        }
      }
    }
  }


  if (state == "ringing" && (millis() - lasttime > 30000)) {
    // digitalWrite(led_buzzer, LOW);
    digitalWrite(led_buzzer, LOW);
    state = "initial";
  } else if (state == "ringing" && millis() - last > 500) {
    digitalWrite(led_buzzer, HIGH);
    if (millis() - last >= 1000) {
      digitalWrite(led_buzzer, LOW);
      last = millis();
    }
  }
}