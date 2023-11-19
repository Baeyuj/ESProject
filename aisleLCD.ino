#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <state.h>
#define sv1 8

SoftwareSerial mySerial(4,5);

const int RS = 2; const int EN = 3;
const int D4 = 6; const int D5 = 7;
const int D6 = 8; const int D7 = 9;
LiquidCrystal lcd(RS,EN,D4,D5,D6,D7);
 
byte c; //시리얼통신으로 상태 송신 받는 변수
byte n; //I2C로 현재역 송신 받는 변수


void setup() {
  lcd.begin(16, 2);
  mySerial.begin(9600); //시리얼 통신
  Wire.begin();
}


void LCD(byte seatStatus, byte currentStation){
  lcd.print("Empty seat");
   
  if(currentStation== ULSAN || currentStation == BEFORE_DAEJEON){
    if(seatStatus==1){
      lcd.setCursor(16,1);
      lcd.print(" ");
    }else{
      lcd.setCursor(16,1);
      lcd.print(" ");
    }
  }
  else{
      lcd.setCursor(16,1);
      lcd.print(" ");
    }
}

void loop() {
  Wire.requestFrom(sv1, 1); //데이터 전송 받기 위함
  if (mySerial.available()) {
    c = mySerial.read(); //시리얼통신으로 송신받음
   }
  while (Wire.available()) {
    n = Wire.read(); //I2C로 현재역 송신받음
    LCD(c,n); //LCD 출력
  }
   delay(100);
}
    
