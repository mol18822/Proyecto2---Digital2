// Universidad del Valle de Guatemala
// Electrónica Digital II
// Proyecto 2 - BOMBER-TANK
// Luis Pedro Molina - 18822
// Guillermo Lam - 19267

// Declaración de puertos y variables
// Pushbottons para player1
int PBP1_UP    = 2;     // up
int PBP1_DOWN  = 3;     // down
int PBP1_RIGHT = 4;     // derecha
int PBP1_LEFT  = 5;     // izquierda
int PBP1_SHOOT = 6;     // bomba
int valP1;              // variable que lee valor de pb up
int valP2;              // variable que lee valor de pb down
int valP3;              // variable que lee valor de pb right
int valP4;              // variable que lee valor de pb left
int valP5;              // variable que lee valor de pb shoot

// Buzzer para efectos de sonido
int buzz = A0;

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
// Declaración de PBS con sus respectivos pines como inputs
pinMode(PBP1_UP   , INPUT_PULLUP);
pinMode(PBP1_DOWN , INPUT_PULLUP);
pinMode(PBP1_RIGHT, INPUT_PULLUP);
pinMode(PBP1_LEFT , INPUT_PULLUP);
pinMode(PBP1_SHOOT, INPUT_PULLUP);
pinMode(buzz      , OUTPUT);    

}

void loop() {
  // read from port 1, send to port 0:
  delay(500);

// Variables que leen PBS de player1  
  valP1 = digitalRead(PBP1_UP);
  valP2 = digitalRead(PBP1_DOWN);
  valP3 = digitalRead(PBP1_RIGHT);
  valP4 = digitalRead(PBP1_LEFT);
  valP5 = digitalRead(PBP1_SHOOT);
    
  if (valP1 == 0) {
    Serial.write("0");
  }

  if (valP2 == 0) {
    Serial.write("1");
  }

  if (valP3 == 0) {
    Serial.write("2");
  }  

  if (valP4 == 0) {
    Serial.write("3");
  }  

  if (valP5 == 0) {
    Serial.write("4");
  }
}    

 
