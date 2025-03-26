
#define  OMEGA_DEBUG //Режим отладки для edu_driver/omegabot_edu_mot.h

#include <Arduino.h>

#include "edu_driver/omegabot_edu_mot.h"
#include "general/omegabot_easy_tasker.h"


omegabot_motor motor_left(omegabot_motor::PAIR_TYPE_LEFT);    //Создание левой пары моторов
omegabot_motor motor_right(omegabot_motor::PAIR_TYPE_RIGHT);  //Создание правой пары моторов


void send_data_1(){ //Вывод данных в Serial порт для построения графиков в VSCode при помощи плагина serial-plotter
  Serial.print(">vel:");
  Serial.print(motor_left.getTicks());
  Serial.print(",pow:");
  Serial.print(motor_left.getPower());
  Serial.print(",spd:");
  Serial.print(analogRead(A0)/10.0);
  Serial.print(",p:");
  Serial.print(motor_left.motor_pid._Kp);
  Serial.print(",i:");
  Serial.print(motor_left.motor_pid._Ki);
  Serial.print(",d:");
  Serial.println(motor_left.motor_pid._Kd);

  motor_left.setTickSpeed(analogRead(A0)/10.0); //Настройка скорости мотора при помощи потенциометра
  if(Serial.available()){ //Обработка команд пользователя для настройки коэффицентов PID
    char cmd = Serial.read();
    if(cmd == 'P'){
      motor_left.setPID_P(Serial.parseFloat());
    } else
    if(cmd == 'I'){
      motor_left.setPID_I(Serial.parseFloat());
    } else
    if(cmd == 'D'){
      motor_left.setPID_D(Serial.parseFloat());
    }
  }
  
}

void send_data(){ //Вывод пройденого роботом расстояния в метрах и установка скорости при помощи потенциометра
  Serial.println(motor_left.getDistanceTRIP());

  motor_left.setTickSpeed(analogRead(A0)/10.0);  
}

void moveTill(){
  if((motor_left.getDistanceTRIP() + motor_right.getDistanceTRIP())/2 < 0.4){
    motor_right.target_ticks = 15; //Установка скорости движения 15тик/сек
    motor_left.target_ticks = 15; //Установка скорости движения 15тик/сек
  }else{
    motor_left.target_ticks = 0; //Установка скорости движения 0тик/сек (остановка)
    motor_right.target_ticks = 0; //Установка скорости движения 0тик/сек (остановка)
  }
}

//!!!Раскомментировать одину из строчек для выбора одного из режимов работы робота!!!
//unitask move_task(send_data, 1000/10);  //Режим контроля растояния (вывод пройденного расстояния) (1000/10 - 10Гц)
//unitask move_task(send_data_1, 1000/10);  //Режим настройки PID (1000/10 - 10Гц)
unitask move_task(moveTill, 1000/10); //Режим прохождения расстояния 0.4м и последующей остановки (1000/10 - 10Гц)


void setup() {
  Serial.begin(9600);
}

void loop() {
  motor_left.service_task();    //Выполнение обработки расстояния и скорости мотора (Сервисная задача)
  motor_right.service_task();   //Выполнение обработки расстояния и скорости мотора (Сервисная задача)
  move_task.makeTask();         //Выполнение контроля пройденого расстояния и принятие решениия о дальнейшем движении или остановке
}