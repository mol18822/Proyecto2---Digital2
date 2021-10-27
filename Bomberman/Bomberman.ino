// Universidad del Valle de Guatemala
// Electrónica Digital II
// Proyecto 2 - BOMBER-TANK
// Luis Pedro Molina - 18822
// Guillermo Lam - 19267

// Librerias
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"
#include <SPI.h>
#include <SD.h>
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};  
const int chipSelect = PA_3;
File myFile;
int x = 40;
int y = 20;
int x2 = 265;
int y2 = 205;
float vel = 0;
float vel1 = 0;
float vel2 = 0;
float vel3 = 0;
float vel4 = 0;
float vel5 = 0;
float vel6 = 0;
float vel7 = 0;
int x_seg_ciclo;
int posicion_y = 0;
unsigned char char1;
unsigned char char2;
char z;
int prueba;
int UltimaPocicion=0;
int primerCero = 0;
int flag = 0;
int flag1 = 1;
int flag11 = 1;
int flag2 = 1;
int flag22 = 1;
int flagm = 1;      
#include "musica.h"         // libreria de musica mario bross
Sd2Card card;
SdVolume volume;
SdFile root;

// Función de musica mario bross
void tone_function(int mel[], int duration[], int quant);

// Musica de juego

// Melodía para inicio de juego
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0,  };

// Tempo de melodía para inicio de juego
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,   };
  
// Melodia para disparo
int melody_shoot[] = { NOTE_C4, 0, 0,};

// Tempo para melodia de disparo
int tempo2[] = {
  12,  };

// Función para música
void tone_function(int mel[], int duration[], int quant) {

  for (int thisnote = 0; thisnote < 30; thisnote++) {
    int durations = 2250/duration[thisnote];
    tone(PF_4, mel[thisnote], durations);
    int pausebetweennotes = durations * 1.30;
    delay(pausebetweennotes);
    noTone(PF_4);
    if(thisnote == (quant-1)) {
      break;
    }
  }
}

// ----------------------------------------------- Prototipos -----------------------------------------------------------------------------

void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
bool Collision(int x1, int y1, int w1, int h1, int x3, int y3, int w3, int h3);
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
char ConversionDecimal(char x, char y);
char ConversionASCII(char x);
void CargarImagen(void);
void inicio(void);
void movimiento(void);
void comparacion1(void);
void comparacion2(void);
void juego (void);
void over (void);
int ord;
float f = 0;
float f1 = 0;
float f2 = 0;
float f3 = 0;
int flags;
int flags1;
int flags2;
int flags3;
int flags4;
int flags5;
int flags6;
int flags7;
int inByte;
int inByte2;
unsigned long previousMillis = 0;  
const long interval = 42;
bool c = false;
bool c1 = false;
bool c2 = false;
bool c3 = false;
extern uint8_t MatrizFondo[];
extern uint8_t uvg[];
String text1;
String text2;

// ------------------------------------------------------ Inicialización ---------------------------------------------------------------

