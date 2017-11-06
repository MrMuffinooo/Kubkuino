#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/sleep.h>
#include <ClickButton.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "vars.h"

/* Mozna zmieniac ale bez przesady */
#define _NAME (String)"Kubkuino_IR" //Mozna zmienic na jakakolwiek nazwe, ale ta pasuje :P
#define _PASS (String)"0000" //Nie chcemy zeby ktos nam sie bawil. Haslo do parowania.
#define ALARM_TIME 10000 //Ile trwa piszczenie alarmu.
#define DEBUG
#define VERSION "SW1.00 HW1.00 Pre"

#if F_CPU != 16000000
#undef F_CPU
#define F_CPU 16000000
#endif

Adafruit_SSD1306 display;

ClickButton ctrl(ctl, LOW);
SoftwareSerial bt(btx, brx);

void setup() {
  ctrl.debounceTime = 10;
  ctrl.longClickTime = 500;
  ctrl.multiclickTime = 400;
  pinMode(btpow, OUTPUT);
  pinMode(btkey, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  pinMode(ctl, INPUT);
  digitalWrite(btkey, LOW);
#if defined DEBUG
  digitalWrite(btpow, HIGH);
  hot = 30;
  cold = 25;
#else
  digitalWrite(btpow, LOW);
  hot = 60;
  cold = 47;
#endif
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  bt.begin(38400);
  splash(F(VERSION));
}

void loop() {
  pomiar = readTemp();
  drawmain(hot, cold, pomiar);
  alarm(pomiar, hot, cold);

  uint32_t czas = millis();
  while (millis() - czas <= 50) {
    ctrl.Update();
    if (ctrl.clicks == 1)
      powermenu();
    if (bt.available()) {
      String input = bt.readStringUntil(';');
      bt.flush();
      btctl(input);
    }
  }
}

double readTemp() {
  Wire.beginTransmission(0x5A);
  Wire.write(0x07);
  Wire.endTransmission(false);

  Wire.requestFrom(0x5A, 0x03);
  uint16_t ret = Wire.read();
  ret |= Wire.read() << 8;
  return ((ret * .02) - 273.15);
}

void drawmain(int _hot, int _cold, double _pomiar) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print(F("Goraca: "));
  display.print(_hot);
  display.write(247);
  display.println('C');
  display.print(F("Zimna:  "));
  display.print(_cold);
  display.write(247);
  display.println('C');

  display.setTextSize(2);
  display.print(_pomiar, 1);
  display.write(247);
  display.print('C');

  display.fillRoundRect(96, 16, 32, 16, 2, WHITE);
  display.fillCircle(93, 24, 6, WHITE);
  display.fillCircle(93, 24, 4, BLACK);

  if (_pomiar > _hot)
    display.drawBitmap(100, 0, state_hot, 24, 16, WHITE);
  else if (_pomiar > _cold)
    display.drawBitmap(100, 0, state_ready, 24, 16, WHITE);
  else
    display.drawBitmap(100, 0, state_cold, 24, 16, WHITE);

  if (conn)
    display.drawBitmap(104, 16, icon, 16, 16, 0);
  display.display();
  return void();
}

