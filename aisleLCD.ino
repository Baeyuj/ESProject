#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <state.h>
#define aisle 8 //I2C 주소 정의

SoftwareSerial mySerial(4,5);

const int RS = 2; const int EN = 3;
const int D4 = 6; const int D5 = 7;
const int D6 = 8; const int D7 = 9;
LiquidCrystal lcd(RS,EN,D4,D5,D6,D7);
 
byte c; //시리얼통신으로 상태 송신 받는 변수
byte n; //I2C로 현재역 송신 받는 변수
volatile boolean done = false;

void setup() {
  lcd.begin(16, 2);
  mySerial.begin(9600); //시리얼 통신
  Serial.begin(9600);
  Wire.begin(aisle);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int howMany){
  while(Wire.available()){
    n=Wire.read(); //I2C로 현재역 송신받음
    done=true;
  }
}

void LCD(byte seatStatus, byte currentStation){ //LCD 출력 함수
  lcd.print("Empty seat");
   
  if(currentStation == ULSAN || currentStation == BEFORE_DAEJEON){
    Serial.write("통신");
    if(seatStatus=='Y'){
      lcd.setCursor(16,1);
      lcd.print(" ");
     
    }else{
      lcd.setCursor(16,1);
      lcd.print("1A");
    }
  }
  else{
      lcd.setCursor(16,1);
      lcd.print(" ");
    }
}

void loop() {
  if (mySerial.available()) {
    c = mySerial.read(); //시리얼통신으로 송신받음
   }
  if (done){ //데이터 수신 확인
    LCD(c,n); //LCD 출력
  }
   delay(1000);
}
    
