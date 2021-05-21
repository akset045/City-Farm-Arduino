#include <SoftwareSerial.h>
#include <Wire.h>
#include "DHT.h"
#include "MHZ.h"   
#include "RTC.h"

// Без Web
// ...

// Пин датчика MH-Z19B для CO2:
#define CO2_IN 9
 
// Пины датчика DHT:
#define DHT22_PIN 8
#define DHTTYPE DHT22

// Пины датчика показателей воды:
#define WB_1_PIN 10
#define WB_2_PIN 11

// Пины 3 ламп освещения:
#define SVET_PIN_1 5
#define SVET_PIN_2 6
#define SVET_PIN_3 7

// Насос воды:
#define POMP_PIN 4

// Сокращения для подачи и отключения электро-питания:
#define A HIGH
#define B LOW

MHZ co2(CO2_IN, MHZ19B); // Датчик MH-Z19B для CO2
DHT dht(DHT22_PIN, DHTTYPE); // Датчик DHT для влажности и температуры
static DS3231 RTC; // Датчик времени
 
String a; // Переменная ввода команд на COM-терминале
int b; // Переменная режима работы: авто и ручной
/*
char c;
char d;
*/
String c;
String d;
String Now;

// Набор сокращений имён:
const String waitg = "Done. I'm listening  MY LORD!";
const String N1 = "Уровень ";
const String N2 = "Температура ";
const String N3 = " больше ";
const String N4 = " меньше ";
const String N5 = "положеного – ";
const String N6 = "влажности";
const String N7 = "CO2";
const String N8 = "%";
const String D1 = "Введите время ";
const String D2 = "начало";
const String D3 = "конца";
const String D4 = " дня ЧЧ:ММ";
 
void waits() // Функция сигнализирующая выполнение команды и ожидания следующей
{
  Serial.print("\n");
  Serial.println(waitg);
}
 
 
void CO2show() // Функция вывода показателей CO2
{
  Serial.println("CO2: " + String(co2.readCO2PWM()));
}
 
void Timeshow() // Функция вывода даты и времени
{
  Serial.println(
  "Date: " +
  String(RTC.getDay()) + "." + 
  String(RTC.getMonth()) + "." + 
  String(RTC.getYear()));
  Serial.println("Time: " + 
  String(RTC.getHours()) + ":" + 
  String(RTC.getMinutes()) 
  );
}
 
void Humishow() // Функция вывода влажности
{
  Serial.println(
  "Владность: " +
  String(dht.readHumidity()) + N8
  );
}
 
void Tempshow() // Функция вывода температуры
{
  Serial.println("Температура: " + 
  String(dht.readTemperature()) + "°C"
  );
}
 
void Vodashow() // Функция вывода уровня воды
{
  if (digitalRead(WB_1_PIN) == 0) 
  {
    Serial.println("Воды есть");
  }
 
  else 
  {
    Serial.println("Воды нет");
  }
}
 
void SvetAll_On() // Функция вкл всех ламп
{
  digitalWrite(SVET_PIN_1, A);
  digitalWrite(SVET_PIN_2, A);
  digitalWrite(SVET_PIN_3, A);
}
 
void Svet1_On() // Функция вкл только первой лампы
{
  digitalWrite(SVET_PIN_1, A);
}
 
void Svet2_On() // Функция вкл только второй лампы
{
  digitalWrite(SVET_PIN_2, A);
}
 
void Svet3_On() // Функция вкл только третьей лампы
{
  digitalWrite(SVET_PIN_3, A);
}
 
void SvetAll_Off() // Функция выкл всех ламп
{
  digitalWrite(SVET_PIN_1, B);
  digitalWrite(SVET_PIN_2, B);
  digitalWrite(SVET_PIN_3, B);
}
 
void Svet1_Off() // Функция выкл только первой лампы
{
  digitalWrite(SVET_PIN_1, B);
}
 
void Svet2_Off() // Функция выкл только второй лампы
{
  digitalWrite(SVET_PIN_2, B);
}
 
void Svet3_Off() // Функция выкл только третьей лампы
{
  digitalWrite(SVET_PIN_3, B);
}
 
void Allshow() // Функция вывода всех датчиков
{
  Serial.println();
  Timeshow();
  delay(1000);
  Serial.println();
  CO2show();
  delay(1000);
  Serial.println();
  Humishow();
  delay(1000);
  Serial.println();
  Tempshow();
  delay(2000);
  Serial.println();
  Vodashow();
  delay(1000);
} 
 
void PompOn() // Функция вкл насоса
{
  digitalWrite (POMP_PIN, A);
}
 
