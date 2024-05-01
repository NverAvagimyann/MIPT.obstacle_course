#include <IRremote.hpp>        
#include <Servo.h>
Servo myservo;

#define IR_RECEIVE_PIN 11        // Сигнальный вывод, подключенный к Ардуино
#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

#define GUN_PIN      2

int SPEED_MAX = 255;

int curr_speed = SPEED_MAX;
auto comand_old = 0xB54AFF00;
void setup()
{
  myservo.attach(10);
  pinMode(GUN_PIN, OUTPUT);
  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600); 
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Инициализация приемника
}
void loop() {
  if (IrReceiver.decode()) {        // // Проверяем, пришли ли данные
      auto comand = IrReceiver.decodedIRData.decodedRawData;
      
      if (comand == 0x0)comand = comand_old;
      else comand_old = comand;

      switch(comand){
          case 0xB54AFF00:
            Serial.println("#");
            break;
          case 0xBD42FF00:
            Serial.println("*");
            break;
          case 0xB946FF00:
            Serial.println("UP");
            digitalWrite(DIR_1, HIGH);
            analogWrite(SPEED_1, curr_speed);
            digitalWrite(DIR_2, LOW);
            analogWrite(SPEED_2, curr_speed);
            break;  
          case 0xEA15FF00:
            Serial.println("DOWN");
            digitalWrite(DIR_1, LOW);
            analogWrite(SPEED_1, curr_speed);
            digitalWrite(DIR_2, HIGH
            );
            analogWrite(SPEED_2, curr_speed);
            break;               
          case 0xBB44FF00:
            Serial.println("LEFT");
            digitalWrite(DIR_1, LOW);
            analogWrite(SPEED_1, curr_speed);
            digitalWrite(DIR_2, LOW);
            analogWrite(SPEED_2, curr_speed);
            break;  
          case 0xBC43FF00:
            Serial.println("RIGHT");
            digitalWrite(DIR_1, HIGH);
            analogWrite(SPEED_1, curr_speed);
            digitalWrite(DIR_2, HIGH);
            analogWrite(SPEED_2, curr_speed);
            break;  
          case 0xBF40FF00:
            Serial.println("OK");
            digitalWrite(GUN_PIN, HIGH);
            delay(500);
            digitalWrite(GUN_PIN, LOW);
            myservo.write(0);
            delay(1000);
            myservo.write(180);
            delay(1000);
            break;
          case 0xE916FF00:
            Serial.println("1");
            myservo.write(0);
            break;
          case 0xF30CFF00:
            Serial.println("4");
            myservo.write(180);
            break;
          case 0xF20DFF00:
            Serial.println("3");
            digitalWrite(DIR_2, HIGH);
            analogWrite(SPEED_2, curr_speed);
            break;
          case 0xA15EFF00:
            Serial.println("6");
            digitalWrite(DIR_2, LOW);
            analogWrite(SPEED_2, curr_speed);
            break;
        }
      IrReceiver.resume(); // Включение приема следующего значения
  }
  else {
    digitalWrite(SPEED_1, LOW);
    digitalWrite(SPEED_2, LOW);
  }
  delay(200);
}
