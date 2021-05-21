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
boolean newcom = 0;
 
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

/* 
void CO2show()
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close"); 
  client.println("Refresh: 5");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("<title>My web Server</title>");                  //название страницы
  client.print("<H1>My web Server</H1>");                        //заголовк на странице
  client.print("CO2: " + String(co2.readCO2PWM()));      //кнопка выключить
  client.println("<br />");
  client.println("</html>");
  break;
}
*/
 
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
  // CO2show();
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

        if (newcom == " ")
        {
          newcom = 0;
        }

        if (c == '1')
        {
          newcom = 1;
        }

        if (newcom == 1)
        {
          Serial.println(c);
          if (c == '1')
          {
            Allshow();
          }

          if (c == "2")
          {
            // CO2show();

          

          }
        }

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
          client.print("<title>My web Server</title>");                  //название страницы
          client.print("<H1>My web Server</H1>");                        //заголовк на странице
          client.print("<a href=\"/$1\"><button>On</button></a>");       //кнопка включить
          client.print("<a href=\"/$2\"><button>Off</button></a>");      //кнопка выключить
          client.println("<br />");
          client.print("<a " String(Vodashow()) "</a>");
          client.println("</html>");
          break;
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

  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
 
void loop() 
{
  clientA();
  delay(3000);
  style_machine();
  // check();
  delay(2000);
}


