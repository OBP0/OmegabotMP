#pragma once
#include <Arduino.h>
#include "omegabot_easy_tasker.h"


unitask::unitask(void (*task_ptr)(), unsigned int interval, unsigned long initial_time = 0)
{
    prev = initial_time;
    task = task_ptr;
    interv = interval;
}

unitask::unitask(unsigned int interval, unsigned long initial_time = 0)
{
    prev = initial_time;
    interv = interval;
}

void unitask::makeTask(){
    if(millis()-prev > interv && active){
        task();
        prev = millis();
    }
}

bool unitask::checkTask(){
    if(millis()-prev > interv && active){
        prev = millis();
        return true;
    }
    return false;
};