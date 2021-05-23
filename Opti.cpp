// Неоптимальный вариант:

Serial.println("Температура больше нормы – " + String(dht.readTemperature()));
Serial.println("Уровень влажности выше нормы – " + String(dht.readHumidity()));
Serial.println("CO2 больше нормы – " + String(co2.readCO2PWM()));\




// Оптимальный вариант:

const Str0 = " ";
const Str1 = "больше";
const Str2 = "нормы";
const Str3 = "выше";
const Str4 = "–";

Serial.println("Температура" + Str0 + Str1 + Str0 + Str2 + Str0 + Str4 + Str0 + String(dht.readTemperature()));
Serial.println("Уровень влажности" + Str0 + Str3 + Str0 + Str2 + Str0 + Str4 + Str0 + String(dht.readHumidity()));
Serial.println("CO2" + Str0 + Str3 + Str0 + Str2 + Str0 + Str4 + Str0 + String(co2.readCO2PWM()));