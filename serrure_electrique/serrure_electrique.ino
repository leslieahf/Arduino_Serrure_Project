#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;
int i = 0; // Pour compter le nombre de touches saisies
int false_try = 0; // Pour compter le nombre de mauvaises saisies

char mdp[5] = "1234"; 
char saisi[5];
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

LiquidCrystal lcd(12, 11, A0, A1, A2, A3);

Servo servo_serr; // Création de l'objet Servo
const int angleFermeture = 0; // Position "Fermé"
const int angleOuverture = 90; // Position "Ouvert" 

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

//const int pinPWM = 9;

void setup() {
  //pinMode(pinPWM, OUTPUT);
  //analogWrite(pinPWM, 100);
  servo_serr.attach(A4); // Attache l'objet Servo à la broche A4 de l'Arduino
  servo_serr.write(angleFermeture); // Initialisation du servo en position "Fermé"
  Serial.begin(9600); // Initialisation de la console
  lcd.begin(16,2); // Initialisation de l'écran lcd 16x2

  // --- AJOUT POUR LA BASE DE DONNEES ---
  // Si la mémoire est vide, on garde "1234", sinon on lit la mémoire
  if (EEPROM.read(0) != 255) {
     for (int j=0; j<4; j++) mdp[j] = char(EEPROM.read(j));
     mdp[4] = '\0'; // Fermeture de la chaine
  }

  // Affichage initial sur la console
  Serial.println("Tapez sur le clavier...");
  Serial.print("Code : ");

  // Affichage initial sur l'écran
  lcd.print("Saisir le code : ");
  lcd.setCursor(0,1);
}

void loop() {
  char key = keypad.getKey();
  char buffer[50];

  // GESTION DU SITE WEB (COMMANDES USB)
  if (Serial.available() > 0) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    
    // --- LE SITE DEMANDE DE VERIFIER UN CODE ---
    if (msg.startsWith("CHECK:")) {
       String codeWeb = msg.substring(6);
       
       // Si le code du site est BON
       if (codeWeb == String(mdp)) {
           Serial.println("RESULT:SUCCESS"); // Signal au site
           Serial.println("Accès WEB autorisé !");
           lcd.clear(); lcd.print("ACCES WEB OK !");
           servo_serr.write(angleOuverture);
           delay(3000);
           servo_serr.write(angleFermeture);
           false_try = 0;
           // Reset écran
           memset(saisi, 0, 5); i = 0;
           lcd.clear(); lcd.print("Saisir le code :"); lcd.setCursor(0, 1);
       }
       // Si le code du site est MAUVAIS
       else {
           Serial.println("RESULT:FAIL"); // Signal au site
           Serial.println("Code WEB incorrect !");
           lcd.clear(); lcd.print("CODE WEB FAUX !");
           false_try++;
           
           if(false_try >= 3){
               Serial.println("RESULT:LOCKED");
               lcd.setCursor(0, 1); lcd.print("BLOQUE 1 MIN");
               delay(60000);
               false_try = 0;
           }
           delay(2000);
           // Reset écran
           memset(saisi, 0, 5); i = 0;
           lcd.clear(); lcd.print("Saisir le code :"); lcd.setCursor(0, 1);
       }
    }
    // --- LE SITE DEMANDE DE CHANGER LE MOT DE PASSE ---
    else if (msg.startsWith("SET:")) {
      String newCode = msg.substring(4);
      for (int j=0; j<4; j++) {
          EEPROM.write(j, newCode[j]); // Ecrit dans la mémoire
          mdp[j] = newCode[j];         // Met à jour la variable
      }
      Serial.println("SUCCESS:CODE_UPDATED"); 
      lcd.clear(); lcd.print("CODE CHANGE !");
      delay(2000);
      // Reset écran
      memset(saisi, 0, 5); i = 0;
      lcd.clear(); lcd.print("Saisir le code :"); lcd.setCursor(0, 1);
    }
  }

  if (key) { 
    // Enregistrement de la saisie
    if(i < 4 && key != '#'){
      Serial.print(key); // Affichage sur le moniteur
      saisi[i]= key;
      // Masquer la saisie par '*'
      lcd.setCursor(i,1); 
      lcd.print('*'); 
      i++;
    }
    // Vérification de la saisie
    else if(key == '#'){
      lcd.clear();
      // Cas où l'utilisateur valide avant d'avoir 4 chiffres
      if(i!=4){
        Serial.println("Code incomplet !");
        lcd.print("CODE INCOMPLET !");
      }
      // Cas où l'utilisateur tape un code valide
      else if(memcmp(saisi, mdp, 4) == 0){
        Serial.println("Accès autorisé !");
        lcd.print("ACCES AUTORISE !");
        // Ouverture de la porte puis fermeture
        servo_serr.write(angleOuverture);
        delay(3000);
        servo_serr.write(angleFermeture);
        false_try = 0; // Réinitialisation des tentatives
      }
      // Cas où l'utilisateur tape un code invalide
      else{
        Serial.println("Code incorrect !");
        lcd.print("CODE INCORRECT !");
        false_try++; // Incrémente après un échec
        // Blocage après 3 tentatives
        if(false_try == 3){
          Serial.println("Trop de mauvaises saisies. Serrure bloquée !");
          Serial.println("Réessayez dans 1 minute");
          lcd.print("SERRURE BLOQUEE !");
          lcd.setCursor(0, 1);
          lcd.print("ATTENDRE 1 MIN");
          delay(60000);
          false_try = 0; // Réinitialisation des tentatives après le délai
        }
        // Affichage des essais restants
        else{
          sprintf(buffer, "Encore %d essais", 3 - false_try);
          Serial.println(buffer);
          lcd.setCursor(0, 1);
          lcd.print(buffer);
        }
      }
      // Prépare la console pour la nouvelle saisie
      memset(saisi, ' ', 4);
      i = 0;
      Serial.print("Code : ");
      delay(2000);
      // Prépare l'écran pour la nouvelle saisie
      lcd.clear();
      lcd.print("Saisir le code :");
      lcd.setCursor(0, 1);
    }
    // Cas où l'utilisateur n'appuie pas sur '#' pour valider sa saisie
    else{
      Serial.println("Tapez sur '#' pour valider votre mot de passe");
    }
    delay(150); // Anti-rebond
  }
}
