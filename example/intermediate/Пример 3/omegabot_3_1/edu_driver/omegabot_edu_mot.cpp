#pragma once
#include <Arduino.h>
#include "omegabot_edu_mot.h"
#include "general/pid.h"
#include "general/omegabot_easy_tasker.h"
#include "general/filter/median_filter.h"


omegabot_motor::omegabot_motor(_pair_type_ type)
{
    if(type == omegabot_motor::PAIR_TYPE_LEFT){
        /*
        Набор параметров применяемых в случае работы с левой парой моторов
        */
        dir_pin = 7;    //Пин направления
        power_pin = 6;  //Пин "мощности"
        encoder_pin = 2;//Пин энкодерра
    }else if (type == omegabot_motor::PAIR_TYPE_RIGHT)
    {
        /*
        Набор параметров применяемых в случае работы с правой парой моторов
        */
        dir_pin = 4;    //Пин направления
        power_pin = 5;  //Пин "мощности"
        encoder_pin = 3;//Пин энкодерра
    }


    pinMode(OUTPUT, dir_pin);
    pinMode(OUTPUT, power_pin);
    pinMode(INPUT_PULLUP, encoder_pin);


    median_filter_init(&encoder_filter, encoder_filter_buffer, 16);  //Инициализация медианного фильтра

    encoder_task = unitask(1000/target_freq_enc);   //Настройка периодов для выполнения задачи опроса энкодера
    motor_task = unitask(1000/target_freq_mot);     //Настройка периодов для выполенния задачи расчета и выставления можности на моторе
    return;
}

void omegabot_motor::service_task(){
    if(encoder_task.checkTask()){
        check_encoder();    //Опрос энкодера
    }
    if(motor_task.checkTask()){
        calculate_motor();  //Расчет и передача мощности на моторы
    }
}

void omegabot_motor::check_encoder(){
    if(enc_prev_state != digitalRead(encoder_pin)){
        enc_prev_state = digitalRead(encoder_pin);
        if(enc_prev_state == 1){
            position_estimate++;
            odometer++;
            current_ticks =1000.0/(millis() - encoder_prev_sig);    //Расчет частоты срабатываний энкодера исходя из разницы между двумя ближайшими срабатываинями
            median_filter_insert_value(&encoder_filter, current_ticks); //Передача расчитанного значения в медианный фильтр
            encoder_prev_sig = millis();
        }
    }else if(1000.0/(millis() - encoder_prev_sig) < current_ticks){
        current_ticks =1000.0/(millis() - encoder_prev_sig);    //Расчет частоты срабатываний энкодера на основании того, что срабатываний энкодера небыло слишком долго -> частота вращения снизилась
        median_filter_insert_value(&encoder_filter, current_ticks); //Передача расчитанного значения в медианный фильтр ---непонятно что с этим делать, оно может всё сломать...---
    }
}

void omegabot_motor::calculate_motor(){
    motor_power += motor_pid.calculate(target_ticks, getTicks()); //Вычисление текушего шим на моторах и установка 
    motor_power = constrain(motor_power, 0, 255);
    if(target_ticks < 0.2) motor_power = 0;
    if(!safe_mode){
        digitalWrite(dir_pin, motor_direction);
        analogWrite(power_pin, motor_power);
    }else{
        analogWrite(power_pin, 0);
    }
}

float omegabot_motor::getRawTicks(){
    return current_ticks;
}

float omegabot_motor::getTicks(){
    float freq = median_filter_get_median(&encoder_filter);
    if(freq > 0.6) return freq;
    else return 0;
}

byte omegabot_motor::getPower(){
    return motor_power;
}

void omegabot_motor::setTickSpeed(float speed){
        target_ticks = speed;
}

void omegabot_motor::setPID_P(float val){
    motor_pid._Kp = val;
}

void omegabot_motor::setPID_I(float val){
    motor_pid._Ki = val;
}

void omegabot_motor::setPID_D(float val){
    motor_pid._Kd = val;
}

int omegabot_motor::getTickDistanceODO(){
    return odometer;

}  //Получить данные одометра
int omegabot_motor::getTickDistanceTRIP(){
    return position_estimate;
}

void omegabot_motor::clearDistance(){
    position_estimate = 0;
}

float omegabot_motor::getDistanceODO(){
    return float(odometer)/step_per_rot * dist_per_rot;

}  //Получить данные одометра

float omegabot_motor::getDistanceTRIP(){
    float trip = position_estimate/float(step_per_rot);
    return trip * dist_per_rot;
}