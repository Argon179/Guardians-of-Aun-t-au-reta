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

int anglH = 0, anglV = 0, trig1 = 0, trig2 = 0, fire = 0, v=255;
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

  // запускаем процедуру танца
  motors(0, 0);
  Run();
}

void loop() {
  if (Serial.available()) {
    input = Serial.read();
    if (fire == 0) {
      if (input == '0' || input == '1' || input == '2' || input == '3') v=255/3;// Изменение скорости движения
      if (input == '4' || input == '5' || input == '6') v=2*255/3;
      if (input == '7' || input == '8' || input == '9' || input == 'q') v=255;
      if (input == 'S') motors(0, 0);
      if (input == 'F') motors(v, -v);
      if (input == 'B') motors(-v, v);
      if (input == 'L') motors(v, v);
      if (input == 'R') motors(-v, -v);
      if (input == 'G') motors(v/4, -v);
      if (input == 'I') motors(v, -v/4);
      if (input == 'J') motors(-v, v/4);
      if (input == 'H') motors(-v/4, v);
    }
    if (fire == 1) {
      if (input == 'L') {
        anglH++; trig1 = 1; // Увеличиваем угол поворота башни
      }
      if (input == 'F') {
        anglV++;  // Увеличиваем угол поворота пушки
        trig2 = 1;
      }
      if (input == 'R') {
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
  
  if (anglH<0) anglH=0;
  if (anglV<0) anglV=0;
  if (trig1 == 1) servoH.write(anglH); // Поворот башни
  if (trig2 == 1) servoV.write(anglV); // Поворот пушки
  trig1 = 0;
  trig2 = 0;
}

void motors(int speed1, int speed2) {
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
  anglH=90;
  anglV=0;
  servoH.write(90); // Возвращаем башню и пушку в безапасное начальное положение
  servoV.write(0);
  digitalWrite(PIN_Run, HIGH);
  digitalWrite(PIN_Fight, LOW);
  fire = 0;
}