void btctl(String tinput) { //Usuwamy \r\n
  if (tinput == "\r\n" || tinput == "\n" || tinput == "\r") return void();
  tinput.trim();
  char command = tinput.charAt(0);
  String tempout;
  if (tinput.length() > 1)
    tinput.remove(0, 1);
  switch (command) {
    case 'e': bt.print(F("HELLO_IR\r\n"));
      conn = true; break; //Odpowiadamy - polaczono z dobrym urzadzeniem
    case 'r': pomiar = readTemp();
      bt.print(pomiar, 1);
      bt.print(F("\r\n")); break;
    case 't': if (pomiar > hot) bt.print(F("H\r\n"));
      else if (pomiar < cold) bt.print(F("C\r\n"));
      else bt.print(F("R\r\n")); break;
    case 'd': service(F("AT+ORGL"));
      service("AT+NAME=" + _NAME);
      service("AT+PSWD=" + _PASS); break; //Reset do fabrycznych
    case 'q': if (tinput == "quit") {
        conn = false; service(F("AT+DISC"));
        digitalWrite(btpow, LOW);//Rozlaczanie
        delay(100); digitalWrite(btpow, HIGH);
      } break;//tak trzeba bo sie arduino zacina
    case 'H': if (tinput.length() >= 1 && tinput[0] != '?')
        hot = atoi(tinput.c_str());//Set hot
      else if (tinput[0] == '?') bt.print((String)hot + "\r\n");
      break;
    case 'C': if (tinput.length() >= 1 && tinput[0] != '?')
        cold = atoi(tinput.c_str());//Set cold
      else if (tinput[0] == '?')
        bt.print((String)cold + "\r\n");
      break;
    case 'S': if (tinput.length() >= 2 ) service(tinput);
      break;
    case 'D': switch (tinput[0]) {
        case '0': if (tinput[1] == '0') { //LCD
            display.ssd1306_command(0xAE); lcdon = false;
          } else if (tinput[1] == '1') {
            display.ssd1306_command(0xAF); lcdon = true;
          } else if (tinput[1] == '?') {
            (lcdon) ? bt.print(F("1\r\n")) :
            bt.print(F("1\r\n"));
          } break;
        case '1': if (tinput[1] == '0') {
            dimlcd = false;
            display.dim(false);
          } else if (tinput[1] == '1') {
            dimlcd = true;
            display.dim(true);
          } else if (tinput[1] == '?') {
            dimlcd ? bt.print(F("1\r\n")) :
            bt.print(F("0\r\n"));
          } break;
        case '2': if (tinput[1] == '0') { //BT
            service(F("AT+DISC")); digitalWrite(btpow, LOW); conn = false;
          } if (tinput[1] == '1') digitalWrite(btpow, HIGH);
          break;
        case '3': if (tinput[1] == '0') { //LED
            ledon = false;
            digitalWrite(rled, LOW);
            digitalWrite(gled, LOW);
            digitalWrite(bled, LOW);
          } else if (tinput[1] == '1') ledon = true;
          else if (tinput[1] == '?') {
            (ledon) ? bt.print(F("1\r\n")) :
            bt.print(F("0\r\n"));
          } break;
        case '4': if (tinput[1] == '0') { //Piezo
            tone(buzz, 432); delay(500); noTone(buzz);
            glosny = false;//Wszystko poza tym mozna wywalic
          } else if (tinput[1] == '1') {
            tone(buzz, 432); delay(75); noTone(buzz); delay(250);
            tone(buzz, 432); delay(75); noTone(buzz);
            glosny = true;//J.w.
          } else if (tinput == "?") {
            (glosny) ? bt.print(F("1\r\n")) :
            bt.print(F("0\r\n"));
          } break;
        case '5': bt.print(F("1\r\n")); break;
        case '6':
          tempout = "";
          (lcdon) ? tempout += "1" : tempout += "0";
          (dimlcd) ? tempout += "1" : tempout += "0";
          (ledon) ? tempout += "1" : tempout += "0";
          (glosny) ? tempout += "1" : tempout += "0";
          tempout += "1\r\n";
          //Brak poprawki czyli zawsze plaskie. Kompatybilnosc wsteczna hyhy
          bt.print(tempout); break;
        case '8': poweroff(); break;
      } break;
  }
  bt.flush();
  return void();
}

//Tutaj wchodzi komenda bez "\r\n"!
void service(String a) {
#ifdef DEBUG
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println(a);
#endif
  if (a == "AT+DISC") conn = false;
  a += "\r\n";
  digitalWrite(btkey, HIGH);
  delay(500);
  for (uint8_t z = 0; z < a.length(); z++)
    bt.write(a[z]);
  long long temptime = millis();
  while (bt.available() == 0 && (millis() - temptime) <= 5000) {}
  if (bt.available() != 0)
    a = bt.readStringUntil('\0');
  else a = "REQUEST TIMEOUT\r\n";
  digitalWrite(btkey, LOW); delay(500);
  bt.flush();
  a.trim();
  for (uint8_t g = 0; g < a.length(); g++)
    if (a[g] == 0x0A || a[g] == 0x0D) a.remove(g);
  a.trim();
  if (conn) bt.print(a + "\r\n");

#ifdef DEBUG
  display.print(a);//Mozna wywalic ale pokazuje ze dziala
  display.display();
  delay(1000);
  display.clearDisplay();
#endif
  return void();
}