void setup() {
  pinMode(PUSH1, INPUT_PULLUP); 
  pinMode(PUSH2, INPUT_PULLUP); 
  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
  Serial.begin(9600);                       // Comunicación serial 1
  Serial2.begin(9600);                      // Comunicación serial 2 -> Con arduino No. 1 para controles de player1
  Serial3.begin(9600);                      // Comunicación serial 3 -> Con arduino No. 2 para controles de player2
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Start");
  LCD_Init();
  LCD_Clear(0x00);
  SPI.setModule(0);
  Serial.print("\nInitializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);     // change this to 53 on a mega

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

// ---------------------------------------------- Pantalla de inicio de juego ---------------------------------------------------

  //LCD_Print(String text, int x, int y, int fontSize, int color, int background)
  //LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
  FillRect(0, 0, 320, 240, 0xff00);
  String text1 = "BOMBER-TANK";
  String text3 = "by Guillermo Lam & Luis Pedro Molina";
  String text2 = "Press Play";
  LCD_Print(text1, 80, 90, 2, 0x0000, 0xff00);
  LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
  LCD_Print(text3, 20, 130, 1, 0x0000, 0xff00);
  delay(1000);
  LCD_Print(text2, 80, 170, 2, 0xffff, 0xff00);
  delay(1000);
  LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
  delay(1000);
  LCD_Print(text2, 80, 170, 2, 0xffff, 0xff00);
  delay(1000);
  LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
  delay(1000);
  LCD_Print(text2, 80, 170, 2, 0xffff, 0xff00);
  delay(1000);
  LCD_Print(text2, 80, 170, 2, 0x0000, 0xff00);
}

// ----------------------------------------------------------- Loop --------------------------------------------------------------------
void loop() {
  
  if (Serial2.available()) {
    inByte = Serial2.read();}
  if (Serial3.available()) {
    inByte2 = Serial3.read();}     
  inicio();
  juego();
}

// ---------------------------------------------- Función para inicializar LCD ---------------------------------------------------------
void inicio(void){
  
  if ( digitalRead(PUSH1) == LOW && flag == 0){
  tone_function(underworld_melody, tempo, 15);  
    flag = 1;
    flag1 = 0;
    flag11 = 0;
    flag2 = 0;
    flag22 = 0;
    inByte = 0;
    inByte = 0;
    flags = 0;
    flags1 = 1;
    flags2 = 1;
    flags3 = 1;
    vel = 0;
    vel1 = 0;
    vel2 = 0;
    vel3 = 0;
    flags4 = 1;
    flags5 = 0;
    flags6 = 1;
    flags7 = 1;
    vel4 = 0;
    vel5 = 0;
    vel6 = 0;
    vel7 = 0;
    LCD_Bitmap(0, 0, 320, 240, uvg);
    LCD_Sprite(x, y,17,15,tanque1_2,1, 2,0,0 );
    LCD_Sprite(x2, y2,17,15,tanque2_3,1, 2,0,0 );
    }}
void juego (void){
  unsigned long currentMillis = millis();
  
  // actualización de frame cada 42ms = 24fps
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    movimiento();
    movimiento11();
    movimiento2();
    movimiento22();
    comparacion1();
    comparacion11();
    comparacion2();
    comparacion22();
    disparo();
    disparo1();
    disparo2();
    disparo3();
    disparo4();
    disparo5();
    disparo6();
    disparo7();
    over();}}

// ----------------------------------------- Funciones "movimientox" para movimiento de tanques ----------------------------------
// Movimiento hacia arriba 
void movimiento (void){
  if (inByte == 0x32 && flag1 == 0 ){
    flags = 0;
    flags1 = 1;
    flags2 = 1;
    flags3 = 1;
    vel = 0;
    vel1 = 0;
    vel2 = 0;
    vel3 = 0;
    if( x > 30 & x < 263){
      inByte = 0;
      x +=4;
      LCD_Sprite(x, y,17,15,tanque1_2,1, 2,0,0 );
      FillRect( x-5, y, 5, 17, 0x04E8);}}
  else if (inByte == 0x33 && flag1 == 0 ){
    flags = 1;
    flags1 = 0;
    flags2 = 1;
    flags3 = 1;
    vel = 0;
    vel1 = 0;
    vel2 = 0;
    vel3 = 0;
    if( x > 40 & x < 270){
      inByte = 0;
      x -=4;
      LCD_Sprite(x, y,17,15,tanque1_3,1, 2,0,0 );
      FillRect( x+17, y, 5, 17, 0x04E8);}}}

