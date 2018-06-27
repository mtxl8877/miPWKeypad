/*
 * A program to control a 3x4 KEYPAD, check and change the password
 * Based on the Keypad library of Mark Stanley, Alexander Brevig <mstanley@technologist.com, alexanderbrevig@gmail.com>
 * Copyright 2018 Nguyen Trong Nghia <mtxl8877@gmail.com>
 */

#include <Keypad.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

//Set the length of a string contains the password to 7, 6 used to contains the characters and 1 used for NULL
#define PWLength 7

const byte ROWS=4;
const byte COLS=3;
char keys[ROWS][COLS]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
//Set the Pins for rows and collumns of keypad, rows start from up to down, collumns start from left to right
byte rowPins[ROWS]={2,3,4,5};
byte colPins[COLS]={6,7,8};

char masterPW[PWLength]; //string contains the current password
char customPW[PWLength]; //string contains the password entered from keypad to check
char tmpPW[PWLength]; //string used as a buffer when chaging password
char key;

Keypad keypad=Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup() {
  //Read the current password in EEPROM and copy it to the masterPW string
  for(byte i=0; i<PWLength-1; i++){
    masterPW[i]=EEPROM.read(i);
  }
  delay(10);

  lcd.init();
  lcd.backlight();
  lcd.home();
  lcd.print("Booting up");
  lcd.setCursor(0,1);
  lcd.print("Please wait...");
  delay(1000);
  lcd.clear();
  delay(10);
}

void loop() {
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Press #:check");
  lcd.setCursor(1,1);
  lcd.print("Press *:change");
  
  while(key!='#' && key!='*'){
    key=keypad.getKey();
  }

  if(key=='#'){    
    checkPW();
  }

  if(key=='*'){       
    changePW();
  }
}

//Procedure's used to check if password entered is correct or not 
/************************************************/
void checkPW(){    
  lcd.clear();
  lcd.home();
  lcd.print("Enter password:");
  lcd.setCursor(5,1);
  insert(customPW);
  delay(100);
  lcd.clear();

  if(strcmp(customPW, masterPW)==0){
    lcd.setCursor(3,0);
    lcd.print("PW correct");
    lcd.setCursor(1,1);
    lcd.print("Please come in");
    }    
  else{
    lcd.setCursor(2,0);
    lcd.print("PW incorrect");
    lcd.setCursor(0,1);
    lcd.print("Please try again");
    }    
  delay(2000); 
  clearPW(customPW);
}

//Procedure's used to change to a new password
/************************************************/
void changePW(){
  lcd.clear();
  lcd.home();
  lcd.print("Old password?"); 
  lcd.setCursor(5,1);
  insert(tmpPW);
  delay(100);
  lcd.clear();
  
  if(strcmp(tmpPW, masterPW)!=0){    
    lcd.home();
    lcd.print("Wrong password!!");
    lcd.setCursor(0,1);
    lcd.print("Please try again");
    clearPW(tmpPW);
    delay(2000);
  }
  else{
    lcd.home();
    lcd.print("New password?");
    lcd.setCursor(5,1);
    insert(customPW);
    delay(100);
    lcd.clear();
    lcd.home();
    lcd.print("Enter again:");
    lcd.setCursor(5,1);
    insert(tmpPW);
    delay(100);
    lcd.clear();      
    if(strcmp(tmpPW, customPW)!=0){
      lcd.home();
      lcd.print("PW Don't match");
      lcd.setCursor(0,1);
      lcd.print("Try again");
      clearPW(tmpPW);
      clearPW(customPW);
      delay(2000);
    }
    else{
      strcpy(masterPW, tmpPW);
      lcd.setCursor(3,0);
      lcd.print("Successful");
      for(byte i=0; i<PWLength-1; i++){
        EEPROM.update(i, masterPW[i]);
        delay(4);
      }
      clearPW(tmpPW);
      clearPW(customPW);
      delay(2000);
    }
  }
}

//Function's used to insert character into a string contains the password
/************************************************/
void insert(char localPW[PWLength]){
  byte cnt=0;
  while(cnt<PWLength-1){
    key=keypad.getKey();
    if(key && key!='*' && key!='#'){
      localPW[cnt]=key;
      cnt++;
      lcd.print("*");
    }
  }
  key=NO_KEY;
}

//Function's used to clear a string contains the password
/************************************************/
void clearPW(char localPW[PWLength]){
  byte cnt=0;
  while(cnt<PWLength-1)
    localPW[cnt++]=0;
  key=NO_KEY;
}