void bipczyk() {
  uint32_t tempbuzz = millis();
  tone(buzz, 432);
  while ((millis() - tempbuzz) <= ALARM_TIME && digitalRead(ctl)) {
    drawmain(hot, cold, readTemp());
    if (bt.available()) {
      String input = bt.readStringUntil(';');
      bt.flush();
      btctl(input);
    }
  }
  noTone(buzz);
  while (digitalRead(ctl) == LOW)
    drawmain(hot, cold, readTemp());
  ctrl.reset();
  return void();
}

void alarm(double _pomiar, int _hot, int _cold) {
  if (_pomiar > _hot)
    alarmState = 0b100;
  else if (_pomiar < _cold)
    alarmState = 0b110;
  else {
    bitClear(alarmState, 2);
    bitSet(alarmState, 0);
  }

  if (ledon) {
    switch (alarmState) {
      default:
        digitalWrite(rled, LOW);
        digitalWrite(gled, HIGH);
        digitalWrite(bled, LOW); break;
      case 0b100:
        digitalWrite(rled, HIGH);
        digitalWrite(gled, LOW);
        digitalWrite(bled, LOW); break;
      case 0b110:
        digitalWrite(rled, LOW);
        digitalWrite(gled, LOW);
        digitalWrite(bled, HIGH); break;
    }
  }
  if ((alarmState >> 0) && !(alarmState >> 2) && !(alarmState >> 1)) {
    if (glosny) bipczyk();
    alarmState = 0b110;
  }
  return void();
}

void powermenu() {
  int tmp = 0;
  int _dn = 0;
  bool draw = true;
  ctrl.reset();
  while (true) {
    while (ctrl.clicks != -1) {
      if (draw) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(1);
        for (int a = _dn; a < _dn + 4; a++) {
          (a == tmp) ? display.setTextColor(BLACK, WHITE) :
          display.setTextColor(WHITE, BLACK);
          display.print(powermodes[a]);
          switch (a) {
            case 0:
              digitalRead(btpow) ?
              display.println(F("ON      ")) :
              display.println(F("OFF     ")); break;
            case 1: ledon ?
              display.println(F("ON      ")) :
              display.println(F("OFF     ")); break;
            case 2: glosny ?
              display.println(F("ON      ")) :
              display.println(F("OFF     ")); break;
            case 3: dimlcd ?
              display.println(F("ON      ")) :
              display.println(F("OFF     ")); break;
          }
        }
        display.display();
        draw = false;
      }
      ctrl.Update();

      if (ctrl.clicks == 1) {
        (tmp < 5) ?
        tmp++ : tmp = 0;

        (tmp > 3 && tmp < 6) ?
        _dn = tmp - 3 : _dn = 0;
        draw = true;
        ctrl.reset();
      }
    }
    switch (tmp) {
      case 0: (digitalRead(btpow)) ? btctl("D20") : btctl("D21");
        break;
      case 1: ledon ? btctl(F("D30")) : btctl(F("D31")); break;
      case 2: glosny ? btctl(F("D40")) : btctl(F("D41")); break;
      case 3: dimlcd ? btctl(F("D10")) : btctl(F("D11")); break;
      case 4: return void(); break;
      case 5: poweroff(); return void(); break;
    }

    draw = true;
    ctrl.reset();
    while (!digitalRead(ctl));
  }
  return void();
}

void poweroff() {
  bool _btpow = false;
  if (digitalRead(btpow) == HIGH) _btpow = true;
  bool _dimlcd = dimlcd;
  bool _ledon = ledon;
  bool _lcdon = lcdon;
  if (_lcdon) btctl("D00;");
  if (_dimlcd) btctl("D10;");
  if (_btpow) btctl("D20;");
  if (_ledon) btctl("D30;");
  while (digitalRead(ctl) == LOW);
  attachInterrupt(0, poweron, LOW);
  delay(150);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  sleep_disable();
  if (_lcdon)btctl("D01;");
  if (_dimlcd) btctl("D11;");
  if (_btpow) btctl("D21;");
  if (_ledon) btctl("D31;");
  return void();
}

void poweron() {
  detachInterrupt(0);
  return void();
}

void splash(String _ver) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display.println("Kubkuino");
  display.setTextSize(1);
  display.println(_ver);
#if defined DEBUG
  display.print(__TIME__);
  display.print(" ");
  display.print(__DATE__);
#endif
  display.display();
  delay(2500);
  return void();
}

