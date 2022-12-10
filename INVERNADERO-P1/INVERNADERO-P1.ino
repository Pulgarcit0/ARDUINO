#include <Wire.h>			// libreria de comunicacion por I2C
#include <LCD.h>			// libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>		// libreria para LCD por I2C
#include <SPI.h> //for the SD card module
#include <SD.h> // for the SD card
#include <DHT.h> // for the DHT sensor
#include <RTClib.h> // for the RTC
#define DHTPIN 8   
#define DHTTYPE DHT11  
int sensorHumedad = A0 ; //sensor de humedad
int bomba= 2;//bomba de agua
RTC_DS1307 rtc;
#include <RTClib.h> 
DHT dht(DHTPIN, DHTTYPE);
const int chipSelect = 4; 
File myFile;
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7


int contador;  //Variable asociada al contador
int presionado1; //Variable para el antirrebote del pulsador aumentar
int presionado2; //Variable para el antirrebote del pulsador disminuir

int pulsador_disminuir = 9; //Pulsador conectado al pin 10
int pulsador_aumentar = 10; //Pulsador conectado a pin 9
//----------------------------------------------<<SETUP>>-----------------------------------------------------------
void setup() {
  pinMode(bomba, OUTPUT);
  Serial.begin(9600);
  
    lcd.setBacklightPin(3,POSITIVE);	// puerto P3 de PCF8574 como positivo
    lcd.setBacklight(HIGH);		// habilita iluminacion posterior de LCD
    lcd.begin(16, 2);			// 16 columnas por 2 lineas para LCD 1602A
    lcd.clear();	
//--------------------------------------------CONEXION_SD-FROM>>PRACTICADELABUENA-------------------------------------
  //initializing the DHT sensor
  dht.begin();

  //initializing Serial monitor
  Serial.begin(9600);
  
  // setup for the RTC
  while(!Serial); // for Leonardo/Micro/Zero
    if(! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
    else {
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    if(! rtc.isrunning()) {
      Serial.println("RTC is NOT running!");
    }
    
  // setup for the SD card
  Serial.print("Initializing SD card...");

  if(!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
    
  //open file
  myFile=SD.open("DATA.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (myFile) {
    Serial.println("File opened ok");
    // print the headings for our data
    myFile.println("Date,Time,Temperature ºC");
  }
  myFile.close();
//-------------------------------------------FINN CONEXION_SD------------------------------------------------------------

}
void RTC_display(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.clear();
  DateTime now = rtc.now();
  char _buffer[17];
  char dow_matrix[7][10] = {" DOMINGO  ", " LUNES  ", " MARTES ", "MIERCOLES",
                             "JUEVES       ", " VIERNES   ", " SABADO "};
  


  lcd.setCursor(3, 0);
  lcd.print( dow_matrix[now.dayOfTheWeek()] );
 
  // print time
lcd.setCursor(0,1);
  lcd.print("HORA: ");
  lcd.print(now.hour(),DEC);
  lcd.print(":");
  lcd.print(now.minute(),DEC);
  lcd.print(":");
  lcd.print(now.second(),DEC);
   lcd.backlight();
   delay(500);
}


void TH(){

   int t = dht.readTemperature();
  float h=dht.readHumidity();
  lcd.setCursor(0,0);
  lcd.print("TEM: ");
  lcd.print(t-4.00);
  lcd.setCursor(0,2);
  lcd.print("HUMEDAD:");
  lcd.print(h);
  lcd.backlight();
  delay(500);
}

//--------------------------------------------DATALOGGER-----------------------------------------------------------------
void loggingTime() {
  DateTime now = rtc.now();
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    myFile.print("FECHA: ");
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print("HORA: ");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(",");
  }
  
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.println(now.day(), DEC);
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  Serial.print('/');
/*
  lcd.print(now.year(),DEC);
  lcd.print("/");
  lcd.print(now.month(),DEC);
  lcd.print("/");
  lcd.print(now.day(),DEC);*/




  myFile.close();
  delay(700);  
}

//--------------------------------------------RAIN-----------------------------------------------------------------


void riego() {
  myFile = SD.open("DATA.txt", FILE_WRITE);

  
if(analogRead(sensorHumedad) <900){
  digitalWrite(bomba,HIGH);
  if (myFile) {
    myFile.print("RIEGO: ");
    myFile.print("NO ,");
  }

}else if(analogRead(sensorHumedad) > 800){
  lcd.setCursor(0,0);
  lcd.print("    ");
  digitalWrite(bomba,LOW);
  lcd.setCursor(0,0);
  lcd.print("PORFAVOR ESPERE");
  lcd.setCursor(0,1);
  lcd.print("REGANDO ");
  lcd.setCursor(8,1);
  lcd.print("...     ");
  lcd.display();
  delay(1700);
  lcd.clear();
  if (myFile) {
    myFile.print("RIEGO: ");
    myFile.print("SI ,");
  }

}
myFile.close();
  delay(700);  
}



//--------------------------------------------ENDRAIN-----------------------------------------------------------------

void loggingTemperature() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius
  int t = dht.readTemperature();
  float h=dht.readHumidity();

  // Read temperature as Fahrenheit
  //float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if  (isnan(t) /*|| isnan(f)*/) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
    //debugging purposes
  Serial.print("TEMPERATURA: "); 
  Serial.print(t-4.00);
  Serial.print(" *C ");
  Serial.print("HUMEDAD: ");
  Serial.print(h);
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(6,0);
  lcd.print("MODO");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(3,1);
  lcd.print("AUTOMATICO");
  //Serial.print(f);
  //Serial.println(" *F\t"); 
  
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("open with success");
    myFile.print("TEMPERATURA: ");
    myFile.print(t-4);
    myFile.print(", ");
    myFile.print("HUMEDAD: ");
    myFile.println(h);
  }
  myFile.close();
  delay(20);
}
//-------------------------------------------FIN DATALOGER---------------------------------------------------------------

void loop() {
  

  

  //Condicionales para antirrebote y uso del pulsador aumentar

  if (digitalRead(pulsador_aumentar) == LOW)
  {
    presionado1 = 1;   //Variable del antirrebote que cambia cuando se presiona el pulsador
  }
  if (digitalRead(pulsador_aumentar) == HIGH && presionado1 == 1)
  {
    presionado1 = 0;  //Se reinicia la variable antirrebote
    contador++;       //Aumenta el contador
    if (contador > 9)
    {
      contador = 9; //Si el contador esta en 9 y se aumenta, sigue mostrando el 9
    }
  }

  //Condicionales para antirrebote y uso del pulsador disminuir

  if (digitalRead(pulsador_disminuir) == LOW)
  {
    presionado2 = 1;    //Variable del antirrebote que cambia cuando se presiona el pulsador
  }
  if (digitalRead(pulsador_disminuir) == HIGH && presionado2 == 1)
  {
    presionado2 = 0;   //Se reinicia la variable antirrebote
    contador--;        //Disminuye el contador

    if (contador < 0) //Si el contador esta en 0 y se disminuye, sigue mostrando el 0
    {
      contador = 0;
    }
  }

  switch (contador)
  {
    
    //Dependiendo del contador, se muestra el numero indicado
    //Envia estados (1 y 0) al display dependiendo del numero a mostrar
    case 0:
    lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  INVERNADERO");
  lcd.setCursor(4,1);
  lcd.print("PICHONES");
  lcd.setCursor(0,0);
  riego();
  delay(1000);
    break;    
    case 1:

      RTC_display();
      riego();
  break;
    case 2:
    riego();
    TH();
      break;

    case 3:
    loggingTemperature();
    loggingTime();
    riego();

break;
  }
}