// Movimiento hacia abajo
void movimiento11 (void){
  if (inByte2 == 0x32 && flag11 == 0 ){
    flags4 = 0;
    flags5 = 1;
    flags6 = 1;
    flags7 = 1;
    vel4 = 0;
    vel5 = 0;
    vel6 = 0;
    vel7 = 0;
    if( x2 > 30 & x2 < 263){
      inByte2 = 0;
      x2 +=4;
      LCD_Sprite(x2, y2,17,15,tanque2_2,1, 2,0,0 );
      FillRect( x2-5, y2, 5, 17, 0x04E8);}}
  else if (inByte2 == 0x33 && flag11 == 0 ){
    flags4 = 1;
    flags5 = 0;
    flags6 = 1;
    flags7 = 1;
    vel4 = 0;
    vel5 = 0;
    vel6 = 0;
    vel7 = 0;
    if( x2 > 40 & x2 < 270){
      inByte2 = 0;
      x2 -=4;
      LCD_Sprite(x2, y2,17,15,tanque2_3,1, 2,0,0 );
      FillRect( x2+17, y2, 5, 17, 0x04E8);}}}

// Movimiento hacia derecha 
void movimiento2 (void){
  if (inByte == 0x30 && flag2 == 0){
    flags = 1;
    flags1 = 1;
    flags2 = 1;
    flags3 = 0;
    vel = 0;
    vel1 = 0;
    vel2 = 0;
    vel3 = 0;
    if( y > 20 & y < 210){
      inByte = 0;
      y -=4;
      LCD_Sprite(x, y,15,17,tanque1_1,1, 2,0,0 );
      FillRect( x, y+17, 17, 5, 0x04E8);}}
  else if (inByte == 0x31 && flag2 == 0){
    flags = 1;
    flags1 = 1;
    flags2 = 0;
    flags3 = 1;
    vel = 0;
    vel1 = 0;
    vel2 = 0;
    vel3 = 0;
    if( y > 19 & y < 204){
      inByte = 0;
      y +=4;
      LCD_Sprite(x, y,15,17,tanque1_4,1, 2,0,0 );
      FillRect( x, y-5, 17, 5, 0x04E8);}}}

// Movimiento hacia la izquierda
void movimiento22 (void){
  if (inByte2 == 0x30 && flag22 == 0){
    flags4 = 1;
    flags5 = 1;
    flags6 = 1;
    flags7 = 0;
    vel4 = 0;
    vel5 = 0;
    vel6 = 0;
    vel7 = 0;
    if( y2 > 22 & y2 < 210){
      inByte2 = 0;
      y2 -=4;
      LCD_Sprite(x2, y2,15,17,tanque2_1,1, 2,0,0 );
      FillRect( x2, y2+17, 17, 5, 0x04E8);}}
  else if (inByte2 == 0x31 && flag22 == 0){
    flags4 = 1;
    flags5 = 1;
    flags6 = 0;
    flags7 = 1;
    vel4 = 0;
    vel5 = 0;
    vel6 = 0;
    vel7 = 0;
    if( y2 > 19 & y2 < 205){
      inByte2 = 0;
      y2 +=4;
      LCD_Sprite(x2, y2,15,17,tanque2_4,1, 2,0,0 );
      FillRect( x2, y2-5, 17, 5, 0x04E8);}}}

// ------------------------------------------------ Funciónes de comparación ---------------------------------------------------
// --------------------------------- Utilizadas para delimitar el périmetro del campo de juego ---------------------------------
void comparacion1 (void){
  if (x > 42 && x < 75){
    flag2 = 1;}
  else if(x > 78 && x < 115){
    flag2 = 1;}
  else if(x > 118 && x < 150){
    flag2 = 1;}
  else if(x > 153&& x < 188){
    flag2 = 1;}
  else if(x > 191 && x < 226){
    flag2 = 1;}
  else if(x > 231 && x < 264){
    flag2 = 1;}
  else{
    flag2 = 0;}}

void comparacion11 (void){
  if (y > 21 && y < 55){
    flag1 = 1;}
  else if(y > 58 && y < 92){
    flag1 = 1;}
  else if(y > 95 && y < 127){
    flag1 = 1;}
  else if(y > 130 && y < 165){
    flag1 = 1;}
  else if(y > 168 && y < 203){
    flag1 = 1;}
  else{
    flag1 = 0;}}

