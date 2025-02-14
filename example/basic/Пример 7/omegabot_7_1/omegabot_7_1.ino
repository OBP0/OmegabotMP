#include <Arduino.h>
#include <Servo.h>

/***
 * Для начала определяем пины к которым подключены моторы
 */

#define SERVO_PIN 8 //Пин контроля направления левого мотора
#define SERVO_MAX_ANG 180 //Пин контроля направления правого мотора
#define SERVO_MIN_ANG 0 //Пин контроля направления правого мотора
#define SERVO_DEG_SEC 120 //Скорость поворота сервы при получении команды (град/сек)

unsigned long servo_rotation_tmer = 0; //Счетчик для поворота сервы
float servo_angle = 90; //Текущий угол сервы
Servo servo_1;

//Учет команд
#define COMMAND_ACTUAL_TIME 65 //Время актуальноси команды с момента её получения
unsigned long terminate_cmd_timer = 0;  //Таймер актуальности команды
char cmd = 'S'; //Имя команды



void setup() {
  //Настройка обмена данными
  Serial.begin(9600); //Открываем последовательный порт для обмена данными c bluetooth модулем
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
    case 'U':
    case 'D':
      //Если пришла известная команда для движения
      cmd = cmd_temp;
      terminate_cmd_timer = millis(); //Сбрасываем счетчик актуальности команды
      break;
    }
  }

  if(cmd == 'U'){
    if(servo_rotation_tmer + 30 < millis()){  //Ограничиваем частоту расчета угла сервы т.к это бесполезно
      servo_angle += SERVO_DEG_SEC*((millis()-servo_rotation_tmer)/1000.0); // Вычисляем угол доворота
      servo_rotation_tmer = millis(); //Сбрасываем таймер
      if (servo_angle > SERVO_MAX_ANG) servo_angle = SERVO_MAX_ANG; //Ограничиваем углы
      servo_1.write(servo_angle); //Устанавливаем угол сервы
    }
  }else if (cmd == 'D')
  {
    if(servo_rotation_tmer + 30 < millis()){  //Ограничиваем частоту расчета угла сервы т.к это бесполезно
      servo_angle -= SERVO_DEG_SEC*((millis()-servo_rotation_tmer)/1000.0); // Вычисляем угол доворота
      servo_rotation_tmer = millis(); //Сбрасываем таймер
      if (servo_angle < SERVO_MIN_ANG) servo_angle = SERVO_MIN_ANG; //Ограничиваем углы
      servo_1.write(servo_angle); //Устанавливаем угол сервы
    }
  }

  if(terminate_cmd_timer + COMMAND_ACTUAL_TIME < millis()){ //Если слишком долго не приходило новой команды, сбрасываем её
    cmd = 'S';
    servo_rotation_tmer = millis(); //Сбрасываем счетчик поворота счервы
  }
}
