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
byte n; //I2C로 현재역 송신받는 변수
volatile boolean done = false;

byte zero[8]={ //빈좌석 출력을 위한 사용자 기호
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

void setup() {
  lcd.createChar(0, zero);
  lcd.begin(16, 2);
  mySerial.begin(9600); //시리얼 통신
  Serial.begin(9600); //통신 확인 위한 시리얼
  Wire.begin(aisle);
  Wire.onReceive(receiveEvent);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("<Empty seat>");
}

void receiveEvent(int howMany){
  while(Wire.available()){
    n=Wire.read(); //I2C로 현재역 송신받음
    done=true;
  }
}

void LCD(byte seatStatus, byte currentStation){ //LCD 출력 함수   
  if(currentStation == ULSAN || currentStation == BEFORE_DAEJEON){
    Serial.println("3 and 4");
    if(seatStatus=='Y'){ //Y일때 빈좌석이 없음으로 삭제
      Serial.println("Y");
      lcd.setCursor(0,1);
      lcd.write((byte)0);
      lcd.setCursor(1,1);
      lcd.write((byte)0);
    }else if(seatStatus=='N'){ // N일때 빈좌석 번호 출력
      Serial.println("N");
      lcd.setCursor(0,1);
      lcd.print("1A");
    }
  }
 else{
      lcd.setCursor(0,1);
      lcd.write((byte)0);
      lcd.setCursor(1,1);
      lcd.write((byte)0);
 }
}

void loop() {
  if (mySerial.available()) {
    c = mySerial.read(); //시리얼통신으로 송신받음
   }
  if (done){ //데이터 수신 확인
    LCD(c,n); //LCD 출력
  }
}
    
