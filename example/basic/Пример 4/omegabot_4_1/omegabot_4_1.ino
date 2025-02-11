#define IR_PIN A0 // Определяем к какому пину подключен ИК датчик

float Read_IR(){   // Функция чтения данных с ИК дальномера
  float measur  = analogRead(IR_PIN) + 1;    // Считывание показаний АЦП
  float voltage = measur * 5 / 1023;         // Преобразование в вольты
  float dist    = 29.988 * pow(voltage, -1.173);  // Преобразование в сантиметры
  return dist;
}


void setup() {
  pinMode(IR_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  float S = Read_IR(); // Получение расстояния с датчика
  Serial.println(S);  // Вывод данных в терминал
  delay(100);         // Задежка 100 мс
  // В данном примере чтение показаний происходит без фильтрации.
}