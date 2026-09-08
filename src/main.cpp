#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    lcd.setCursor(0, 0);
    lcd.print("Erro no sensor ");
    return;
  }

  // ALERTA: Se a temperatura passar de 30°C
  if (t > 30.0)
  {
    lcd.setCursor(0, 0);
    lcd.print("   !!ALERTA!!   ");
    lcd.setCursor(0, 1);
    lcd.print("Temp alta: ");
    lcd.print(t, 1);
    lcd.print("C ");

    // Faz a luz de fundo piscar
    lcd.noBacklight();
    delay(500);
    lcd.backlight();
    delay(500);
  }
  else
  {
    // Funcionamento Normal (Garante que a luz fique acesa)
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t, 1);
    lcd.print((char)223); // Símbolo de grau °
    lcd.print("C    ");

    lcd.setCursor(0, 1);
    lcd.print("Umid: ");
    lcd.print(h, 1);
    lcd.print(" %   ");

    delay(2000);
  }
}
