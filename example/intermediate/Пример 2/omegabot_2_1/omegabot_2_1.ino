#include <Arduino.h>
#include <Wire.h>


void setup() {
  Serial.begin(9600); //Запускаем посделовательный порт
  Wire.begin(); //Запускаем I2C шину
  Wire.setClock(400000);  //Устанавливаем скорость I2C

}

void loop() {
  Wire.beginTransmission(0x68); //Нчинаем общение с IMU
  Wire.write(0x00); //Передаём адрес интересуюшего нас регистра, в данном случае 0x00 - Регистр WHO_AM_I
  Wire.endTransmission(false);  //Передаем слово сенсору
  int len = Wire.requestFrom(0x68, 1);  //Запрашиваем с него данные
  if(len == 1)  //Если количество принятых байт == количеству отправленны байт, то читаем полученые данные
  {
    if(Wire.read() == 0xEA){
      Serial.println("Привет, я IMU сенсор!");  //Если сенсор отвечает как он должен, выводим сообщение в консоль
    }
  }
  
}