
String strVersion = "1.0";

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

int pinBuzzer         = 6;

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

void loop() {
  // put your main code here, to run repeatedly:

  estadoActual = validateSensorsStatus();

  setLedsStatus(estadoActual);

  if(estadoActual == estadoLleno){   //estadoLleno
    alarmFull();
  }
  else if(estadoActual == estadoErrorFunction){
    alarmErrorFunction();
  }

  delay(500);
}
