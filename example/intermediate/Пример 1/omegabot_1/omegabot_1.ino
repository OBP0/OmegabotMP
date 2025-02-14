#include <Servo.h>

/*
 * Для начала определяем пины к которым подключены моторы
 */

#define MOTOR_DIR_L 7 //Пин контроля направления левого мотора
#define MOTOR_DIR_R 4 //Пин контроля направления правого мотора

#define MOTOR_POW_L 6 //Пин для контроля мощности левого мотора
#define MOTOR_POW_R 5 //Пин для контроля мощности правого мотора

#define SERVO_PIN 9 //Пин контроля направления левого мотора
#define SERVO_MAX_ANG 180 //Пин контроля направления правого мотора
#define SERVO_MIN_ANG 0 //Пин контроля направления правого мотора
#define SERVO_DEG_SEC 120 //Скорость поворота сервы при получении команды (град/сек)

unsigned long servo_rotation_tmer = 0; //Счетчик для поворота сервы
float servo_angle = 90; //Текущий угол сервы
Servo servo_1;

int speed_level = 0;

//Учет команд
#define COMMAND_ACTUAL_TIME 80 //Время актуальноси команды с момента её получения
unsigned long terminate_cmd_timer = 0;  //Таймер актуальности команды
char cmd = 'S'; //Имя команды



void setup() {
  //Настройка обмена данными
  Serial.begin(9600); //Открываем последовательный порт для обмена данными c bluetooth модулем

  //Настройка двигателей
  pinMode(MOTOR_DIR_L, OUTPUT);
  pinMode(MOTOR_DIR_R, OUTPUT);
  pinMode(MOTOR_POW_L, OUTPUT);
  pinMode(MOTOR_POW_R, OUTPUT);

  servo_1.attach(SERVO_PIN);
  servo_1.write(servo_angle);
  
}

void loop() {
  //Получение команд из Serial
  if(Serial.available()){
    //Если в посдеовательном порту что-то есть - читаем ,байт
    char cmd_temp = Serial.read();
    switch (cmd_temp)
    {
    case 'F':
    case 'B':
    case 'L':
    case 'R':
    case 'U':
    case 'D':
      //Если пришла известная команда для движения
      cmd = cmd_temp;
      terminate_cmd_timer = millis(); //Сбрасываем счетчик актуальности команды
      break;
    case 'E':
      speed_level += 1;
      if(speed_level > 3) speed_level = 0;
    break;
    }
  }

  switch (cmd)
  {
  case 'F':
    //Направление
    digitalWrite(MOTOR_DIR_L, HIGH);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, HIGH);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 80 + speed_level * 30); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 80 + speed_level * 30); //Установка скорости мотора
    break;

    case 'B':
    //Направление
    digitalWrite(MOTOR_DIR_L, LOW);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, LOW);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 80 + speed_level * 30); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 80 + speed_level * 30); //Установка скорости мотора
    break;

    case 'L':
    //Направление
    digitalWrite(MOTOR_DIR_L, LOW);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, HIGH);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 80 + speed_level * 15); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 80 + speed_level * 15); //Установка скорости мотора
    break;

    case 'R':
    //Направление
    digitalWrite(MOTOR_DIR_L, HIGH);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, LOW);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 80 + speed_level * 15); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 80 + speed_level * 15); //Установка скорости мотора
    break;

    case 'S':
    //Направление
    digitalWrite(MOTOR_DIR_L, LOW);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, LOW);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 0); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 0); //Установка скорости мотора
    break;

    case 'U':
    if(servo_rotation_tmer + 40 < millis()){  //Ограничиваем частоту расчета угла сервы т.к это бесполезно
      servo_angle += SERVO_DEG_SEC*((millis()-servo_rotation_tmer)/1000.0); // Вычисляем угол доворота
      servo_rotation_tmer = millis(); //Сбрасываем таймер
      if (servo_angle > SERVO_MAX_ANG) servo_angle = SERVO_MAX_ANG; //Ограничиваем углы
      servo_1.write(servo_angle); //Устанавливаем угол сервы
    }
    break;

    case 'D':
    if(servo_rotation_tmer + 40 < millis()){  //Ограничиваем частоту расчета угла сервы т.к это бесполезно
      servo_angle -= SERVO_DEG_SEC*((millis()-servo_rotation_tmer)/1000.0); // Вычисляем угол доворота
      servo_rotation_tmer = millis(); //Сбрасываем таймер
      if (servo_angle < SERVO_MIN_ANG) servo_angle = SERVO_MIN_ANG; //Ограничиваем углы
      servo_1.write(servo_angle); //Устанавливаем угол сервы
    }
    break;
  }

  if(terminate_cmd_timer + COMMAND_ACTUAL_TIME < millis()){ //Если слишком долго не приходило новой команды, сбрасываем её
    cmd = 'S';
    servo_rotation_tmer = millis(); //Сбрасываем счетчик поворота счервы
  }

  
}
