#include <Keypad.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;
int i = 0;
char mdp[4] = {'1','2','3','4'};
char saisi[4];
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
  Serial.begin(9600); // Initialisation de la console
  Serial.println("Taper sur le clavier...");
  Serial.print("code : ");
}

void loop() {
  char key = keypad.getKey();

  if (key) { 
    if(i != 4 && key != '#'){
      Serial.println(key);
      saisi[i]= key;
      i++;
    }else{
      if(memcmp(saisi, mdp, 4) == 0){
        Serial.print("code correct ");
      }else{
        Serial.print("code mauvais ");
      }
      Serial.print("\ncode : ");
      memset(saisi, ' ', 4);
      i = 0;
    }
    /*Serial.print("Touche : ");
    Serial.println(key);*/
    delay(150); // Anti-rebond
  }
}
