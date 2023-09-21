#include <stdio.h>
#include <Arduino.h>
#include "DISPLAY.h"

#define LCD_SYMBOLS 0x14
#define LCD_RS_PIN 2
#define LCD_EN_PIN 3

static void EXdisplay(int value, int mode);
static void write4bits(int value);
static long write(int value);

struct
{
  const int _row_offsets[4]={0x00,0x40,0x00 + LCD_SYMBOLS,0x40 + LCD_SYMBOLS};
  const int _data_pins[4]={4,5,6,7};
} DISPLAY1;

void ConfigDisplay(void)
{
  pinMode (LCD_RS_PIN, OUTPUT);
  pinMode (LCD_EN_PIN, OUTPUT);
  for (int i=0; i< 4; ++i)
  {
    pinMode ( DISPLAY1._data_pins[i], OUTPUT);
  } 

  delay(50);
  digitalWrite ( LCD_RS_PIN, LOW);
  digitalWrite( LCD_EN_PIN, LOW); 

  EXdisplay(0b00000011, LOW);                                     //набор функций()
  delay(40);
  EXdisplay(0b00000010, LOW);                                     //набор функций()
  EXdisplay(0b00101000, LOW); 
  delay(40);
  EXdisplay(0b00000010, LOW);                                     //набор функций()
  EXdisplay(0b00101000, LOW); 
  delay(40);
  EXdisplay(0b00000000, LOW);                                     //включение дисплея(вкл, курсор, мигающий курсор)
  EXdisplay(0b00001111, LOW); 
  delay(40);
  ClsDisplay();                                                   //очистка диплея 
  delay(40);
  EXdisplay(0b00000000, LOW);                                     //набор режимов (вывод слева на право)                                 
  EXdisplay(0b00000110, LOW); 
}

void ClsDisplay(void)                                             //функция очистки диплея
{
  write4bits(0b00000000);                                         
  write4bits(0b00000001); 
  delay(40);
}

void DisplayYX(int y, int x, char *str)                           //x,y-координаты курсора
{
  EXdisplay(128 | (y +  DISPLAY1._row_offsets[x]), LOW);
  if (str == NULL) return 0;  
  else{
    const uint8_t *buffer=(const uint8_t *)str;
    long size=strlen(str);
    while (size--) {
      if (write(*buffer++));
      else break;
    }
  }
}

long write(int value) 
{
  EXdisplay(value, HIGH);
  return value;
}

static void EXdisplay(int value, int mode)
{
	digitalWrite(LCD_RS_PIN, mode);
  write4bits(value>>4);
  write4bits(value);
}

void write4bits(int value) 
{
  for (int i = 0; i < 4; i++) {
    digitalWrite( DISPLAY1._data_pins[i], (value >> i) & 0x01);
  }

  digitalWrite( LCD_EN_PIN, LOW);
  delayMicroseconds(1);    
  digitalWrite( LCD_EN_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite( LCD_EN_PIN, LOW);
  delayMicroseconds(100); 
}