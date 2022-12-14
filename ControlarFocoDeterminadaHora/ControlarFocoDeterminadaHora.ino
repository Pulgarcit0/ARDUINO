// Prueba de Reloj Tiempo Real (RTC) y Modulo I2C para LCD 2x16 By: http://dinastiatecnologica.com
// Conexion i2C :Arduino UNO  SCL-> A5   SDA->A4  VCC->5V  GND->GND
// Conexion RTC :Arduino UNO    GND->GND VCC->5V SCL-> SCL  SDA->SDA los dos pines despues del ...12,13,GND,AREF,SDA,SCL
// NOTA: se debe cargar dos veces este programa 1.Con la linea 9= RTC.adjust(DateTime(__DATE__, __TIME__));
//                                              2. Sin esa linea

#include <Wire.h> 
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS1307 RTC;
bool evento_inicio = true;	// variable de control para inicio de evento con valor true
bool evento_fin = true;		// variable de control para finalizacion de evento con valor true

# define RELE 3	  
void setup () {
  pinMode(RELE, OUTPUT);		// pin 3 como salida
  digitalWrite(RELE, LOW);
  if (! RTC.begin()) {				// si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");	// muestra mensaje de error
 while(1);
  }
Wire.begin(); // Inicia el puerto I2C
RTC.begin(); // Inicia la comunicaci¢n con el RTC
//RTC.adjust(DateTime(__DATE__, __TIME__)); // Establece la fecha y hora (Comentar una vez establecida la hora)
//el anterior se usa solo en la configuracion inicial
Serial.begin(9600); // Establece la velocidad de datos del puerto serie
 lcd.init();
 lcd.backlight();
  lcd.clear();
}
void loop(){
DateTime now = RTC.now(); // Obtiene la fecha y hora del RTC
   
Serial.print(now.year(), DEC); // A§o
Serial.print('/');
Serial.print(now.month(), DEC); // Mes
Serial.print('/');
Serial.print(now.day(), DEC); // Dia
Serial.print(' ');
Serial.print(now.hour(), DEC); // Horas
Serial.print(':');

Serial.print(now.minute(), DEC); // Minutos
Serial.print(':');
Serial.print(now.second(), DEC); // Segundos
Serial.println();
lcd.setCursor(0,0);
lcd.print("D:");
lcd.print(now.year(), DEC);
lcd.print("/");
lcd.print(now.month(), DEC);
lcd.print("/");
lcd.print(now.day(), DEC);
lcd.print(" ");
lcd.setCursor(0,1);
lcd.print("T: ");
lcd.print(now.hour(), DEC);
lcd.print(":");
lcd.print(now.minute(), DEC);
lcd.print(":");
lcd.print(now.second(), DEC);
delay(1000); // La informaci¢n se actualiza cada 1 seg.
lcd.setCursor(0,0);
lcd.print("               ");
lcd.setCursor(0,1);
lcd.print("               ");
  if ( now.hour() == 6 && now.minute() == 1 ){	// si hora = 14 y minutos = 30
    if ( evento_inicio == true ){			// si evento_inicio es verdadero
      digitalWrite(RELE, LOW);				// activa modulo de rele con nivel alto
      Serial.println( "Rele encendido" );		// muestra texto en monitor serie
      lcd.setCursor(0,0);
      lcd.print("LUCES");
      lcd.setCursor(0,1);
      lcd.print("ENCENDIDO");
      lcd.setCursor(0,0);
      lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print("               ");

      evento_inicio = false;				// carga valor falso en variable de control
    }							// para evitar ingresar mas de una vez
  }
  if ( now.hour() == 0 && now.minute() == 41 ){	// si hora = 15 y minutos = 30
    if ( evento_fin == true ){				// si evento_fin es verdadero
      digitalWrite(RELE, HIGH);				// desactiva modulo de rele con nivel bajo
      Serial.println( "Rele apagado" );			// muestra texto en monitor serie
       lcd.setCursor(0,1);
      lcd.print("LUCES");
      lcd.setCursor(0,1);
      lcd.print("APAGADO");
      lcd.setCursor(0,0);
      lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print("               ");
      evento_fin = false;				// carga valor falso en variable de control
    }							// para evitar ingresar mas de una vez
  }


}