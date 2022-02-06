#include "LedControl.h"

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(13,12,8,1);

/* we always wait a bit between updates of the display */
unsigned long delaytime=1000;

//Potenciometro
int potenciometro = A0;
float valor_potenciometro = 0;

//
int posicion = 0;

//Dibujo
int dibujo [8][54] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0},
  {0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0},
  {0,1,0,1,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0},
  {0,1,0,0,0,1,0,1,1,1,1,0,1,1,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0},
  {0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0},
  {0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,1,0,0,1,0,0, 1,0,0,0,1,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0}
};
int filas[] = {22, 23, 24, 25, 26, 27, 28, 29};
int columnas[] {46, 47, 48, 49, 50, 51, 52, 53};

void setup() {
  Serial.begin(9600);
  pinMode(potenciometro, INPUT); //Se declara la entrada del potenciometro
  // Se declaran las filas y columnas que seran salidas
  for(int i = 0; i < 8; i++){
    pinMode(filas[i], OUTPUT);
    pinMode(columnas[i], OUTPUT);
  }
  LimpiarMatriz();
  /*********************************************************/
  /*********************************************************/
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  /*********************************************************/
  /*********************************************************/
}

void loop() {
  valor_potenciometro = analogRead(potenciometro);
  valor_potenciometro = (valor_potenciometro/10) < 1 ? 2: valor_potenciometro/10;
  for(int x = 0; x < valor_potenciometro; x++){
    for(int i = 0; i < 8; i++){
      SeleccionarFila(i+1);
      for(int j = 0; j < 8; j++){
        Setear_LED_En_Fila_Activa(j+1, dibujo[i][(j+posicion)-abs((j+posicion)/54)*54]);
      }      
      delay(4);
      writeArduinoOnMatrix();
    }
  }
  posicion = posicion + 1;  
}

void SeleccionarFila(int fila){
  if(fila == 1) digitalWrite(filas[0],LOW); else digitalWrite(filas[0],HIGH);
  if(fila == 2) digitalWrite(filas[1],LOW); else digitalWrite(filas[1],HIGH);
  if(fila == 3) digitalWrite(filas[2],LOW); else digitalWrite(filas[2],HIGH);
  if(fila == 4) digitalWrite(filas[3],LOW); else digitalWrite(filas[3],HIGH);
  if(fila == 5) digitalWrite(filas[4],LOW); else digitalWrite(filas[4],HIGH);
  if(fila == 6) digitalWrite(filas[5],LOW); else digitalWrite(filas[5],HIGH);
  if(fila == 7) digitalWrite(filas[6],LOW); else digitalWrite(filas[6],HIGH);
  if(fila == 8) digitalWrite(filas[7],LOW); else digitalWrite(filas[7],HIGH);
}

void Setear_LED_En_Fila_Activa(int columna, int estado){
  if (columna == 1) digitalWrite(columnas[0], estado);
  if (columna == 2) digitalWrite(columnas[1], estado);
  if (columna == 3) digitalWrite(columnas[2], estado);
  if (columna == 4) digitalWrite(columnas[3], estado);
  if (columna == 5) digitalWrite(columnas[4], estado);
  if (columna == 6) digitalWrite(columnas[5], estado);
  if (columna == 7) digitalWrite(columnas[6], estado);
  if (columna == 8) digitalWrite(columnas[7], estado);
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

/*
 This method will display the characters for the
 word "Arduino" one after the other on the matrix. 
 (you need at least 5x7 leds to see the whole chars)
 */
void writeArduinoOnMatrix() {
  /* here is the data for the characters */
  byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte r[5]={B00111110,B00010000,B00100000,B00100000,B00010000};
  byte d[5]={B00011100,B00100010,B00100010,B00010010,B11111110};
  byte u[5]={B00111100,B00000010,B00000010,B00000100,B00111110};
  byte i[5]={B00000000,B00100010,B10111110,B00000010,B00000000};
  byte n[5]={B00111110,B00010000,B00100000,B00100000,B00011110};
  byte o[5]={B00011100,B00100010,B00100010,B00100010,B00011100};

  /* now display them one by one with a small delay */
  lc.setRow(0,0,a[0]);
  lc.setRow(0,1,a[1]);
  lc.setRow(0,2,a[2]);
  lc.setRow(0,3,a[3]);
  lc.setRow(0,4,a[4]);
  /*delay(delaytime);
  lc.setRow(0,0,r[0]);
  lc.setRow(0,1,r[1]);
  lc.setRow(0,2,r[2]);
  lc.setRow(0,3,r[3]);
  lc.setRow(0,4,r[4]);
  delay(delaytime);
  lc.setRow(0,0,d[0]);
  lc.setRow(0,1,d[1]);
  lc.setRow(0,2,d[2]);
  lc.setRow(0,3,d[3]);
  lc.setRow(0,4,d[4]);
  delay(delaytime);
  lc.setRow(0,0,u[0]);
  lc.setRow(0,1,u[1]);
  lc.setRow(0,2,u[2]);
  lc.setRow(0,3,u[3]);
  lc.setRow(0,4,u[4]);
  delay(delaytime);
  lc.setRow(0,0,i[0]);
  lc.setRow(0,1,i[1]);
  lc.setRow(0,2,i[2]);
  lc.setRow(0,3,i[3]);
  lc.setRow(0,4,i[4]);
  delay(delaytime);
  lc.setRow(0,0,n[0]);
  lc.setRow(0,1,n[1]);
  lc.setRow(0,2,n[2]);
  lc.setRow(0,3,n[3]);
  lc.setRow(0,4,n[4]);
  delay(delaytime);
  lc.setRow(0,0,o[0]);
  lc.setRow(0,1,o[1]);
  lc.setRow(0,2,o[2]);
  lc.setRow(0,3,o[3]);
  lc.setRow(0,4,o[4]);
  delay(delaytime);
  lc.setRow(0,0,0);
  lc.setRow(0,1,0);
  lc.setRow(0,2,0);
  lc.setRow(0,3,0);
  lc.setRow(0,4,0);
  delay(delaytime);*/
}

/*
  This function lights up a some Leds in a row.
 The pattern will be repeated on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(delaytime);
    lc.setRow(0,row,B10100000);
    delay(delaytime);
    lc.setRow(0,row,(byte)0);
    for(int i=0;i<row;i++) {
      delay(delaytime);
      lc.setRow(0,row,B10100000);
      delay(delaytime);
      lc.setRow(0,row,(byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(delaytime);
    lc.setColumn(0,col,B10100000);
    delay(delaytime);
    lc.setColumn(0,col,(byte)0);
    for(int i=0;i<col;i++) {
      delay(delaytime);
      lc.setColumn(0,col,B10100000);
      delay(delaytime);
      lc.setColumn(0,col,(byte)0);
    }
  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void single() {
  for(int row=0;row<8;row++) {
    for(int col=0;col<8;col++) {
      delay(delaytime);
      lc.setLed(0,row,col,true);
      delay(delaytime);
      for(int i=0;i<col;i++) {
        lc.setLed(0,row,col,false);
        delay(delaytime);
        lc.setLed(0,row,col,true);
        delay(delaytime);
      }
    }
  }
}