void comparacion2 (void){
  if (x2 > 40 && x2 < 75){
    flag22 = 1;}
  else if(x2 > 80 && x2 < 110){
    flag22 = 1;}
  else if(x2 > 115 && x2 < 150){
    flag22 = 1;}
  else if(x2 > 153&& x2 < 188){
    flag22 = 1;}
    else if(x2 > 191 && x2 < 226){
    flag22 = 1;}
    else if(x2 > 231 && x2 < 264){
    flag22 = 1;}
  else{
    flag22 = 0;}}

void comparacion22 (void){
  if (y2 > 21 && y2 < 55){
    flag11 = 1;}
  else if(y2 > 58 && y2 < 92){
    flag11 = 1;}
  else if(y2 > 95 && y2 < 127){
    flag11 = 1;}
  else if(y2 > 130 && y2 < 165){
    flag11 = 1;}
  else if(y2 > 168 && y2 < 205){
    flag11 = 1;}
  else{
    flag11 = 0;}}

// ------------------------------------------------- Funciones de disparo para player1 -----------------------------------
// Disparo para player 1 hacia arriba
void disparo(void){
  if (inByte == 0x34 && flags == 0){
    flagm = 1; 
    vel += 4;
    f = x+20+vel;
    f1 = x+15+vel;

    if (f < 280){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( f, y+3, 5, 5, 0x0000);
    FillRect( f1, y+3, 5, 5, 0x04E8);
    delay(100);
    FillRect( f, y+3, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);  
    }}}}

// Disparo para player 1 hacia abajo
void disparo1(void){
  if (inByte == 0x34 && flags1 == 0){
    flagm = 1;
    vel1 -= 4;
    f = x-5+vel1;
    f1 = x+vel1;

    if (f > 40){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( f, y+3, 5, 5, 0x0000);
    FillRect( f1, y+3, 5, 5, 0x04E8);
    delay(100);
    FillRect( f, y+3, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);
    }}}}
    
// Disparo para player 1 hacia derecha
void disparo2(void){
  if (inByte == 0x34 && flags2 == 0){
    flagm = 1;
    vel2 += 4;
    f = y+15+vel2;
    f1 = y+10+vel2;

    if (f < 213){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( x+4, f, 5, 5, 0x0000);
    FillRect( x+4, f1, 5, 5, 0x04E8);
    delay(100);
    FillRect( x+4, f, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);
    }}}}

// Disparo para player 1 hacia izquierda
void disparo3(void){
  if (inByte == 0x34 && flags3 == 0){
    flagm = 1;
    vel3 -= 4;
    f = y-5+vel3;
    f1 = y+vel3;

    if (f > 20){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( x+4, f, 5, 5, 0x0000);
    FillRect( x+4, f1, 5, 5, 0x04E8);
    delay(100);
    FillRect( x+4, f, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);
    }}}}

// ------------------------------------------------- Funciones de disparo para player1 -----------------------------------
// Disparo para player 2 hacia arriba
void disparo4(void){
  if (inByte2 == 0x34 && flags4 == 0){
    flagm = 1;
    vel4 += 4;
    f2 = x2+20+vel4;
    f3 = x2+15+vel4;

    if (f2 < 280){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( f2, y2+3, 5, 5, 0x0000);
    FillRect( f3, y2+3, 5, 5, 0x04E8);
    delay(100);
    FillRect( f2, y2+3, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);  
    }}}}

// Disparo para player 2 hacia abajo
void disparo5(void){
  if (inByte2 == 0x34 && flags5 == 0){
    flagm = 1;
    vel5 -= 4;
    f2 = x2-5+vel5;
    f3 = x2+vel5;

    if (f2 > 40){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( f2, y2+3, 5, 5, 0x0000);
    FillRect( f3, y2+3, 5, 5, 0x04E8);
    delay(100);
    FillRect( f2, y2+3, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);
    }}}}

