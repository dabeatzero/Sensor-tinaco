#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

String strVersion = "2.1";

int pinLedVacio        = 0;
int pinLedFondo        = 1;
int pinLedUnCuarto     = 2;
int pinLedMitad        = 3;
int pinLedTresCuartos  = 4;
int pinLedLleno        = 5;

int pinSensorFondo        = 7;
int pinSensorUnCuarto     = 8;
int pinSensorMitad        = 9;
int pinSensorTresCuartos  = 10;
int pinSensorLleno        = 11;

int estadoVacio       = 0;
int estadoFondo       = 1;
int estadoUnCuarto    = 2;
int estadoMitad       = 3;
int estadoTresCuartos = 4;
int estadoLleno       = 5;

int estadoValidando   = 9;

int estadoErrorFunction = -1;

int estadoActual;

int estadoAnterior; 

int pinBuzzer         = 6;

//GND
//VCC
//SDA - A4
//SCL - A5

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);   //DIR, E, RW, RS, D4, D5, D6, D7
// 2,1,0,4,5,6,7 indican a qué puertos de convertidor a I2C se conectan los pines del lcd. Esto es P0,P1,P2,P3,P4,P5,P6,P7

byte barraVer[8] = {B10101,B10101,B10101,B10101,B10101,B10101,B10101,B10101};

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLedVacio, OUTPUT);
  pinMode(pinLedFondo, OUTPUT);
  pinMode(pinLedUnCuarto, OUTPUT);
  pinMode(pinLedMitad, OUTPUT);
  pinMode(pinLedTresCuartos, OUTPUT);
  pinMode(pinLedLleno, OUTPUT);

  digitalWrite(pinLedVacio, LOW);
  digitalWrite(pinLedFondo, LOW);
  digitalWrite(pinLedUnCuarto, LOW);
  digitalWrite(pinLedMitad, LOW);
  digitalWrite(pinLedTresCuartos, LOW);
  digitalWrite(pinLedLleno, LOW);

  pinMode(pinSensorFondo, INPUT);
  pinMode(pinSensorUnCuarto, INPUT);
  pinMode(pinSensorMitad, INPUT);
  pinMode(pinSensorTresCuartos, INPUT);
  pinMode(pinSensorLleno, INPUT);

  pinMode(pinBuzzer, OUTPUT);

  estadoActual = estadoValidando;
  estadoAnterior = estadoValidando;

  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16,2);
  lcd.clear();

  lcd.createChar(1,barraVer);

  alarmInitSystem();
  showInit();
}

int validateSensorsStatus() {
  int tempEdoFondo        = digitalRead(pinSensorFondo);
  int tempEdoUnCuarto     = digitalRead(pinSensorUnCuarto);
  int tempEdoMitad        = digitalRead(pinSensorMitad);
  int tempEdoTresCuartos  = digitalRead(pinSensorTresCuartos);
  int tempEdoLleno        = digitalRead(pinSensorLleno);

  int tempEdoSistema = estadoValidando;

  if (tempEdoFondo == LOW && tempEdoUnCuarto == LOW && tempEdoMitad == LOW && tempEdoTresCuartos == LOW && tempEdoLleno == LOW) {
    tempEdoSistema = estadoVacio;
  }
  else if (tempEdoFondo == HIGH && tempEdoUnCuarto == LOW && tempEdoMitad == LOW && tempEdoTresCuartos == LOW && tempEdoLleno == LOW) {
    tempEdoSistema = estadoFondo;
  }
  else if (tempEdoFondo == HIGH && tempEdoUnCuarto == HIGH && tempEdoMitad == LOW && tempEdoTresCuartos == LOW && tempEdoLleno == LOW) {
    tempEdoSistema = estadoUnCuarto;
  }
  else if (tempEdoFondo == HIGH && tempEdoUnCuarto == HIGH && tempEdoMitad == HIGH && tempEdoTresCuartos == LOW && tempEdoLleno == LOW) {
    tempEdoSistema = estadoMitad;
  }
  else if (tempEdoFondo == HIGH && tempEdoUnCuarto == HIGH && tempEdoMitad == HIGH && tempEdoTresCuartos == HIGH && tempEdoLleno == LOW) {
    tempEdoSistema = estadoTresCuartos;
  }
  else if (tempEdoFondo == HIGH && tempEdoUnCuarto == HIGH && tempEdoMitad == HIGH && tempEdoTresCuartos == HIGH && tempEdoLleno == HIGH) {
    tempEdoSistema = estadoLleno;
  }
  else {
    tempEdoSistema = estadoErrorFunction;
  }

  return tempEdoSistema;
}


