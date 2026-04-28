#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RS485 pins
#define RE 8
#define DE 7

// Relay pin
#define RELAY 9

// Soil sensor request frame
const byte soilSensorRequest[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
byte soilSensorResponse[9];

SoftwareSerial mod(2, 3); // RX, TX

float Moisture_Percent;
float Temperature_Celsius;

void setup()
{
  Serial.begin(9600);
  mod.begin(4800);

  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH); // Relay OFF

  // LCD Start
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Soil Monitor");
  delay(2000);
  lcd.clear();
}

void loop()
{
  // RS485 transmit
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  mod.write(soilSensorRequest, sizeof(soilSensorRequest));

  // RS485 receive
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(10);

  unsigned long startTime = millis();
  while (mod.available() < 9 && millis() - startTime < 1000)
  {
    delay(1);
  }

  if (mod.available() >= 9)
  {
    byte index = 0;

    while (mod.available() && index < 9)
    {
      soilSensorResponse[index] = mod.read();
      index++;
    }

    // Moisture
    int Moisture_Int = int(soilSensorResponse[3] << 8 | soilSensorResponse[4]);
    Moisture_Percent = Moisture_Int / 10.0;

    // Temperature
    int Temperature_Int = int(soilSensorResponse[5] << 8 | soilSensorResponse[6]);
    Temperature_Celsius = Temperature_Int / 10.0;

    if (Temperature_Int > 0x7FFF)
    {
      Temperature_Celsius = 0x10000 - Temperature_Int;
      Temperature_Celsius = -Temperature_Celsius / 10.0;
    }

    // Serial Monitor
    Serial.print("Moisture: ");
    Serial.print(Moisture_Percent);
    Serial.println("%");

    Serial.print("Temp: ");
    Serial.print(Temperature_Celsius);
    Serial.println("C");

    // Relay Control Logic
    if (Moisture_Percent < 30)
    {
      digitalWrite(RELAY, LOW); // Pump ON
      Serial.println("Pump ON");
    }
    else if (Moisture_Percent >= 35)
    {
      digitalWrite(RELAY, HIGH); // Pump OFF
      Serial.println("Pump OFF");
    }

    // LCD Display
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("M:");
    lcd.print(Moisture_Percent);
    lcd.print("%");

    lcd.setCursor(9,0);
    lcd.print("T:");
    lcd.print(Temperature_Celsius);
    lcd.print("C");

    lcd.setCursor(0,1);

    if (Moisture_Percent < 30)
    {
      lcd.print("Pump: ON ");
    }
    else
    {
      lcd.print("Pump: OFF");
    }

  }
  else
  {
    Serial.println("Sensor timeout");

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sensor Error");
  }

  delay(2000);
}
