#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configura o endereço do LCD para 0x27 com 16 colunas e 2 linhas
// Nota: Se o texto não aparecer, mude o endereço para 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  // Inicializa o display LCD
  lcd.init();

  // Liga a luz de fundo (backlight)
  lcd.backlight();

  // Define o cursor na Coluna 0, Linha 0 (Primeira linha)
  lcd.setCursor(0, 0);
  lcd.print("Fer Bighi"); // <-- Substitua pelo seu nome real

  // Define o cursor na Coluna 0, Linha 1 (Segunda linha)
  lcd.setCursor(0, 1);
  lcd.print("IoT SENAI-SP");
}

void loop()
{
  // Código estático, não precisa rodar nada repetidamente no loop
}