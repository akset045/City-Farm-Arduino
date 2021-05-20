#include <SoftwareSerial.h>
#include <Wire.h>
#include "DHT.h"
#include "MHZ.h"   
#include "RTC.h"
#include <SPI.h>
#include <Ethernet.h>

// С Web-Server
 
#define CO2_IN 9
 
#define DHT22_PIN 8
#define DHTTYPE DHT22
 
#define WB_1_PIN 10
#define WB_2_PIN 11
 
#define SVET_PIN_1 5
#define SVET_PIN_2 6
#define SVET_PIN_3 7
 
#define POMP_PIN 4
 
#define A HIGH
#define B LOW
 
MHZ co2(CO2_IN, MHZ19B);
DHT dht(DHT22_PIN, DHTTYPE);
static DS3231 RTC;
 
String a;
int b;
char c;
char d;
 
const String waitg = "Done. I'm listening  MY LORD!";
const String N1 = "Уровень ";
const String N2 = "Температура ";
const String N3 = " больше ";
const String N4 = " меньше ";
const String N5 = "положеного – ";
const String N6 = "влажности";
const String N7 = "CO2";
const String N8 = "%";

byte mac[] = 
{
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(172,22,0,10);

EthernetServer server(80);

/* 
void waits()
{
  Serial.print("\n");
  Serial.println(waitg);
}
*/
 
void CO2show()
{
  Serial.println("CO2: " + String(co2.readCO2PWM()));
}
 
void Timeshow()
{
  Serial.println(
  "Date: " +
  String(RTC.getDay()) + "." + 
  String(RTC.getMonth()) + "." + 
  String(RTC.getYear())  + "\n" + "Time: " + 
  String(RTC.getHours()) + ":" + 
  String(RTC.getMinutes()) + ":" + 
  String(RTC.getSeconds())
  );
}
 
void Humishow()
{
  Serial.println(
  "Владность: " +
  String(dht.readHumidity()) + N8
  );
}
 
void Tempshow()
{
  Serial.println("Температура: " + 
  String(dht.readTemperature()) + "°C"
  );
}
 
void Vodashow()
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
 
void SvetAll_On()
{
  digitalWrite(SVET_PIN_1, A);
  digitalWrite(SVET_PIN_2, A);
  digitalWrite(SVET_PIN_3, A);
}
 
void Svet1_On()
{
  digitalWrite(SVET_PIN_1, A);
}
 
void Svet2_On()
{
  digitalWrite(SVET_PIN_2, A);
}
 
void Svet3_On()
{
  digitalWrite(SVET_PIN_3, A);
}
 
void SvetAll_Off()
{
  digitalWrite(SVET_PIN_1, B);
  digitalWrite(SVET_PIN_2, B);
  digitalWrite(SVET_PIN_3, B);
}
 
void Svet1_Off()
{
  digitalWrite(SVET_PIN_1, B);
}
 
void Svet2_Off()
{
  digitalWrite(SVET_PIN_2, B);
}
 
void Svet3_Off()
{
  digitalWrite(SVET_PIN_3, B);
}
 
void Allshow()
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
 
void PompOn()
{
  digitalWrite (POMP_PIN, A);
}
 
void PompOff()
{
  digitalWrite (POMP_PIN, B);
}
 
/*  
void user_commans()
{
  while (Serial.available() > 0)
  {
    a = Serial.readString();
    if (a == "1" || a == "All")
    {
      Serial.print("\n");
      Allshow();
      waits();
    }
 
    else if (a == "2" || a == "Time" || a == "t")
    {
      Serial.print("\n");
      Timeshow();
      delay(1000);
      waits();
    }
 
    else if (a == "3" || a == "CO2") 
    {
      Serial.print("\n");
      CO2show();
      delay(1000);
      waits();
    }
 
    else if (a == "4" || a == "Humi")
    {
      Serial.print("\n");
      Humishow();
      delay(1000);
      waits();
    }
    else if (a == "5" || a == "Temp")
    {
      Serial.print("\n");
      Tempshow();
      delay(1000);
      waits();
    }
 
    else if (a == "6" || a == "Voda")
    {
      Serial.print("\n");
      Vodashow();
      delay(1000);
      waits();
    }
 
    else if (a == "7+" || a == "SvetOn")
    {
      SvetAll_On();
      Serial.println("Свет вкл");
      waits();
    }
 
    else if (a == "7-" || a == "SvetOff")
    {
      SvetAll_Off();
      Serial.println("Свет выкл");
      waits();
    }
 
    else if (a == "71+" || a == "SvetOn 1")
    {
      Svet1_On();
      Serial.println("Свет 1 вкл");
      waits();
    }
 
    else if (a == "72+" || a == "SvetOn 2")
    {
      Svet2_On();
      Serial.println("Свет 2 вкл");
      waits();
    }
 
    else if (a == "73+" || a == "SvetOn 3")
    {
      Svet3_On();
      Serial.println("Свет 3 вкл");
      waits();
    }
 
    else if (a == "71-" || a == "SvetOff 1")
    {
      Svet1_Off();
      Serial.println("Свет 1 выкл");
      waits();
    }
 
    else if (a == "72-" || a == "SvetOff 2")
    {
      Svet2_Off();
      Serial.println("Свет 2 выкл");
      waits();
    }
 
    else if (a == "73-" || a == "SvetOff 3")
    {
      Svet3_Off();
      Serial.println("Свет 3 выкл");
      waits();
    }
 
    else if (a == "ИзВр" || a == "newt")
    {
        
      Serial.println();
      Serial.print("Старое время: ");
      Timeshow();
      Serial.println();
      Serial.println("Укажите новое: ");
      Serial.print("1: ");
      c = Serial.read();
      delay(1000);
      Serial.print("2: ");
      d = Serial.read();
      RTC.setDateTime(c, d);
      // RTC.setDateTime(c*, d*);
      // RTC.setDateTime(__DATE__, __TIME__);
      // RTC.setDateTime(*__DATE__, *__TIME__);

      Timeshow();              
      waits();
    }
 
    else if (a == "ПоРж" || a == "wm")
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
 
    else if (a == "mAut")
    {
      b = 0;
      Serial.println("Активирован автороконтоль");
      waits();
    }
 
    else if (a == "mMan")
    {   
      b = 1;
      Serial.println("Активирован ручной режим");
      waits();
    }
    
    else if (a == "PomU")
    {
      PompOn();
      Serial.println("Носос вкл");
      waits();
    } 
 
    else if (a == "PomD")
    {
      PompOff();
      Serial.println("Носос выкл");
      waits();
    }
 
    else
    {
      Serial.println("\n");
      Serial.print("I don't know this command, MY LORD!");
      Serial.println("\n");
    }
  
  }
}

*/
 
void Light_Time() // Автоматические вырубает свет
{
  if (String(RTC.getHours()) > "06" && String(RTC.getHours()) < "18")
  {
    SvetAll_On();
  }

  else if (String(RTC.getHours()) > "18" && String(RTC.getHours()) <"06")
  {
    SvetAll_Off();
  }
}
 
void Pomp_In() // Автополив
{ 
  if 
  (
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
 
void style_machine()
{
  if (b == 0)
  {
    Light_Time();
    Pomp_In();
  }
}
 
void check()
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


void clientA()

{

  EthernetClient client = server.available();

  if (client)
  
  {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) 
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close"); 
          client.println("Refresh: 5");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) 
          {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }

        if (c == '\n') 
        {
          currentLineIsBlank = true;
        } else if (c != '\r') 
        
        {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client disconnected");
  }

}
 
// БАЗОВЫЕ БЛОКИ 
 
void setup() 
{
  Serial.begin(9600);
  pinMode(CO2_IN, INPUT);
  RTC.begin();
  dht.begin();
 
  pinMode(SVET_PIN_1, OUTPUT);
  pinMode(SVET_PIN_2, OUTPUT);
  pinMode(SVET_PIN_3, OUTPUT); 
  pinMode (10, INPUT);
  pinMode (11, INPUT);
 
  digitalWrite(SVET_PIN_1, LOW);
  digitalWrite(SVET_PIN_2, LOW);
  digitalWrite(SVET_PIN_3, LOW);
  b = 0;

  Serial.begin(9600);
  while (!Serial) 
  {
    ; 
  }

  Serial.println("Ethernet WebServer Example");

  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) 
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) 
    {
      delay(1);
    }
  }

  if (Ethernet.linkStatus() == LinkOFF) 
  {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
 
  // Serial.println("YES, MY LORD!");
}
 
void loop() 
{
  // user_commans();
  delay(3000);
  style_machine();
  // check();
  delay(2000);
}