void setLedsStatus(int estado) {

  if (estado == estadoVacio) {
    digitalWrite(pinLedVacio, HIGH);
    digitalWrite(pinLedFondo, LOW);
    digitalWrite(pinLedUnCuarto, LOW);
    digitalWrite(pinLedMitad, LOW);
    digitalWrite(pinLedTresCuartos, LOW);
    digitalWrite(pinLedLleno, LOW);
  }
  else if (estado == estadoFondo) {
    digitalWrite(pinLedVacio, LOW);
    digitalWrite(pinLedFondo, HIGH);
    digitalWrite(pinLedUnCuarto, LOW);
    digitalWrite(pinLedMitad, LOW);
    digitalWrite(pinLedTresCuartos, LOW);
    digitalWrite(pinLedLleno, LOW);
  }
  else if (estado == estadoUnCuarto) {
    digitalWrite(pinLedVacio, LOW);
    digitalWrite(pinLedFondo, HIGH);
    digitalWrite(pinLedUnCuarto, HIGH);
    digitalWrite(pinLedMitad, LOW);
    digitalWrite(pinLedTresCuartos, LOW);
    digitalWrite(pinLedLleno, LOW);
  }
  else if (estado == estadoMitad) {
    digitalWrite(pinLedVacio, LOW);
    digitalWrite(pinLedFondo, HIGH);
    digitalWrite(pinLedUnCuarto, HIGH);
    digitalWrite(pinLedMitad, HIGH);
    digitalWrite(pinLedTresCuartos, LOW);
    digitalWrite(pinLedLleno, LOW);
  }
  else if (estado == estadoTresCuartos) {
    digitalWrite(pinLedVacio, LOW);
    digitalWrite(pinLedFondo, HIGH);
    digitalWrite(pinLedUnCuarto, HIGH);
    digitalWrite(pinLedMitad, HIGH);
    digitalWrite(pinLedTresCuartos, HIGH);
    digitalWrite(pinLedLleno, LOW);
  }
  else if (estado == estadoLleno) {
    digitalWrite(pinLedVacio, LOW);
    digitalWrite(pinLedFondo, LOW);
    digitalWrite(pinLedUnCuarto, LOW);
    digitalWrite(pinLedMitad, LOW);
    digitalWrite(pinLedTresCuartos, LOW);
    digitalWrite(pinLedLleno, HIGH);
  }
  else if (estado == estadoErrorFunction) {
    digitalWrite(pinLedVacio, HIGH);
    digitalWrite(pinLedFondo, LOW);
    digitalWrite(pinLedUnCuarto, LOW);
    digitalWrite(pinLedMitad, LOW);
    digitalWrite(pinLedTresCuartos, LOW);
    digitalWrite(pinLedLleno, HIGH);
  }
}

void alarmFull(){
  for(int i = 0; i < 5; i++){
    digitalWrite(pinBuzzer, HIGH);
    delay(800);
    digitalWrite(pinBuzzer, LOW);
    delay(300);
  }

  delay(2000);
}

void alarmErrorFunction(){
  for(int i = 0; i < 5; i++){
    digitalWrite(pinBuzzer, HIGH);
    delay(200);
    digitalWrite(pinBuzzer, LOW);
    delay(300);
  }
  delay(2000);
}

