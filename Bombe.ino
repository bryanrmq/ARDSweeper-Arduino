#include <LiquidCrystal.h>
#include <DFR_Key.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

DFR_Key keypad;

int key     = 0;
int keyIn   = 0;

#define btnRIGHT  3
#define btnUP     2
#define btnDOWN   4
#define btnLEFT   1
#define btnSELECT 0
#define btnNONE   5

#define MAX_FAIL  3

char* code = "12344321";
int userValue     = btnNONE;
int i             = 0;
int fail          = 0;

bool keyPress = false;

bool explosionClear = false;

// read the buttons
int readButtons()
{
  key = keypad.getKey();
  keyIn = analogRead(0);
  keyPress = true;
  if (keyIn > 1000) return btnNONE;
  if (keyIn < 50)   return btnRIGHT;  
  if (keyIn < 250)  return btnUP; 
  if (keyIn < 450)  return btnDOWN; 
  if (keyIn < 650)  return btnLEFT; 
  if (keyIn < 850)  return btnSELECT;  
  return btnNONE;
}

void setup()
{
  lcd.begin(16, 2);
  showCode();
  
  char temp;
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}

bool checkValue(int inputBtn, int i) {
  int val = code[i] - '0';
  if(val == inputBtn)
    return true;
  return false;
}

void showCode() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CODE :");
  lcd.setCursor(0,1); 
  lcd.print(code);
}

void explosion() {
  lcd.clear();
  delay(250);
  lcd.setCursor(0, 0  );
  lcd.print("XXXX ERROR XXXX");
  lcd.setCursor(0, 1);
  lcd.print("EXPLOSION...");
  delay(800);
}

void success() {
  lcd.clear();
  delay(250);
  lcd.setCursor(0, 0  );
  lcd.print("CODE : SUCCESS");
  lcd.setCursor(0, 1);
  lcd.print("DESARMED");
  delay(800);
}

void wrongButton() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WRONG BUTTON");
  lcd.setCursor(0, 1);
  lcd.print("TRY : ");
  lcd.setCursor(6, 1);
  lcd.print(fail);
  lcd.setCursor(7, 1);
  lcd.print("/");
  lcd.setCursor(8, 1);
  lcd.print(MAX_FAIL);
  delay(1000);
}
 
void loop()
{
  if(fail >= MAX_FAIL) {
    explosion();
  } else {
    if(code[i] != '\n') {
      lcd.setCursor(i,1);
      key = readButtons();
      if(key != btnNONE) {
        if(checkValue(key, i)) {
          lcd.print('X');
          i++;
        } else {
          fail++;
          wrongButton();
          showCode();
        }
        delay(150);
      }
    } else {
      fail = 0;
      success();
    }
  }
  delay(150);
}

