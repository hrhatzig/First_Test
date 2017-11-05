/******************************************
*                                         *
*    DOOR PROJECT WITH ARDUINO            *
*                                         *
*  Chrysostomos A. Chatzigeorgiou         *
*      hrhatzig@gmail.com                 *
*          23/10/2017                     *
*                                         *
*******************************************/

#include <Keypad.h>
#include <LiquidCrystal.h>

#define PIN_SIZE 4

LiquidCrystal lcd(2,3,4,5,6,7);

const byte ROWS = 4;               //four rows
const byte COLS = 4;               //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {22,24,26,28}; //Rows 0 to 3
byte colPins[COLS] = {30,32,34,36}; //Columns 0 to 3

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char pin[4];
char root[4] = {'1', '3', '5', '7'};
//int i;
int people = 0;

const int pirPin_1    = 9;
const int pirPin_2    = 12;       // the number of the PIR pin
const int ledPin_1    = 11;       // the number of the LED pin
const int ledPin_2    = 10;       // the number of the LED pin
const int ledPin_3    = 13;
const int buttonPin_2 = 31;       // the number of the pushbutton pin


int buttonState_1 = 0;           // variable for reading the pushbutton status
int buttonState_2 = 0;           // variable for reading the pushbutton status
int pirState_1    = 0;

void setup() {
  pinMode(ledPin_1, OUTPUT);
  pinMode(ledPin_3, OUTPUT);
  pinMode(ledPin_2, OUTPUT);
  
  pinMode(pirPin_2, INPUT);
  pinMode(pirPin_1, INPUT);
  pinMode(buttonPin_2, INPUT);
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  lcd.print("Number of people:");
  lcd.setCursor(7, 1);
  lcd.print(people);
  
  digitalWrite(ledPin_1, LOW);
  digitalWrite(ledPin_2, LOW);
  digitalWrite(ledPin_3, LOW);
}

void loop() {
  pirState_1    = digitalRead(pirPin_1);
  buttonState_1 = digitalRead(pirPin_2);
  buttonState_2 = digitalRead(buttonPin_2);
  
  int tries = 0;
  
  if (pirState_1 == HIGH) {
    digitalWrite(ledPin_2, HIGH);
    if (buttonState_1 == HIGH) {
      lcd.clear();
      delay(500);
      digitalWrite(ledPin_1, HIGH);
      lcd.print("Welcame!");
      
      while(1) {
        lcd.setCursor(0, 1);
        lcd.print("Give your pass: ");  
        ReadPIN();
        int check = CheckPIN(pin, root);
        if(check == 0) {
          tries++;
          if(tries == 3) {
            lcd.clear();
            lcd.print("Wait a minute...");
            delay(10000);
            tries = 0;
            lcd.clear();
          }
          lcd.clear();
          lcd.print("Try again..");
        } else {
          tries = 0;
          digitalWrite(ledPin_2, HIGH);
          people++;
          lcd.clear();
          lcd.print("Hello Root!");
          delay(1000);
          setup();
          break;
        }
      }
    }
    digitalWrite(ledPin_2, LOW);
  }
  
  
  if ((buttonState_2 == HIGH) && (people > 0)) {        //someone must be inside...
    lcd.clear();
    digitalWrite(ledPin_3, HIGH);
    lcd.print("Hi again!");
    
    while(1) {
      lcd.setCursor(0, 1);
      lcd.print("Give your pass: ");  
      ReadPIN();
      int check = CheckPIN(pin, root);
      if(check == 0) {
        tries++;
        if(tries == 3) {
          lcd.clear();
          lcd.print("Wait a minute...");
          delay(10000);
          tries = 0;
          lcd.clear();
        }
        lcd.clear();
        lcd.print("Try again..");
      } else {
        tries = 0;
        digitalWrite(ledPin_1, HIGH);
        digitalWrite(ledPin_2, HIGH);
        people--;
        lcd.clear();
        lcd.print("Buy Root!");
        delay(1000);
        setup();
        break;
      }
    }
  }
}

/*
* Function that read the PIN from user and save it into *pin.
*/
void ReadPIN()
{
  char key = keypad.getKey();
  
  while(key == NO_KEY) {
    key = keypad.getKey();
  }
  
  if(key) {
    lcd.clear();
    pin[0] = key;
    lcd.print("*");
    
    for(int i = 1; i < PIN_SIZE; i++) {
      key = keypad.getKey();
      while(key == NO_KEY) {
        key = keypad.getKey();
      }
      pin[i] = key;
      lcd.print("*");
    }
  }
}

/*
* pin:  The number that the user gives.
* user: The golden pass of the user.
* Returns 1 if PIN is OK.
*/
int CheckPIN(char *pin, char *user)
{
  for(int i = 0; i < PIN_SIZE; i++) {
    if(pin[i] != user[i])
      return 0;
  }
  return 1;
}

/*
* pin: Print the PIN.
*/
void PrintPIN(char *pin)
{
  lcd.print("Your PIN: ");
  for(int i = 0; i < PIN_SIZE; i++) {
    lcd.print(pin[i]);
  }
}
