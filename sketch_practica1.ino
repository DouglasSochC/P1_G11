#include "LedControl.h"

LedControl lc=LedControl(13,12,8,1);

//Para el movimiento de la cadena
int potenciometro = A0;
float valor_potenciometro = 0;
int posicion_cadena = 0;
const int cantidad_columnas = 160;
int boton_direccion = A1;
bool movimiento_hacia_abajo = true;

//Controlan el tiempo de recorrido de la cadena
long previousMillis = 0; 
unsigned long delaytime = 1000;

//Cadena
byte cadena_sin_driver [9][cantidad_columnas] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,1,1,1,1,1,1,1,0,1,1,0,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,0,1,1,1,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,0,0,1,1,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

//Salidas
int filas[] = {22, 23, 24, 25, 26, 27, 28, 29};
int columnas[] {46, 47, 48, 49, 50, 51, 52, 53};

void setup() {
  //Se declara para poder utilizar la terminar virtual
  Serial.begin(9600);
  //Se declara las entradas
  pinMode(potenciometro, INPUT);
  pinMode(boton_direccion, INPUT);
  // Se declaran las filas y columnas que seran salidas
  for(int i = 0; i < 8; i++){
    pinMode(filas[i], OUTPUT);
    pinMode(columnas[i], OUTPUT);
  }

  //Se limpia la matriz que no posee driver
  LimpiarMatriz();
  //Se configura la matriz con driver
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  
}

void loop() {
  MatrizSinDriver();
  CambiarDireccion();
}

void CambiarDireccion(){
  if(analogRead(boton_direccion) > 0){
    movimiento_hacia_abajo = true;
  }else{
    movimiento_hacia_abajo = false;
  }
}

void MatrizSinDriver(){
  valor_potenciometro = analogRead(potenciometro);
  delaytime = valor_potenciometro;
  unsigned long currentMillis = millis();
  
  //Imprime un bloque de 8x8 del array -> cadena_sin_driver
  for(int i = 0; i < 9; i++){
    SeleccionarFila(i);
    for(int j = 0; j < 8; j++){
      Setear_LED_En_Fila_Activa(j, cadena_sin_driver[i][j+posicion_cadena]);
    }
    delay(2);
  }
  
  if(currentMillis - previousMillis > delaytime) {
    previousMillis = currentMillis;
    if(movimiento_hacia_abajo == true){
      posicion_cadena = (posicion_cadena == (cantidad_columnas-1)) ? 0: posicion_cadena + 1;
    }else{
      posicion_cadena = (posicion_cadena == 0) ? cantidad_columnas-2: posicion_cadena - 1;
    }
    MatrizConDriver(posicion_cadena-1);
  }
}

void MatrizConDriver(int posicion){
  short cantidad_ciclos = posicion > 7? 8: posicion+1;
  short posicion_inicial_columna = posicion < 8? 0: posicion-7;
  short columna = 7;
  //Se utiliza para las columnas
  for(int i = 0; i < cantidad_ciclos; i++){
    //Imprime todas las filas
    for(int j = 0; j < 8; j++){
      lc.setLed(0,columna-j,(cantidad_ciclos-1)-i,cadena_sin_driver[j][posicion_inicial_columna+i]);
    }
  }
}

void SeleccionarFila(int fila){
  if(fila == 0) digitalWrite(filas[0],LOW); else digitalWrite(filas[0],HIGH);
  if(fila == 1) digitalWrite(filas[1],LOW); else digitalWrite(filas[1],HIGH);
  if(fila == 2) digitalWrite(filas[2],LOW); else digitalWrite(filas[2],HIGH);
  if(fila == 3) digitalWrite(filas[3],LOW); else digitalWrite(filas[3],HIGH);
  if(fila == 4) digitalWrite(filas[4],LOW); else digitalWrite(filas[4],HIGH);
  if(fila == 5) digitalWrite(filas[5],LOW); else digitalWrite(filas[5],HIGH);
  if(fila == 6) digitalWrite(filas[6],LOW); else digitalWrite(filas[6],HIGH);
  if(fila == 7) digitalWrite(filas[7],LOW); else digitalWrite(filas[7],HIGH);
}

void Setear_LED_En_Fila_Activa(int columna, int estado){
  if (columna == 0) digitalWrite(columnas[0], estado);
  if (columna == 1) digitalWrite(columnas[1], estado);
  if (columna == 2) digitalWrite(columnas[2], estado);
  if (columna == 3) digitalWrite(columnas[3], estado);
  if (columna == 4) digitalWrite(columnas[4], estado);
  if (columna == 5) digitalWrite(columnas[5], estado);
  if (columna == 6) digitalWrite(columnas[6], estado);
  if (columna == 7) digitalWrite(columnas[7], estado);
}

void LimpiarMatriz(){
  for (int j=0; j<8; j++){ 
    digitalWrite(filas[j], HIGH); 
    for (int k= 0; k<8 ; k++){ 
      digitalWrite(columnas[k], LOW); 
    } 
  }
}
//Serial.println(valor_potenciometro/10);
