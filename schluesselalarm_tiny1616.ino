//tiny1616
#include <avr/sleep.h>
#include <avr/power.h>

boolean alarm __attribute__ ((section (".noinit")));

byte sleep_counter __attribute__ ((section (".noinit")));

int cycles = 0;

void setup() {
  // put your setup code here, to run once:
  if (sleep_counter > 1) sleep_counter = 1; //TESTING!!! AUSKOMMENTIEREN!!!!
  sleep_counter --;

  if (sleep_counter != 0) {
    sleep();
  }



  asm("WDR");
  //CCP = 0xD8; //unlock writing to WDT bits
  _PROTECTED_WRITE(WDT.CTRLA, 0x00); //WDT off

  power_all_enable();
  pinMode(6, INPUT_PULLUP); //PB3

}


void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(6) == HIGH) {
    if (!alarm) {
    info_tone();
    alarm = true;
    sleep_counter = 10; //10*8s
    sleep();
    }
    else {
      while (digitalRead(6) == HIGH) {
        if (cycles < 10) {
          tone_loop();
          cycles++;
        } 
        else {
          pinMode(10, OUTPUT);
          digitalWrite(10, HIGH);
          delay(100);
          digitalWrite(10, LOW);
          delay(1000);
        }

      }
      alarm = false;
      sleep_counter = 5;
      sleep();
    }

  }
  else {
    alarm = false;
    sleep_counter = 5; //5*8s
    sleep();
  }

  //deep sleep 30s
}





void info_tone() {
  pinMode(10, OUTPUT);
  pinMode(5, OUTPUT);
  tone(5, 1000, 50);
  delay(100);
  digitalWrite(10, HIGH);
  tone(5, 1000, 50);
  delay(100);
  digitalWrite(10, LOW);
}

void tone_loop() {
pinMode(5, OUTPUT);
pinMode(10, OUTPUT);
tone(5,1000, 200);
delay(200);
tone(5, 600, 200);
delay(200);
digitalWrite(10, HIGH);
delay(100);
digitalWrite(10, LOW);
delay(100);
}

void sleep() {
  for (int i = 0; i < 20; i++) { //this saves a ton of energy!
    pinMode(i, OUTPUT);
  }
  //watchdog setup!
  //CCP = 0xD8; //unlock writing to WDT bits
  //_PROTECTED_WRITE(WDT.CTRLA, 0b00001011); //WDT to 8s normal operation
  asm("WDR");
  power_all_disable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
}

