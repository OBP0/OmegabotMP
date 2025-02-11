#define IK_PIN A0 // Определяем к какому пину подключен ИК датчик

int ReadIK(){   // Функция чтения данных с ИК дальномера
  double measur  = analogRead(IK_PIN) + 1;    // Считывание показаний АЦП
  double voltage = measur * 5 / 1023;         // Преобразование в вольты
  double dist    = 29.988 * pow(voltage, -1.173);  // Преобразование в сантиметры
  return dist;
}


void setup() {
  pinMode(IK_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  float S = ReadIK(); // Получение расстояния с датчика
  Serial.println(S);  // Вывод данных в терминал
  delay(100);         // Задежка 100 мс
  // В данном примере чтение показаний происходит без фильтрации.
}
