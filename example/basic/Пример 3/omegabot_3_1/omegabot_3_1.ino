#define POTENTIOMETER_PIN A0 // Аналоговый ПИН, к которому подключён потенциометр

// Функция преобразования значений AnalogRead в проценты
float convert_to_percent(int raw_value) {
  // Максимальное значение AnalogRead - 1023
  return ((float)raw_value / 1023.0) * 100.0;
}

void setup() {
  Serial.begin(9600); // Инициализируем последовательный порт
  pinMode(POTENTIOMETER_PIN, INPUT); //Настраиваем пин к которому подключен потенциометр на вход.
}

void loop() {
  int raw_value = analogRead(POTENTIOMETER_PIN); // Читаем значение с потенциометра
  float percent_value = convert_to_percent(raw_value); // Преобразуем в проценты

    //Выводим данные в порт
  Serial.print("Аналоговое значение: ");
  Serial.print(raw_value);
  Serial.print("\tЗнчение в процентах: ");
  Serial.println(percent_value);

  delay(100); // Пауза перед следующим замером
}