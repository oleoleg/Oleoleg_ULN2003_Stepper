#include <Oleoleg_ULN2003_Stepper.h>
#include <TimerOne.h>
#include <IRremote.hpp> // Подключаем библиотеку для работы с инфракрасным приемником
 

// Определяем константы для направления вращения двигателя
#define FORWARD 1 // Вперед
#define REVERSE 0 // Назад
// Создаем объект Stepper для управления шаговым двигателем
Oleoleg_ULN2003_Stepper Stepper(8, 9, 10, 11); // Пины 8, 9, 10, 11 Arduino UNO подключены к входам In1, In2, In3, In4 драйвера ULN2003A

//Настраиваем ввод с пульта
#define IR_RECEIVE_PIN 6 
volatile bool irFlag = false;    // Флаг: получен новый ИК-пакет
unsigned long targetAngle = 0;   // Переменная для накопления введенного числа


// Функция setup вызывается один раз при старте программы
void setup() {

  Timer1.initialize(500000);         // Интервал 500 000 мкс (0.5 сек)
  Timer1.attachInterrupt(ReceivTask);    // Привязать функцию к прерыванию
  

Stepper.begin(); // Инициализируем драйвер двигателя
Stepper.setSpeed(3);
Serial.begin(9600);
// Настраиваем пины кнопок как входы с подтяжкой к питанию
pinMode(2, INPUT_PULLUP);
pinMode(3, INPUT_PULLUP);

// Инициализируем инфракрасный приемник на пине
IrReceiver.begin(IR_RECEIVE_PIN);
Serial.println("Система готова. Введите угол и нажмите Вперед/назад.");

}

// Функция loop вызывается в бесконечном цикле после выполнения setup
void loop() {

//1. Обработка с кнопок на плате
// Проверяем состояние первой кнопки
if (digitalRead(2) == 1) {
Serial.print("Вперед 90° от кнопки \n");
Stepper.resumeStepper(); // Разрешаем движение снова
Stepper.stepAngle(FORWARD, 90);
}

// Проверяем состояние второй кнопки
if (digitalRead(3) == 1) {
Serial.print("Назад 90° от кнопки \n");
Stepper.resumeStepper(); // Разрешаем движение снова
Stepper.stepAngle(REVERSE, 90);
}

 // 2. Обработка ввода с ИК пульта
  if (irFlag) {
    uint16_t cmd = IrReceiver.decodedIRData.command;
    int digit = -1;

    switch (cmd) {
      // Коды цифр
      case 0x16: digit = 0; break;
      case 0xC:  digit = 1; break;
      case 0x18: digit = 2; break;
      case 0x5E: digit = 3; break;
      case 0x8:  digit = 4; break;
      case 0x1C: digit = 5; break;
      case 0x5A: digit = 6; break;
      case 0x42: digit = 7; break;
      case 0x52: digit = 8; break;
      case 0x4A: digit = 9; break;

      case 0x40: // ВПЕРЕД
        if (targetAngle > 0) {
          Serial.print("\nДВИЖЕНИЕ: Вперед на "); Serial.println(targetAngle);
          Stepper.resumeStepper(); // Разрешаем движение снова
          Stepper.stepAngle(FORWARD, (float)targetAngle);
          targetAngle = 0; // Обнуляем после выполнения
        }
        break;

      case 0x44: // НАЗАД
        if (targetAngle > 0) {
          Serial.print("\nДВИЖЕНИЕ: Назад на "); Serial.println(targetAngle);
          Stepper.resumeStepper(); // Разрешаем движение снова
          Stepper.stepAngle(REVERSE, (float)targetAngle);
          targetAngle = 0; // Обнуляем после выполнения
        }
        break;

      case 0x43: // СБРОС
        targetAngle = 0;
        Serial.println("\nУгол сброшен в 0.");
        break;
    }

    // Если нажата цифра — «наращиваем» число
    if (digit != -1) {

       if (targetAngle==0) Serial.print("Текущий угол: ");
      targetAngle = (targetAngle * 10) + digit;
      //Serial.println(targetAngle);
      Serial.print(digit);
    }

    irFlag = false; 
   // IrReceiver.resume(); 
  }
}

//Проверка приёма сигнала с пульта, был ли принят инфракрасный сигнал
void ReceivTask() 
{
  if (IrReceiver.decode()   )  {
      // Если нажата кнопка Сброс  — мгновенно взводим стоп-флаг
      if (IrReceiver.decodedIRData.command == 0x43) {
        if (Stepper.isBusy) Serial.println("Операция прервана. Мотор остановлен");
           Stepper.stopFlag = true;
      }
      IrReceiver.resume();
      irFlag = true; 
  }
}
 
