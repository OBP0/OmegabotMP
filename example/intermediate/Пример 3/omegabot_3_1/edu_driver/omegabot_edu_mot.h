#pragma once
#include <Arduino.h>
#include "general/pid.h"
#include "general/omegabot_easy_tasker.h"
#include "general/filter/median_filter.h"

/***
 * @brief Класс отвечающий за работу с моторами
 *          на Omegabot поддерживающими обратную связь(Энкодеры).
 *          Предназначен для обработки 1й колесной пары.
 */
class omegabot_motor
{
//private:
public:

    bool safe_mode = false; //Безопасный режим работы (моторы не крутятся шим автоматически утсанавливается 0)

    byte power_pin; //Номер пина отвечающего за уровень можности передаваемый на привод колесной пары
    byte dir_pin;   //Номер пина отвечающего за направление вращения колесной пары

    byte encoder_pin;

    bool enc_prev_state = 0;    //Переменная для хранения предыдущего сосояния энкодера(Внутренняя переменная для правильной работы в режиме регулярного опроса энкодера)

    unsigned long encoder_prev_sig = 0; //Переменная для хранения времени прошлого срабатывания энкодера

    //Целевая частота вызова сервисной задачи энкодера (Гц)
    int target_freq_enc = 600;

    //Целевая частота вызова сервисной задачи мотора (Гц)
    int target_freq_mot = 60;

    //Контроллер PID отвечающий за скорость моторов
    PIDImpl motor_pid = PIDImpl(1.0/target_freq_mot, 50, -50, 0.09, 0.01, 0.03);

    int motor_power = 0; //Значение текущего шим на моторе (0-255)

    bool motor_direction = 0;

    //Счетчик импульсов с энкодера (сколько всего накрутило колесо c момента включения машинки)
    int odometer = 0;

    //Счетчик расстояния (увеличивается по мере движения вперед и уменьшается при движении в обратном направлении)
    int position_estimate = 0;

    //Количество импульсов на оборот колеса (для расчета скорости и расстояния)
    int step_per_rot = 24;

    //Расстояние на оборот колеса (для расчета скорости и расстояния)
    const float dist_per_rot = 0.2;

    //Целевая частота срабатывания энкодера (тик/сек)
    float target_ticks = .0;

    //Текущая частота срабатывания энкодера (тик/сек) не фильтрованное значение
    float current_ticks = 0;

    //Медианный фильтр для фильтрации шумов работы энкодера
    MEDIAN_FILTER_NEW(encoder_filter, float);   //Инициализация структуры фильтра
    MEDIAN_FILTER_BUFFER_NEW(encoder_filter_buffer, 16, float);   //Инициализация структуры буффера фильтра


    #ifdef OMEGA_DEBUG

    //Граничная частота страбатывания сервисной задачи (Гц) Если сервисную задачу вызывают реже, будет засчитана ошибка
    int warnint_service_freq = 90;

    //Счетчик ошибок по частоте вызова сервисной функции
    int freq_warnings_ammount = 0;

    #endif

    void check_encoder();   //Функция проверки энкодера

    void calculate_motor(); //Функциия расчета и усатновки шип на моторы.

    unitask encoder_task;//Задача для энкодера check_encoder
    unitask motor_task;   //Задача для мотора calculate_motor


public:
    
    enum _pair_type_{
        PAIR_TYPE_LEFT,
        PAIR_TYPE_RIGHT
    };

    /***
     * @brief Инициализация колесной пары
     * @param type Какая колесная пара PAIR_TYPE_(LEFT/RIGHT)
     */
    omegabot_motor(_pair_type_ type);

    /**
     * @brief Сервисная задача, должна регулярно вызываться для опроса энкодера в случае если не используется прерывание
     */
    void service_task();

    /**
     * @brief Функция для получения данных о "сырых" скоростях вращения мотора(очень шумное значение, на него сложно ориентироваться)
     * @return float значение частоты срабатывания энкодера
     */
    float getRawTicks();    //Не рекомендуется использовать

    /**
     * @brief Функция для получения данных о скоростях вращения мотора(значение после фильтрации)
     * @return float значение частоты срабатывания энкодера
     */
    float getTicks();

    /**
     * @brief Функция для получения данных о текущем шим на моторах
     * @return byte текущий шим на моторах
     */
    byte getPower();

    void setTickSpeed(float speed);

    //Функции для корекции PID коэффицентов
    void setPID_P(float val);
    void setPID_I(float val);
    void setPID_D(float val);


    int getTickDistanceODO();  //Получить данные одометра (тики)
    int getTickDistanceTRIP();  //Получить данные трипа (тики)
    void clearDistance();  //Очистить трип  (тики)

    float getDistanceODO();  //Получить данные одометра (м)
    float getDistanceTRIP();  //Получить данные трипа (м)
};