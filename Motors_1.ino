#include <Servo.h>
// объявляем пины
#define PIN_ENA 10 // Вывод управления скоростью вращения мотора №1
#define PIN_ENB 5 // Вывод управления скоростью вращения мотора №2
#define PIN_IN1 9 // Вывод управления направлением вращения мотора №1
#define PIN_IN2 8 // Вывод управления направлением вращения мотора №1
#define PIN_IN3 7 // Вывод управления направлением вращения мотора №2
#define PIN_IN4 6 // Вывод управления направлением вращения мотора №2
#define PIN_Hor 3 // Вывод управления направлением горизонтального вращения башни
#define PIN_Vert 11 // Вывод управления направлением вертикального вращения пушки
#define PIN_Run 2 // Вывод управления диодом при жвижении
#define PIN_Fight 12 // Вывод управления диодом в боевом положении
#define PIN_Fire 13 // Вывод управления диодом во время выстрела
#define PIN_Shot 4 // Вывод управления лазером

#define DEBUG_LED 1//Tест
#define LED 13


int anglH = 0, anglV = 0, trig1 = 0, trig2 = 0, fire = 0;
char input;
Servo servoH;
Servo servoV;

void setup() {
  Serial.begin(9600);
  // запускаем пины
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_Run, OUTPUT);
  pinMode(PIN_Fight, OUTPUT);
  pinMode(PIN_Fire, OUTPUT);
  pinMode(PIN_Shot, OUTPUT);
  servoH.attach(PIN_Hor);
  servoV.attach(PIN_Vert);

  pinMode(LED, OUTPUT); //Тест

  // запускаем процедуру танца
  motors(0, 0);
  Run();
}

void loop() {
  if (Serial.available()) {
    input = Serial.read();//сделать блокировку движения при стрельбе и наоборот
    if (fire == 0) {
      if (input == 'S') motors(0, 0);
      if (input == 'F') motors(255, 255);
      if (input == 'B') motors(-255, -255);
      if (input == 'L') motors(-255, 255);
      if (input == 'R') motors(255, -255);
      if (input == 'G') motors(55, 255);
      if (input == 'I') motors(255, 55);
      if (input == 'J') motors(-255, -55);
      if (input == 'H') motors(-55, -255);
    }
    if (fire == 1) {
      if (input == 'R') {
        digitalWrite(LED, HIGH);
        anglH++; /*Serial_delay(25);*/ trig1 = 1; // Увеличиваем угол поворота башни
      }
      if (input == 'F') {
        anglV++;  // Увеличиваем угол поворота пушки
        trig2 = 1;
      }
      if (input == 'L') {
        anglH--;  // Уменьшаем угол поворота башни
        trig1 = 1;
      }
      if (input == 'B') {
        anglV--;  // Уменьшаем угол поворота пушки
        trig2 = 1;
      }
      if (input == 'W' || input == 'w') shot();
    }
    if (input == 'x') Run(); // Переход в режим движения
    if (input == 'X') {
      motors(0, 0);  // Переход в режим стрельбы
      digitalWrite(PIN_Fight, HIGH);
      digitalWrite(PIN_Run, LOW);
      fire = 1;
    }
  }

servoH.write(90);
digitalWrite(LED, LOW);
  if (trig1 == 1) servoH.write(anglH); // Поворот башни
  if (trig2 == 1) servoV.write(anglV); // Поворот пушки
  trig1 = 0;
  trig2 = 0;
}

void Serial_delay(int tt) {
  unsigned long int t = millis();
  while (t + tt > millis()) {
    if (Serial.available())
      Serial.read();
  }
}

void motors(int speed1, int speed2) {
  if (DEBUG_LED) digitalWrite(LED, speed1 != 0);
  motor(1, speed1);
  motor(2, speed2);
}

void motor(int number, int speed) {
  if (number == 1) {
    analogWrite(PIN_ENA, abs(speed) );
    digitalWrite(PIN_IN1, speed > 0);
    digitalWrite(PIN_IN2, speed < 0);
  }
  else {
    analogWrite(PIN_ENB, abs(speed) );
    digitalWrite(PIN_IN3, speed > 0);
    digitalWrite(PIN_IN4, speed < 0);
  }
}

void shot() {
  digitalWrite(PIN_Fire, HIGH);
  digitalWrite(PIN_Shot, HIGH);
  delay(400);
  digitalWrite(PIN_Fire, LOW);
  digitalWrite(PIN_Shot, LOW);
  delay(1000);
}

void Run() {
  servoH.write(0); // Возвращаем башню и пушку в безапасное начальное положение
  servoV.write(0);
  digitalWrite(PIN_Run, HIGH);
  digitalWrite(PIN_Fight, LOW);
  fire = 0;
}
