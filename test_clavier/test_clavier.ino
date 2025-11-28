#include <Keypad.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

void setup() {
  Serial.begin(9600);
  Serial.println("Diagnostic clavier : prêt.");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Touche détectée : ");
    Serial.println(key);
  }
  else {
    Serial.println("Aucune touche...");
  }

  delay(500);
}
