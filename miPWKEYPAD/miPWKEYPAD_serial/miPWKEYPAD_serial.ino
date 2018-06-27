/*
 * A program to control a 3x4 KEYPAD, check and change the password
 * Based on the Keypad library of Mark Stanley, Alexander Brevig <mstanley@technologist.com, alexanderbrevig@gmail.com>
 * Copyright 2018 Nguyen Trong Nghia <mtxl8877@gmail.com>
 */

#include <Keypad.h>
#include <EEPROM.h>

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

void setup() {
  Serial.begin(9600);
  //Read the current password in EEPROM and copy it to the masterPW string
  for(byte i=0; i<PWLength-1; i++){
    masterPW[i]=EEPROM.read(i);
  }
  delay(100);
}

void loop() {
  Serial.println("Please enter your choice");
  Serial.println("Press * to change password");
  Serial.println("Press # to check password"); 
  
  while(key!='#' && key!='*'){
    key=keypad.getKey();
  }

  if(key=='#'){
    Serial.println("Enter password:");
    checkPW();
  }

  if(key=='*'){
    Serial.println("Change password");
    Serial.println("Please enter old password");    
    changePW();
  }
}

//Procedure's used to check if password entered is correct or not 
/************************************************/
void checkPW(){    
  insert(customPW);

  if(strcmp(customPW, masterPW)==0)
    Serial.println("Password correct");
  else
    Serial.println("Password incorrect"); 
  delay(2000); 
  clearPW(customPW);
}

//Procedure's used to change to a new password
/************************************************/
void changePW(){
  insert(tmpPW);
  if(strcmp(tmpPW, masterPW)!=0){
    Serial.println("Wrong password");
    clearPW(tmpPW);
  }
  else{
    Serial.println("Please enter your new password:");
    insert(customPW);
    Serial.println("Enter again:");
    insert(tmpPW);
      
    if(strcmp(tmpPW, customPW)!=0){
      Serial.println("Passwords don't match, try again");
      clearPW(tmpPW);
      clearPW(customPW);
      delay(2000);
    }
    else{
      strcpy(masterPW, tmpPW);
      Serial.println("Successful");
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
      Serial.print("*");
    }
  }
  key=NO_KEY;
  Serial.println();
}

//Function's used to clear a string contains the password
/************************************************/
void clearPW(char localPW[PWLength]){
  byte cnt=0;
  while(cnt<PWLength-1)
    localPW[cnt++]=0;
  key=NO_KEY;
}

