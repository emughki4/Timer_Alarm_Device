void cancel(char keyval) {
  if (keyval == 'B' && state == "input date") {
    switch (next1) {
      case 0:
        lcd.clear();
        count1 = false;
        state = "initial";
        next1 = 0;
        next2 = 0;
        break;
      case 1:
        next1 = next1 - 1;
        lcd.setCursor(next1 + 7, 0);
        lcd.print("  ");
        break;
      case 3:
        lcd.setCursor(next1 + 6, 0);
        lcd.print("  ");
        next1 = next1 - 2;
        break;
      case 4:
        lcd.setCursor(next1 + 6, 0);
        lcd.print("  ");
        next1 = next1 - 1;
        break;
      case 6:
        lcd.setCursor(next1 + 6, 0);
        lcd.print("  ");
        next1 = next1 - 2;
        break;
      case 7:
        lcd.setCursor(next1 + 6, 0);
        lcd.print("  ");
        next1 = next1 - 1;
        break;
      case 8:
        lcd.setCursor(next1 + 6, 0);
        lcd.print("  ");
        next1 = next1 - 1;
        break;
      case 9:
        lcd.setCursor(next1 + 6, 0);
        lcd.print("  ");
        next1 = next1 - 1;
        break;
      case 10:
        lcd.setCursor(next1 + 6, 0);
        lcd.print("  ");
        next1 = next1 - 1;
        break;
    }
  }
  if (keyval == 'B' && state == "input time") {  ////////////////////////////////////////////////////time cancel
    switch (next2) {
      case 1:
        next2 = next2 - 1;
        lcd.setCursor(next2 + 7, 0);
        lcd.print("  ");
        break;
      case 3:
        lcd.setCursor(next2 + 6, 0);
        lcd.print("  ");
        next2 = next2 - 2;
        break;
      case 4:
        lcd.setCursor(next2 + 6, 0);
        lcd.print("  ");
        next2 = next2 - 1;
        break;
      case 5:
        lcd.setCursor(next2 + 6, 0);
        lcd.print("  ");
        next2 = next2 - 1;
        break;
    }
  }
  if (keyval == 'D' && count1 == true) {
    digitalWrite(output, LOW);
    digitalWrite(led_buzzer, LOW);
    count1 = false;
    state = "initial";
    next1 = 0;
    next2 = 0;
  }
}