// Disparo para player 2 hacia derecha
void disparo6(void){
  if (inByte2 == 0x34 && flags6 == 0){
    flagm = 1;
    vel6 += 4;
    f2 = y2+15+vel6;
    f3 = y2+10+vel6;

    if (f2 < 213){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( x2+4, f2, 5, 5, 0x0000);
    FillRect( x2+4, f3, 5, 5, 0x04E8);
    delay(100);
    FillRect( x2+4, f2, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);
    }}}}

// Disparo para player 2 hacia izquierda
void disparo7(void){
  if (inByte2 == 0x34 && flags7 == 0){
    flagm = 1;
    vel7 -= 4;
    f2 = y2-5+vel7;
    f3 = y2+vel7;

    if (f2 > 20){  //  tone_function(melody_shoot, tempo2, 1);
    FillRect( x2+4, f2, 5, 5, 0x0000);
    FillRect( x2+4, f3, 5, 5, 0x04E8);
    delay(100);
    FillRect( x2+4, f2, 5, 5, 0x04E8);
    if (flagm == 1) {
    tone_function(melody_shoot, tempo2, 1);
    }}}}

// ------------------------------------------------ Función de comparación de colisiones ----------------------------------------
// Si el tiro impacta algún tanque el jugador que disparó al otro gana
void over (void){
  c = Collision(x, y, 15, 15, x2+4, f2, 5, 5 );
  c1 = Collision(x, y, 15, 15, f2, y2+3, 5, 5);
  c2 = Collision(x2, y2, 15, 15, x+4, f, 5, 5);
  c3 = Collision(x2, y2, 15, 15, f, y+3, 5, 5);
  if (c || c1){
    flagm = 0;
    flags = 1;
    flags1 = 1;
    flags2 = 1;
    flags3 = 1;
    flags4 = 1;
    flags5 = 1;
    flags6 = 1;
    flags7 = 1;
    FillRect(0, 0, 320, 240, 0xff00);
    String text4 = "PLAYER 2 WINS";
    LCD_Print(text4, 60, 90, 2, 0x0000, 0xff00);
    c = false;
    c1 = false;
    c2 = false;
    c3 = false;
     myFile = SD.open("prueba.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println("PLAYER 2 WINS");
    // close the file:
    myFile.close();
    }}
  else if (c2 || c3){
     flagm = 0;
    flags = 1;
    flags1 = 1;
    flags2 = 1;
    flags3 = 1;
    flags4 = 1;
    flags5 = 1;
    flags6 = 1;
    flags7 = 1;
    FillRect(0, 0, 320, 240, 0xff00);
    String text4 = "PLAYER 1 WINS";
    LCD_Print(text4, 60, 90, 2, 0x0000, 0xff00);
    c = false;
    c1 = false;
    c2 = false;
    c3 = false;
     myFile = SD.open("prueba.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println("PLAYER 1 WINS");
    // close the file:
    myFile.close();
    }}}
    
char ConversionDecimal(char x, char y) {
int x1;
int y1;
int resultado;
x1 = ConversionASCII(x);
y1 = ConversionASCII(y);
resultado = int(x1)*16 + int(y1);  
return resultado;
}

char ConversionASCII(char x) {
  if (x == '0'){
    return 0;
  }else if (x == '1'){
    return 1;
  }else if (x == '2'){
    return 2;
  }else if (x == '3'){
    return 3;
  }else if (x == '4'){
    return 4;
  }else if (x == '5'){
    return 5;
  }else if (x == '6'){
    return 6;
  }else if (x == '7'){
    return 7;
  }else if (x == '8'){
    return 8;
  }else if (x == '9'){
    return 9;
  }else if (x == 'a'){
    return 10;
  }else if (x == 'b'){
    return 11;
  }else if (x == 'c'){
    return 12;
  }else if (x == 'd'){
    return 13;
  }else if (x == 'e'){
    return 14;
  }else if (x == 'f'){
    return 15;
  }else {
    return 0;
  }
}

