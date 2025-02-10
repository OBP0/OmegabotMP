// Определение номера пина, к которому подключен светодиод
const int LED_PIN = 2; // Пин 2 выбран произвольно, вы можете использовать другой доступнй на шилде
// Также вы можете выбрать пин 13 и тогда будет загораться встроенный в шилд светодиод

const int BUTTON_PIN = 3;  // Пин для подключения кнопки

// Задержка между включением/выключением светодиода (в миллисекундах)
const unsigned long DELAY_TIME = 1000; // 1000 мс = 1 секунда

bool isBlinking = false;  // Флаг состояния мигания светодиода

void setup() {
  // Настраиваем пины
  pinMode(LED_PIN, OUTPUT);       // Светодиод как выход
  pinMode(BUTTON_PIN, INPUT);  // Кнопка как вход

  // Изначально светодиод выключен
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Читаем состояние кнопки
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState && !isBlinking) {  // Если кнопка нажата и мигание отключено
    isBlinking = true;                // Включить мигание
    delay(150); //Задержка для борьбя с дребезгом
    while (buttonState) continue; //Ждем пока кнопку отпустят
  }
  else if (buttonState && isBlinking) {  // Если кнопка нажата и мигание включено
    isBlinking = false;               // Отключить мигание
    delay(150); //Задержка для борьбя с дребезгом
    while (buttonState) continue; //Ждем пока кнопку отпустят
  }

  if (isBlinking) {
    // Мигание светодиодом
    digitalWrite(LED_PIN, HIGH);
    delay(DELAY_TIME);
    digitalWrite(LED_PIN, LOW);
    delay(DELAY_TIME);
  }
  else {
    // Светодиод выключен
    digitalWrite(LED_PIN, LOW);
  }
}