void PompOff() // Функция выкл насоса
{
  digitalWrite (POMP_PIN, B);
}

void Vremy()
{
  Serial.println(D1 + D2 + D4);
  delay(5000);
  c = Serial.readString();
  Serial.println(String("Начало = " + c));
  Serial.println(D1 + D3 + D4);
  delay(5000);
  d = Serial.readString();
  Serial.println(String("Конец = " + d));
  delay(1000);

}

void user_commans() // Функция ожидания команд пользователя

/*
  Список команд:

  1 или All – вывести показатели всех датчиков;
  2 или Time или t – вывести дату и время;
  3 или CO2 – вывести показители CO2;
  4 или Humi – вывести показатели влажности;
  5 или Temp – вывести показатели температуры;
  6 или Voda – вывести показатели воды;
  7+ – вкл все 3 лампы;
  71+ – вкл только первую лампу;
  72+ – вкл только вторую лампу;
  73+ – вкл только третью лампу;
  7- – выкл все 3 лампы;
  71- – выкл только первую лампу;
  72- – выкл только вторую лампу;
  73- – выкл только третью лампу;
  PomU – вкл насос;
  PomD – выкл насос;
  ПоРж или wm – вывести активный режим работы;
  mAut – вкл автоконтроль;
  mMan – вкл ручной режим.

*/

{
  while (Serial.available() > 0)
  {
    a = Serial.readString();

    if (a == "1" || a == "All") // Вывод показателй всех датчиков
    {
      Serial.print("\n");
      Allshow();
      waits();
    }
 
    else if (a == "2" || a == "Time" || a == "t") // Вывод даты и времени
    {
      Serial.print("\n");
      Timeshow();
      waits();
    }
 
    else if (a == "3" || a == "CO2") // Вывод показателя CO2
    {
      Serial.print("\n");
      CO2show();
      delay(1000);
      waits();
    }
 
    else if (a == "4" || a == "Humi") // Вывод влажности
    {
      Serial.print("\n");
      Humishow();
      delay(1000);
      waits();
    }
    else if (a == "5" || a == "Temp") // Вывод температуры
    {
      Serial.print("\n");
      Tempshow();
      delay(1000);
      waits();
    }
 
    else if (a == "6" || a == "Voda") //  Вывод уровня воды
    {
      Serial.print("\n");
      Vodashow();
      delay(1000);
      waits();
    }
 
    else if (a == "7+" || a == "SvetOn") // Вкл всех 3 ламп
    {
      SvetAll_On();
      Serial.println("Свет вкл");
      waits();
    }
 
    else if (a == "7-" || a == "SvetOff") // Выкл всех 3 ламп
    {
      SvetAll_Off();
      Serial.println("Свет выкл");
      waits();
    }
 
    else if (a == "71+" || a == "SvetOn 1") // Вкл только первой лампы
    {
      Svet1_On();
      Serial.println("Свет 1 вкл");
      waits();
    }
 
    else if (a == "72+" || a == "SvetOn 2") // Вкл только второй лампы
    {
      Svet2_On();
      Serial.println("Свет 2 вкл");
      waits();
    }
 
    else if (a == "73+" || a == "SvetOn 3") // Вкл только третьей лампы
    {
      Svet3_On();
      Serial.println("Свет 3 вкл");
      waits();
    }
 
    else if (a == "71-" || a == "SvetOff 1") // Выкл только первой лампы
    {
      Svet1_Off();
      Serial.println("Свет 1 выкл");
      waits();
    }
 
    else if (a == "72-" || a == "SvetOff 2") // Выкл только второй лампы
    {
      Svet2_Off();
      Serial.println("Свет 2 выкл");
      waits();
    }
 
    else if (a == "73-" || a == "SvetOff 3") // Выкл только третьей лампы
    {
      Svet3_Off();
      Serial.println("Свет 3 выкл");
      waits();
    }
 
    else if (a == "ИзВр" || a == "newt") // Изменить время
    {
      Vremy();
      Serial.println();
      waits();
    }
 
    else if (a == "ПоРж" || a == "wm") // Вывод нынешнего режима работы
    {
      switch (b)
      {
        case (0):
        {
          Serial.println("Активен автоконтроль");
          break;
        }
        case (1):
        {
          Serial.println("Активен автоконтроль");
          break;
        }
        break;
      }
      waits();
    }
 
    else if (a == "mAut") // Вкл авторежима
    {
      b = 0;
      Serial.println("Активирован автороконтоль");
      waits();
    }
 
    else if (a == "mMan") // Вкл ручного режима
    {   
      b = 1;
      Serial.println("Активирован ручной режим");
      waits();
    }
    
    else if (a == "PomU") // Вкл насоса
    {
      PompOn();
      Serial.println("Носос вкл");
      waits();
    } 
 
    else if (a == "PomD") // Выкл насоса
    {
      PompOff();
      Serial.println("Носос выкл");
      waits();
    }

    else // Вывод при вводе неверной команды
    {
      Serial.println("\n");
      Serial.print("I don't know this command, MY LORD!");
      Serial.println("\n");
    }
  
  }
}
 
