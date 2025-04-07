#include <Wire.h> // Добавление библиотеки для работы с УЗ датчиком по I2C


int UltrasonikRead() {
  
  Wire.requestFrom(56, 1);
  unsigned long startTime = millis();
  while (Wire.available() == 0) {
    if (millis() - startTime > 100) { // Таймаут 100 мс
      return -1; // Возвращаем ошибку
    }
  }
  int S = Wire.read();
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