// -------------------------------------------------- Función que carga el fondo del juego -------------------------------------------------

void CargarFondo(int height, int width, int x1, int y1, int negro){
  if (negro == 1){
    LCD_Clear(0x00);
  }
  UltimaPocicion = 0;
  posicion_y = y1;
  primerCero = 0;
  char1 = 0;
  char2 = 0;
  x = 0;
  x_seg_ciclo = 0;
   int totalBytes = myFile.size();
    if (myFile) {
      for (x_seg_ciclo = 0; x_seg_ciclo < height; x_seg_ciclo++){
        for (x = 0; x < width*2 ; x++){
          myFile.seek(UltimaPocicion);
          while (myFile.available()){
            char caracter = myFile.read();
            if (caracter == ','){
              UltimaPocicion = myFile.position();
              break;
            }else if (caracter == '0' && primerCero == 0){
              primerCero = 1;
            }else if (caracter == 'x'){

            }else if (caracter == ' '){

            }else if (caracter == 13){

            } else{
              primerCero = 0;
              char1 = caracter;
              char2 = myFile.read();
              MatrizFondo[x] = ConversionDecimal(char1, char2);
            }
          }
        }
        LCD_Bitmap(x1, posicion_y, width, 1, MatrizFondo);
        posicion_y++;
      }
    } else {
    // if the file didn't open, print an error:
    Serial.println("error opening archivo.txt");
  }
}

void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++){
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER) 
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36);  // (MEMORYACCESS)
  LCD_DATA(0x40|0x80|0x20|0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
//  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on 
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
// ------------------------------------------ Función para enviar parámetros a LCD -----------------------------------------------------
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}

// -------------------------------------------- Función para enviar datos a LCD --------------------------------------------------------
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}

// -------------------------- Función para definir rango de direcciones de memoria con las cuales se trabajara -------------------------
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);   
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);   
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);   
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);   
  LCD_CMD(0x2c); // Write_memory_start
}

// ----------------------------------------- Función para borrar la pantalla - parámetros -----------------------------------------------
void LCD_Clear(unsigned int c){  
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);   
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
    }
  digitalWrite(LCD_CS, HIGH);
} 

// -------- Función para dibujar una linea horizontal - parámetros (coordenada x, coordenada y, longitud, color) ------------------------
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      LCD_DATA(c >> 8); 
      LCD_DATA(c); 
  }
  digitalWrite(LCD_CS, HIGH);
}
// ----------- Función para dibujar una linea vertical - parámetros (coordenada x, coordenada y, longitud, color) ------------------------ 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8); 
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}

// ------------- Función para dibujar una linea rectangulo - parámetros (coordenada x, coordenada y, longitud, color) ------------------------
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

// -------- Función para dibujar una linea rectangulo lleno - parámetros (coordenada x, coordenada y, longitud, color) ----------------------
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c); 
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

// -------------------- Función para dibujar texto - parámetros (texto,  coordenada x, coordenada y, color, background) --------------------
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}


// ------ Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits) -------
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      //LCD_DATA(bitmap[k]);    
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
    for (int j = 0; j < height; j++){
        k = (j*(ancho) + index*width -1 - offset)*2;
        k = k+width*2;
       for (int i = 0; i < width; i++){
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k+1]);
        k = k - 2;
       } 
    }
  }
  else{
     for (int j = 0; j < height; j++){
      k = (j*(ancho) + index*width + 1 + offset)*2;
     for (int i = 0; i < width; i++){
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k+1]);
      k = k + 2;
     } 
    }
  }
  digitalWrite(LCD_CS, HIGH);
}

bool Collision(int x1, int y1, int w1, int h1, int x3, int y3, int w3, int h3){
  return (x1 < x3 + w3) && (x1+ w1 > x3) && (y1 < y3 + h3) && (y1 + h1 > y3);
}