void Light_Time() // Функция автоматического вкл и выкл всех трех ламп
{
  /*
  if (String(RTC.getHours()) > "06" && String(RTC.getHours()) < "18")
  {
    SvetAll_On();
  }

  else if (String(RTC.getHours()) > "18" && String(RTC.getHours()) <"06")
  {
    SvetAll_Off();
  }
  */
 
  if (Now > c && Now < d)
  {
    SvetAll_On();
  }

}
 
void Pomp_In() // Функция автополива: автоматический вкл и выкл насоса
{ 
  if 
  (
    /*
    String(String(RTC.getHours()) + ":" + String(RTC.getMinutes())) > "14:27" && 
    String(String(RTC.getHours()) + ":" + String(RTC.getMinutes())) < "14:29"
    */
   
    String(String(RTC.getHours()) + ":" + String(RTC.getMinutes())) > "14:27" && 
    String(String(RTC.getHours()) + ":" + String(RTC.getMinutes())) < "14:29"
   
  )
 
  {
    PompOn();
  }
 
  else if 
  (
    String(String(RTC.getHours()) + ":" + String(RTC.getMinutes())) > "14:28" || 
    String(String(RTC.getHours()) + ":" + String(RTC.getMinutes())) < "14:27"
  )
 
  {
   PompOff();
  }
}
 
void style_machine() // Функция выполнения автоматического вкл и выкл ламп и насоса
{
  if (b == 0)
  {
    Now = String(String(RTC.getHours()) + ":" + String(RTC.getMinutes()));
    Light_Time();
    Pomp_In();
  }
}
 
void check() // Фунция проверки норм показателй
{
  if (String(co2.readCO2PWM()) < "0")
  {
    Serial.println(N1 + N7 + N4 + N5 + String(co2.readCO2PWM()));
  }
  
  else if (String(co2.readCO2PWM()) > "100")
  {
    Serial.println(N1 + N7 + N3 + N5 + String(co2.readCO2PWM()));
  }

  if (String(dht.readTemperature()) < "-5")
  {
    Serial.println(N2 + N4 + N5 + String(dht.readTemperature()));
  }

  else if (String(dht.readTemperature()) > "70")
  {
    Serial.println(N2 + N3 + N5 + String(dht.readTemperature()));
  }

  if (String(dht.readHumidity()) > "20")
  {
    Serial.println(N1 + N6 + " ниже " + N5 + String(dht.readHumidity()) + N8);
  }

  else if (String(dht.readHumidity()) < "80")
  {
    Serial.println(N1 + N6 + " выше " + N5 + String(dht.readHumidity()) + N8);
  } 
}
 
// БАЗОВЫЕ БЛОКИ 
 
void setup() 
{
  Serial.begin(9600); // Arduino подключилась к COM-порту
  RTC.begin();
  dht.begin();

  pinMode(CO2_IN, INPUT); // Подключение датчика MH-Z19B
  pinMode(SVET_PIN_1, OUTPUT); // Подключение перво лампы
  pinMode(SVET_PIN_2, OUTPUT); // Подключение второй лампы
  pinMode(SVET_PIN_3, OUTPUT); // Подключение третьей лампы
  pinMode (WB_1_PIN, INPUT); // Подключение первого пина датчика воды
  pinMode (WB_2_PIN, INPUT); // Подключение второго пина датчика воды
 
  digitalWrite(SVET_PIN_1, B);
  digitalWrite(SVET_PIN_2, B);
  digitalWrite(SVET_PIN_3, B);
  // Изначально все лампы вык

  c = " ";
  d = " ";
  // Vremy();
  // delay(5000);


  b = 0; // При включение программы изначально активирован авторежим
  // Serial.println(Now);
  Serial.println("YES, MY LORD!");
}
 
void loop() 
{
  user_commans(); // Ожидание ввода команд
  delay(3000);
  style_machine(); // Проверка автоматического вкл и выкл ламп и насоса
  // check(); // Проверка нормы показателей
  delay(2000);
  // Serial.println(Now);
}