void alarmInitSystem(){
  for(int i = 0; i < 3; i++){
    digitalWrite(pinBuzzer, HIGH);
    delay(400);
    digitalWrite(pinBuzzer, LOW);
    delay(50);
  }
  
}

void alarmChangedLevel(){
  digitalWrite(pinBuzzer, HIGH);
  delay(800);
  digitalWrite(pinBuzzer, LOW);
  delay(50);

  
  for(int i = 0; i < 2; i++){
    digitalWrite(pinBuzzer, HIGH);
    delay(250);
    digitalWrite(pinBuzzer, LOW);
    delay(50);
  }
  
}

void showLevelLine(int estado){
  int totalLines = 0;
  if (estado == estadoVacio) {
    
  }
  else if (estado == estadoFondo) {
    totalLines = 1;
  }
  else if (estado == estadoUnCuarto) {
    totalLines = 4;
  }
  else if (estado == estadoMitad) {
    totalLines = 8;
  }
  else if (estado == estadoTresCuartos) {
    totalLines = 12;
  }
  else if (estado == estadoLleno) {
    totalLines = 16;
  }
  else if (estado == estadoErrorFunction) {
    
  }

  if(estado == estadoVacio){
    lcd.setCursor(0,1);
    lcd.print("[            ]");
  }else if(estado == estadoErrorFunction){
    lcd.setCursor(0,1);
    lcd.print("      XXX       ");
  }else{
    for(int i=0; i<totalLines; i++){
      lcd.setCursor(i,1);
      lcd.write(byte(1));
    }
  }
}

void displayLevelStatus(int estado){
  String strMsgLine1 = "";
  String strMsgLine2 = "";
  
  if (estado == estadoVacio) {
    strMsgLine1 = "Nivel: Vacio";
  }
  else if (estado == estadoFondo) {
    strMsgLine1 = "Nivel: Fondo";
  }
  else if (estado == estadoUnCuarto) {
    strMsgLine1 = "Nivel: Un cuarto";
  }
  else if (estado == estadoMitad) {
    strMsgLine1 = "Nivel: Mitad";
  }
  else if (estado == estadoTresCuartos) {
    strMsgLine1 = "Nivel: Tres cuartos";
  }
  else if (estado == estadoLleno) {
    strMsgLine1 = "Nivel: Lleno";
  }
  else if (estado == estadoErrorFunction) {
    strMsgLine1 = "Error en sistema";
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(strMsgLine1);
  //lcd.setCursor(0,1);
  //lcd.print(strMsgLine2);
  showLevelLine(estado);
}

void showInit(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Starting system");

  for(int i=0; i<16; i++){
    lcd.setCursor(i,1);
    lcd.write(0x2E);
    delay(150);
  }

  delay(500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ready to start");
  lcd.setCursor(0,1);
  lcd.print("System ver " + strVersion);
  delay(1500);
}

void loop() {
  // put your main code here, to run repeatedly:

  estadoActual = validateSensorsStatus();

  if(estadoActual != estadoAnterior){ //Sólo cambia los mensajes en LCD cuando el estado ha cambiado, evita parpadeo
    setLedsStatus(estadoActual);
    displayLevelStatus(estadoActual);
    
    if((estadoAnterior != estadoValidando) && (estadoActual != estadoLleno) && (estadoActual != estadoErrorFunction)){
      //la alarma no se emite en estados lleno y error, porque estos estados emiten su propia alarma
      alarmChangedLevel();
    }

    if((estadoActual != estadoLleno) && (estadoActual != estadoErrorFunction)){
      delay(10000); //Evita que se disparen múltiples alarmas hasta que el sensor estabilice su estado
    }
  }

  if(estadoActual == estadoLleno){   //estadoLleno
    alarmFull();
  }
  else if(estadoActual == estadoErrorFunction){
    alarmErrorFunction();
  }

  estadoAnterior = estadoActual;

  delay(500);
} 
