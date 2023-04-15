#define BLYNK_USE_DIRECT_CONNECT
#define BLYNK_PRINT DebugSerial

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); 

#include <BlynkSimpleSerialBLE.h>

//DHT sensor
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float Temperature= 0;
float Humidity= 0;

//Motors 
//Motor PINS
int M1P1= 12;
int M1P2= 13;
int M2P1= 8;
int M2P2= 9;
int EN1= 10;
int EN2= 11;

// Motor CMD
int FW_CMD;
int BW_CMD;
int RT_CMD;
int LT_CMD;
int SP_CMD;
int ST_CMD;

//Motor speed initials
int Speed=255;

//BLYNK
BlynkTimer Timer;
char auth[] = "q2osdejVmrC8GUO8D_6GRB35OeorKAdH";

//LCD
WidgetLCD lcd(V1);

void setup()
{
   // Debug console
  DebugSerial.begin(38400);
  Serial.begin(9600);
  Serial.println("Waiting for connections...");

  // Blynk will work through Serial
  // 9600 is for HC-06. For HC-05 default speed is 38400
  // Do not read or write this serial manually in your sketch
  
  Blynk.begin(Serial, auth);
  Serial.println("Connected");

  //Motor initials
  pinMode(M1P1, OUTPUT);
  pinMode(M1P2, OUTPUT);
  pinMode(M2P1, OUTPUT);
  pinMode(M2P2, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  
  //Start DHT
  dht.begin();

//  //Off all motor
//  digitalWrite(M1P1, LOW);
//  digitalWrite(M1P2, LOW);
//  digitalWrite(M2P1, LOW);
//  digitalWrite(M2P2, LOW);
//  
  //set speed to 0
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

//  //lcd screen on
//  lcd.clear();
//  lcd.print(0, 0, "Hello");
//  delay(1000);
//  lcd.clear();
//  lcd.print(0, 1, "I'm booting");
//  delay(1000);
//  lcd.clear();

  Timer.setInterval(2000L, Weather);
}

void loop()
{
  Blynk.run();
  Timer.run();

}

BLYNK_READ(V2O){
  Temperature= dht.readTemperature();
  Blynk.virtualWrite(V0, Temperature);
  Serial.println("Temperature");
  }

BLYNK_READ(V21){
  Humidity= dht.readHumidity();
  Blynk.virtualWrite(V1, Humidity);
  Serial.println("humidity");
  }

//Move forward
void Moveforward(){
  digitalWrite(M1P1, HIGH);
  digitalWrite(M1P2, LOW);
  digitalWrite(M2P1, HIGH);
  digitalWrite(M2P2, LOW);
  analogWrite(EN1, Speed);
  analogWrite(EN2, Speed);
  }


void Movebackward(){
  digitalWrite(M1P1, LOW);
  digitalWrite(M1P2, HIGH);
  digitalWrite(M2P1, LOW);
  digitalWrite(M2P2, HIGH);
  analogWrite(EN1, Speed);
  analogWrite(EN2, Speed);
  }

 void Turnright(){
  digitalWrite(M1P1, LOW);
  digitalWrite(M1P2, LOW);
  digitalWrite(M2P1, HIGH);
  digitalWrite(M2P2, LOW);
  analogWrite(EN1, Speed);
  analogWrite(EN2, Speed);
  }

 void Turnleft(){
  digitalWrite(M1P1, HIGH);
  digitalWrite(M1P2, LOW);
  digitalWrite(M2P1, LOW);
  digitalWrite(M2P2, LOW);
  analogWrite(EN1, Speed);
  analogWrite(EN2, Speed);
  }

 void Stop(){
  digitalWrite(M1P1, LOW);
  digitalWrite(M1P2, LOW);
  digitalWrite(M2P1, LOW);
  digitalWrite(M2P2, LOW);
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);
  }

 void Weather(){
  lcd.clear();
  Temperature= dht.readTemperature();
  Humidity= dht.readHumidity();
//  lcd.print(0,0,"Temp: ");
  lcd.print(0,0, Temperature);
//  lcd.print(12,0, 255);
//  lcd.print(13,0, "C");
//  lcd.print(0,1, "Humidity: ");
  lcd.print(0,0, Humidity);
//  lcd.print(15,0, "%"); 
  Serial.print("Weather printing");  
  }


 BLYNK_WRITE(V2){
  FW_CMD= param.asInt();
  if(FW_CMD){
    Moveforward();
    Serial.println("going forward");}
  else{
    Stop();}
  }

 BLYNK_WRITE(V3){
  BW_CMD= param.asInt();
  if(BW_CMD){
    Movebackward();
    Serial.println("going backward");
}
  else{
    Stop();}
  }

 BLYNK_WRITE(V4){
  RT_CMD= param.asInt();
  if(RT_CMD){
    Turnright();
    Serial.println("going right");
}
  else{
    Stop();}
  }

 BLYNK_WRITE(V5){
  LT_CMD= param.asInt();
  if(LT_CMD){
    Turnleft();
    Serial.println("going left");
}
  else{
    Stop();}
  }

 BLYNK_WRITE(V6){
  ST_CMD= param.asInt();
  if(ST_CMD){
    Stop();
    Serial.println("Stopping");
}
  }

 BLYNK_WRITE(V7){
  SP_CMD= param.asInt();
  Speed= SP_CMD;
//  Serial.println("speed changed");
  }