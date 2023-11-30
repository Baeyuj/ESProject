#include <state.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define seat 7 //슬레이브 주소 정의

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

byte inputData; //통신으로 받을 데이터 변수 선언

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2); //lcd 크기 지정

  Wire.begin(seat); //슬레이브 주소로 I2C 통신 참여
  Wire.onReceive(receiveEvent); //통신으로 데이터를 받는 대응 핸들러 함수 등록
  Serial.begin(9600); //보드레이트 설정
}

void printLCD(byte c) { //lcd 문장 출력 함수
  if(c == BUSAN) {
    lcd.setCursor(0, 0);
    lcd.print("This station is");
    lcd.setCursor(0, 1);
    lcd.print("Busan station");
    Serial.println("Busan");
  }
  else if (c == BEFORE_ULSAN){
    lcd.setCursor(0, 0);
    lcd.print("We'll arrive at");
    lcd.setCursor(0, 1);
    lcd.print("Ulsan station");
    Serial.println("Ulsan Before");
  }
  else if(c == ULSAN) {
    lcd.setCursor(0, 0);
    lcd.print("This station is");
    lcd.setCursor(0, 1);
    lcd.print("Ulsan station");
    Serial.println("Ulsan");
  }
  else if(c == BEFORE_DAEJEON) {
    lcd.setCursor(0, 0);
    lcd.print("We'll arrive at");
    lcd.setCursor(0, 1);
    lcd.print("Daejeon station");
    Serial.println("Daejeon Before");
  }
  else if(c == DAEJEON) {
    lcd.setCursor(0, 0);
    lcd.print("This station is");
    lcd.setCursor(0, 1);
    lcd.print("Daejeon station");
    Serial.println("Daejeon");
  }
  else if(c == SEOUL) {
    lcd.setCursor(0, 0);
    lcd.print("This station is");
    lcd.setCursor(0, 1);
    lcd.print("Seoul station");
    Serial.println("Seoul");
  }
  else if(c == DEFAULT_STATE) { //초기화
    lcd.clear(); //lcd 지우기
    Serial.println("default");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

void receiveEvent(int howMany) {
  inputData = Wire.read(); //inputData에 읽어들인 값 할당
  printLCD(inputData);
}
