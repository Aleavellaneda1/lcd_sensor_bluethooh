//#include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);  //

#include <dht11.h>
#include <SoftwareSerial.h>
//#include <string>

#define DHT11PIN A3 //Pin data del sensor
#define LEDPIN 13//Led
#define PINLED_AZUL 2 //Led azul
#define PINLED_VERDE 3 //Led verde
#define PINLED_AMARILLO 4 //Led amarillo (alerta)
#define PINLED_NARANJA 5//Led naranja 
#define PINLED_ROJO 6 //Led rojo
#define PIN_LAMP 8//Lámpara
#define PIN_FAN 9//Ventilador
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD
#define VELOCIDAD 300 // Velocidad a la que se mueve el texto

dht11 DHT11; //Sensor de temperatura y humedad
//LiquidCrystal lcd(12,11,5,4,3,2); //defino lo pines
SoftwareSerial BT(0, 1); //TX, RX  pins of arduino respetively

// Variables
String textoSuperior = "Bienvenidos al Taller de Sistemas - 2019";
String textoInferior = "IOT - 2019";
String command;
float ultimaTemperatura;
float ultimaHumedad;

void setup() {
  // put your setup code here, to run once:
    // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  lcd.setCursor(0, 0);
  //lcd.begin(COLS,ROWS);  //definiciiones del display, 16 columnas por 2 filas
//  lcd.setCursor(0,0);
  Serial.begin(9600); //Serial
  BT.begin(9600); //Bluetooth
  pinMode(PIN_LAMP, OUTPUT);
  pinMode(PIN_FAN,OUTPUT); 
  pinMode(LEDPIN,OUTPUT);
}

void loop() {
  int chk = DHT11.read(DHT11PIN); //lee el pin y en chk (check) sirve para comprobar si hay errores
  int textoLongitud=textoSuperior.length(); // Obtenemos el tamaño del texto

  // Mostramos salida del texto por la izquierda
  for(int i=1; i<=textoLongitud ; i++)
  {
    String texto = textoSuperior.substring(i-1);

    // Limpiamos pantalla
    lcd.clear();

    //Situamos el cursor
    lcd.setCursor(0, 0);

    // Escribimos el texto
    lcd.print(texto);

    //Situamos el cursor
    lcd.setCursor(0, 1);

    // Escribimos el texto
    textoInferior = "Temp:" + String(DHT11.temperature)+ " Hum:" + String(DHT11.humidity) + "%";

    //Serial.print("Humidity (%): ");
    //Serial.println((float)DHT11.humidity, 2);
    //Serial.print("Temperature (C): ");
    //Serial.println((float)DHT11.temperature, 2);
    if ((DHT11.temperature != ultimaTemperatura) || (DHT11.humidity != ultimaHumedad))
    {
      Serial.print("Temperature (C): ");
      Serial.println((float)DHT11.temperature, 2);
      Serial.print("Humidity (%): ");
      Serial.println((float)DHT11.humidity, 2);

      ultimaTemperatura=DHT11.temperature;
      ultimaHumedad=DHT11.humidity;
    }

    lcd.print(textoInferior); //Imprimi en el lcd
   
    // Esperamos
    delay(VELOCIDAD);
   nivelAlertas(ultimaTemperatura,ultimaHumedad);
    /*Inicio bluetooth*/
    
    while (BT.available()){  //Check if there is an available byte to read
      delay(10); //Delay added to make thing stable
      char c = BT.read(); //Conduct a serial read
      command += c; //build the string.
    }
    if (command.length() > 0) {
      Serial.println(command);
      //Serial.println(command.length());
      //Serial.println(command.substring(0,command.length()-2));
      //if(command == "led on") //this command will be given as an input to switch on light1
      if (command.substring(0,command.length()-2) == "led on") //this command will be given as an input to switch on light1
      //if (command.length() > 0)
      {
        digitalWrite(LEDPIN, HIGH);
        //delay(1000);
      }
      else if (command.substring(0,command.length()-2) == "led off") //this command will be given as an input to switch off light1 simillarly other commands work
      {
        digitalWrite(LEDPIN, LOW);
      }
      else if (command.substring(0,command.length()-2) == "lamp on")
      {
        digitalWrite (PIN_LAMP, HIGH);
      }
      else if (command.substring(0,command.length()-2) == "lamp off")
      {
        digitalWrite (PIN_LAMP, LOW);
      }
      else if (command.substring(0,command.length()-2) == "fan on")
      {
        digitalWrite (PIN_FAN, HIGH);
      }
      else if (command.substring(0,command.length()-2) == "fan off")
      {
        digitalWrite (PIN_FAN, LOW);
      }
      else if (command.substring(0,command.length()-2) == "all on")//using this command you can switch on all devices
      {
        digitalWrite (PIN_LAMP, HIGH);
        digitalWrite (PIN_FAN, HIGH);
        digitalWrite (LEDPIN, HIGH);
      }
      else if (command.substring(0,command.length()-2) == "all off")//using this command you can switch off all devices
      {
        digitalWrite (PIN_LAMP, LOW);
        digitalWrite (PIN_FAN, LOW);
        digitalWrite (LEDPIN, LOW);
      }
         
      command=""; //Reset the variable  
    }
    /*Fin bluetooth*/
  }
}


void nivelAlertas(int temperatura,int humedad) {
  if(temperatura < 15) {
    digitalWrite(PINLED_AZUL, HIGH);
    digitalWrite(PINLED_VERDE, LOW);
    digitalWrite(PINLED_AMARILLO, LOW);
    digitalWrite(PINLED_NARANJA, LOW);
    digitalWrite(PINLED_ROJO, LOW);
  }
  else if(temperatura >= 15 && temperatura <27){
    digitalWrite(PINLED_AZUL,LOW);
    digitalWrite(PINLED_VERDE, HIGH);
    digitalWrite(PINLED_AMARILLO, LOW);
    digitalWrite(PINLED_NARANJA, LOW);
    digitalWrite(PINLED_ROJO, LOW);
  }
  else if(temperatura >= 27 && temperatura <30){
    digitalWrite(PINLED_AZUL,LOW);
    digitalWrite(PINLED_VERDE, LOW);
    digitalWrite(PINLED_AMARILLO, HIGH);
    digitalWrite(PINLED_NARANJA, LOW);
    digitalWrite(PINLED_ROJO, LOW);
}
else if(temperatura >= 30 && temperatura <35){
    digitalWrite(PINLED_AZUL,LOW);
    digitalWrite(PINLED_VERDE, LOW);
    digitalWrite(PINLED_AMARILLO, LOW);
    digitalWrite(PINLED_NARANJA, HIGH);
    digitalWrite(PINLED_ROJO, LOW);
}
else if(temperatura >= 35){
    digitalWrite(PINLED_AZUL,LOW);
    digitalWrite(PINLED_VERDE, LOW);
    digitalWrite(PINLED_AMARILLO, LOW);
    digitalWrite(PINLED_NARANJA, LOW);
    digitalWrite(PINLED_ROJO, HIGH);
}
else {
    digitalWrite(PINLED_AZUL,LOW);
    digitalWrite(PINLED_VERDE, LOW);
    digitalWrite(PINLED_AMARILLO, LOW);
    digitalWrite(PINLED_NARANJA, LOW);
    digitalWrite(PINLED_ROJO, LOW);
}
}
