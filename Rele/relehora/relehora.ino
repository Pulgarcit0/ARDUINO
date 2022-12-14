
// Date and time functions using rtclib
#include "RTClib.h"

// Librería para la comunicación I2C 
#include <Wire.h>

 
// Declaramos un RTC DS3231
RTC_DS3231 rtc;
 
void setup () {
   pinMode(13,OUTPUT);

 Serial.begin(9600);
 
 delay(3000);
 
 // Comprobamos si tenemos el RTC conectado
 if (! rtc.begin()) {
 Serial.println("No hay un módulo RTC");
 while (1);
 }
 
 // Ponemos en hora, solo la primera vez, luego comentar y volver a cargar.
 
 //rtc.adjust(DateTime(2022, 2, 26, 8, 53 , 4 ));
}
 
void loop () {


  char fecha[19]; //Matriz de caracteres 
  DateTime now = rtc.now(); //Obtener fecha y hora actual.

  int dia = now.day();
  int mes = now.month();
  int anio = now.year();
  int hora = now.hour();
  int minuto = now.minute();
  int segundo = now.second();

  sprintf( fecha, "%.2d.%.2d.%.4d %.2d:%.2d:%.2d", dia, mes, anio, hora, minuto, segundo);
     String msg = " ";
    Serial.print(fecha + msg);
// programamos el horario de encendido
   digitalWrite(13,LOW);

 if(now.hour() == 23 && now.minute() == 42){
  digitalWrite(13,HIGH);
 }
 // programamos el horario de apagado
 else if(now.hour() == 23 && now.minute() == 43){
  digitalWrite(13,LOW);
 }
 delay(3000);
}
