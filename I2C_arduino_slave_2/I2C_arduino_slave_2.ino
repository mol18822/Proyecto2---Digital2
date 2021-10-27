// Universidad del Valle de Guatemala
// Electrónica Digital II
// Proyecto 2 - BOMBER-TANK
// Luis Pedro Molina - 18822
// Guillermo Lam - 19267
// Declaración de puertos y variables

// Pushbottons para player2
int PBP2_UP    = 2;     // up
int PBP2_DOWN  = 3;     // down
int PBP2_RIGHT = 4;     // derecha
int PBP2_LEFT  = 5;     // izquierda
int PBP2_SHOOT = 6;     // bomba
int valP6;              // variable que lee valor de pb up              
int valP7;              // variable que lee valor de pb down
int valP8;              // variable que lee valor de pb right
int valP9;              // variable que lee valor de pb left
int valP10;             // variable que lee valor de pb shoot

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
// Declaración de PBS con sus respectivos pines como inputs
pinMode(PBP2_UP   , INPUT_PULLUP);
pinMode(PBP2_DOWN , INPUT_PULLUP);
pinMode(PBP2_RIGHT, INPUT_PULLUP);
pinMode(PBP2_LEFT , INPUT_PULLUP);
pinMode(PBP2_SHOOT, INPUT_PULLUP);

}

void loop() {
  // read from port 1, send to port 0:
  delay(500);

// Variables que leen PBS de player2
  valP6  = digitalRead(PBP2_UP);
  valP7  = digitalRead(PBP2_DOWN);
  valP8  = digitalRead(PBP2_RIGHT);
  valP9  = digitalRead(PBP2_LEFT);
  valP10 = digitalRead(PBP2_SHOOT);
  
  if (valP6 == 0) {
    Serial.write("0");
  }

  if (valP7 == 0) {
    Serial.write("1");
  }

  if (valP8 == 0) {
    Serial.write("2");
  }

  if (valP9 == 0) {
    Serial.write("3");
  }

  if (valP10 == 0) {
    Serial.write("4");
  }
}
