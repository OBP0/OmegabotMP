// В данном примере используется прерывание на Watchdog таймере (WDT).
// WDT может вызывать всего одно прерывание с интервалами 16, 32, 64, 125, 250, 500, 1000, 2000, 4000, 8000 мс.
// Основной функционал WDT заключается в перезагрузке контроллера при зависаниях.
// При активации WDT Arduino может начать постоянно перезагружаться, чтобы этого избежать при узтановке делителя зануляется регистр WDTCSR (WDTCSR = 0)
// Если допустить ошибку и не выключить перезагрузку, могут возникнуть проблемы при последующей загрузке программы, поскольку контроллер будет постоянно перезагружаться.
// Исправить проблему с прошивкой возможно записав загрузчик через ICSP разъём с помощью программатора или другой Arduino.
// Если вы не уверенны в своём коде, который использует WDT, то лучше добавить в setup задержку 5 сек до настройки WDT.

#define IR_PIN A0

volatile int filtered_measur = 0;
// volatile - является указателем компилятора, покаывающая что она должна находиться в ОЗУ
// указатель volatile позволяет обращаться и изменять значение переменной в прерываниях  

ISR(WDT_vect) { // Функция прерывания, в которой будет происходить чтение АЦП и фильтрация данных
  filtered_measur += (analogRead(IR_PIN) - filtered_measur) >> 3;
}

float Read_IR(int measur){   // Функция чтения вычисления расстояния ИК дальномера из показаний АЦП
  float dist    = 29.988 * pow(measur / 204.8, -1.173); 
  return dist;
}

void setup() {
  //delay(5000); // Задержка в 5 секунд
  Serial.begin(9600);

  // инициализация WDT
  cli();                                         // Запрт прерываний
  asm("wdr");                                    // Сбрасываем WDT
  WDTCSR = (1 << WDCE) | (1 << WDE);             // Разрешить изменение значения предделителя WDT

  // Ниже приведены варианты наиболее часто используемых настроек WDT
  // Выбран режим срабатывания с частотой 16 Гц. Это значит, что проверка датчика с усреднением будет проходить 16 раз в секунду.

  //WDTCSR = 0 | (1 << WDIE );                             // Запрещаем RESET и устанавливаем период ≈16 мсек (≈62,5 Гц). 
  //WDTCSR = 0 | (1 << WDP0) | (1 << WDIE );               // Запрещаем RESET и устанавливаем период ≈32 мсек (≈31,25 Гц). 
  WDTCSR = 0 | (1 << WDP1) | (1 << WDIE );                 // Запрещаем RESET и устанавливаем период ≈64 мсек (≈16 Гц).
  //WDTCSR = 0 | (1 << WDP0) | (1 << WDP1) | (1 << WDIE ); // Запрещаем RESET и устанавливаем период ≈125 мсек (≈8 Гц).
  //WDTCSR = 0 | (1 << WDP2) | (1 << WDIE )                // Запрещаем RESET и устанавливаем период ≈250 мсек (≈4 Гц).
  //WDTCSR = 0 | (1 << WDP0) | (1 << WDP2) | (1 << WDIE ); // Запрещаем RESET и устанавливаем период ≈0,5 мсек (≈2 Гц).
  sei();                                        // Разрешение прерываний
}

void loop() {
  // Поскольку обработка датчика вынесена в прерывание, в основном цикле мы можем свободно использовать функции delay 
  delay(1000);
  float S = Read_IR(filtered_measur);
  Serial.println(S);
}