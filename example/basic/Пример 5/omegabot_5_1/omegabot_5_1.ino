#include <Wire.h> // Добавление библиотеки для работы с УЗ датчиком по I2C


int UltrasonikRead() { // Функция считывания УЗ датчика расстояния
  Wire.requestFrom(56, 1); // Запрашиваем данные с датчика
  int S = Wire.read(); // Получаем дальность с датчика расстояния в сантиметрах
  delay(50);
  return S;
}

void InitUltrasonic() { // Открываем соединение с датчиком по I2C
  Wire.begin();         // Инициализация I2C
  Wire.setClock(4000);  // Установка тактовой частоты I2C 
}


void setup() {
  InitUltrasonic();   // Инициализация УЗ датчика расстояния 
  Serial.begin(9600); // Инициализация Serial 
}

void loop() {
  int S = UltrasonikRead(); // Чтение даннах с датчика
  Serial.println(S);        // Вывод данных в терминал
  delay(100);               // Задержка 100 мс
}
