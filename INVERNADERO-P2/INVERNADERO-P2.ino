#include <Keypad.h>            // Incluir la libreria Keypad
#include <Servo.h>             // Incluir la libreria Servo
#include <Wire.h>			// libreria de comunicacion por I2C
#include <LCD.h>			// libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>		// libreria para LCD por I2C
int estado=0;                  // 0=cerrado 1=abierto
const int rojo = 13;
const int verde = 12;
const int parlante = 10;
Servo servo11;                 // Crea el objeto servo11 con las caracteristicas de Servo
const byte FILAS = 4;          // define numero de filas
const byte COLUMNAS = 4;       // define numero de columnas
char keys[FILAS][COLUMNAS] = {    // define la distribucion de teclas
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinesFilas[FILAS] = {9,8,7,6};         // pines correspondientes a las filas
byte pinesColumnas[COLUMNAS] = {5,4,3,2};  // pines correspondientes a las columnas
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // crea objeto teclado
char TECLA;                        // almacena la tecla presionada
char CLAVE[5];                     // almacena en un array 4 digitos ingresados
char CLAVE_MAESTRA[5] = "0000";    // almacena en un array la contraseña inicial
byte INDICE = 0;                   // indice del array
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7                                   
void setup()
{
    lcd.setBacklightPin(3,POSITIVE);	// puerto P3 de PCF8574 como positivo
    lcd.setBacklight(HIGH);		// habilita iluminacion posterior de LCD
    lcd.begin(16, 2);			// 16 columnas por 2 lineas para LCD 1602A
    lcd.clear();
 servo11.attach(11,660,1400);        // Asocia el servo1 al pin 11, define el min y max del ancho del pulso 
 servo11.write(150);                // Gira el servo a 150 grados Cierra la puerta
 limpia();
 Serial.begin(9600);
pinMode(rojo, OUTPUT);
pinMode(verde, OUTPUT);
}

void loop(){
  TECLA = teclado.getKey();   // obtiene tecla presionada y asigna a variable
  if (TECLA)                  // comprueba que se haya presionado una tecla
  {
    CLAVE[INDICE] = TECLA;    // almacena en array la tecla presionada
    INDICE++;                 // incrementa indice en uno
    lcd.print(TECLA);         // envia al LCD la tecla presionada
  }

  if(INDICE == 4)             // si ya se almacenaron los 4 digitos
  {
    if(!strcmp(CLAVE, CLAVE_MAESTRA))    // compara clave ingresada con clave maestra
        abierto();              
    else 
        error();
    
    INDICE = 0;
  }
  if(estado==1 && (analogRead(A3)==0))     // si esta abierta y se pulsa boton de Nueva Clave
         nueva_clave();   


int distancia = 0.009 * readUltrasonicDistance(A1, A0);
digitalWrite(verde, HIGH);
digitalWrite(rojo, LOW);
//digitalWrite(motor, LOW);
if(distancia<=10)
{
distancia=10;
digitalWrite(verde, HIGH);
digitalWrite(rojo, LOW);
//digitalWrite(motor, LOW);
}
if(distancia>=100)
{
distancia=100;
digitalWrite(verde, LOW);
digitalWrite(rojo, HIGH);
//digitalWrite(motor, HIGH);
tone(parlante,200,200);
delay(1000);
}
int porcentaje= map(distancia,10,100,100,0);
Serial.println(distancia);
}
long readUltrasonicDistance(int triggerPin, int echoPin)
{
pinMode(triggerPin, OUTPUT);
digitalWrite(triggerPin, LOW);
delayMicroseconds(2);
digitalWrite(triggerPin, HIGH);
delayMicroseconds(10);
digitalWrite(triggerPin, LOW);
pinMode(echoPin, INPUT);
return pulseIn(echoPin, HIGH);
}

///////////////////// Error //////////////////////////////
void error(){   
      lcd.setCursor(0,1);
      lcd.print("ERROR DE CLAVE    "); 
      limpia();
      tone(parlante,200,200);
}
/////////////////////abierto o cerrado //////////////////////////////
void abierto(){ 
  int i; 
   if(estado==0){ 
      estado=1;
      /*lcd.setCursor(0,1);
      lcd.print("Abierto            ");  // imprime en el LCD que esta abierta
      */
  for(i=0;i<180;i++)
  {
   servo11.write(i);
   delay (5);             //VELOCIDAD DEL SERVOMOTOR 
  }
   delay (1500);          //TIEMPO DE REPOSO DEL SERVOMOTOR


  
   }
   

   else{
    estado=0;
    lcd.setCursor(0,1);
    lcd.print("Cerrado              ");  // imprime en el LCD que esta cerrada
     for(i=180;i>0;i--)
   {
   servo11.write(i);
   delay (5);           //VELOCIDAD DEL SERVOMOTOR 
   }
   delay (1500);        //TIEMPO DE REPOSO DEL SERVOMOTOR 
   } 
 limpia();
}

/////////////////////Nueva_Clave //////////////////////////////
void nueva_clave(){  
  lcd.setCursor(0,0);
  lcd.print("NUEVA CLAVE:        ");
  lcd.setCursor(12,0);
  INDICE=0;
  while (INDICE<=3) {
   TECLA = teclado.getKey();   // obtiene tecla presionada y asigna a variable TECLA
   if (TECLA)                 // comprueba que se haya presionado una tecla
    {
      CLAVE_MAESTRA[INDICE] = TECLA;    // almacena en array la tecla presionada
      CLAVE[INDICE] = TECLA;
      INDICE++;                 // incrementa indice en uno
      lcd.print(TECLA);         // envia a monitor serial la tecla presionada
    }   
  }
 estado=0;
  lcd.setCursor(0,1);
  lcd.print("CLAVE CAMBIADA");
  delay(2000);
   
  limpia();
}

///////////////////// limpia //////////////////////////////
void limpia(){  
 lcd.setCursor(0,0);
 lcd.print("CLAVE :          ");
 lcd.setCursor(9,0);
}

