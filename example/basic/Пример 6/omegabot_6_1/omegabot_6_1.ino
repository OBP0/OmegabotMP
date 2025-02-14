/***
 * Для начала определяем пины к которым подключены моторы
 */

#define MOTOR_DIR_L 7 //Пин контроля направления левого мотора
#define MOTOR_DIR_R 4 //Пин контроля направления правого мотора

#define MOTOR_POW_L 6 //Пин для контроля мощности левого мотора
#define MOTOR_POW_R 5 //Пин для контроля мощности правого мотора

//Учет команд
#define COMMAND_ACTUAL_TIME 65 //Время актуальноси команды с момента её получения
unsigned long terminate_cmd_timer = 0;  //Таймер актуальности команды
char cmd = 'S'; //Имя команды



void setup() {
  //Настройка обмена данными
  Serial.begin(9600); //Открываем последовательный порт для обмена данными c bluetooth модулем

  //Настройка двигателей
  pinMode(MOTOR_DIR_L, OUTPUT);
  pinMode(MOTOR_DIR_R, OUTPUT);
  pinMode(MOTOR_POW_L, OUTPUT);
  pinMode(MOTOR_POW_R, OUTPUT);

  
}

void loop() {
  //Получение команд из Serial
  if(Serial.available()){
    //Если в посдеовательном порту что-то есть - читаем ,байт
    char cmd_temp = Serial.read();
    switch (cmd_temp)
    {
    case 'F':
    case 'B':
    case 'L':
    case 'R':
      //Если пришла известная команда для движения
      cmd = cmd_temp;
      terminate_cmd_timer = millis(); //Сбрасываем счетчик актуальности команды
      break;
    }
  }

  switch (cmd)
  {
  case 'F':
    //Направление
    digitalWrite(MOTOR_DIR_L, HIGH);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, HIGH);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 160); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 160); //Установка скорости мотора
    break;

    case 'B':
    //Направление
    digitalWrite(MOTOR_DIR_L, LOW);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, LOW);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 160); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 160); //Установка скорости мотора
    break;

    case 'L':
    //Направление
    digitalWrite(MOTOR_DIR_L, LOW);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, HIGH);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 100); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 100); //Установка скорости мотора
    break;

    case 'R':
    //Направление
    digitalWrite(MOTOR_DIR_L, HIGH);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, LOW);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 100); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 100); //Установка скорости мотора
    break;

    case 'S':
    //Направление
    digitalWrite(MOTOR_DIR_L, LOW);  //Установка направляния мотора
    digitalWrite(MOTOR_DIR_R, LOW);  //Установка направления мотора
    //Скрость
    analogWrite(MOTOR_POW_L, 0); //Установка скорости мотора
    analogWrite(MOTOR_POW_R, 0); //Установка скорости мотора
    break;
  }

  if(terminate_cmd_timer + COMMAND_ACTUAL_TIME < millis()){ //Если слишком долго не приходило новой команды, сбрасываем её
    cmd = 'S';
  }

  
}
