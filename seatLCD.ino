#include <state.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define seat 7 //슬레이브 주소 정의

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

byte inputData; //통신으로 받을 데이터 변수 선언

unsigned long l1 = 0; //millis() 시간 기준 변수
unsigned long l2 = 0; //millis() 시간 체크 변수
unsigned long interval = 60000; //역 도착 출력 후 lcd 꺼지는 시간 == 1분

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2); //lcd 크기 지정

  Wire.begin(seat); //슬레이브 주소로 I2C 통신 참여
  Wire.onReceive(receiveEvent); //통신으로 데이터를 받는 대응 핸들러 함수 등록
  Serial.begin(9600); //보드레이트 설정
}

void printLCD(byte c) { //lcd 문장 출력 함수
  switch(c) {
      case BUSAN: //출발역-부산
        if(l2 - l1 >= interval) { //LCD 출력 후 1분이 지났을 때 실행
          lcd.clear(); //LCD 지우기
          Serial.println("clear");
        }
        else {
          lcd.setCursor(0, 0);
          lcd.print("This station is");
          lcd.setCursor(0, 1);
          lcd.print("Busan");
          Serial.println("Busan");
        }
        break;
      case BEFORE_ULSAN: //울산역 도착 3분 전
        l1 = l2; //기준 시간 변수 값 재할당 (3분 전->도착역으로 바뀌는 시점)
        lcd.setCursor(0, 0);
        lcd.print("We'll arrive at");
        lcd.setCursor(0, 1);
        lcd.print("Ulsan station");
        Serial.println("Ulsan Before");
        break;
      case ULSAN: //울산역 도착
        if(l2 - l1 >= interval) { //LCD 출력 후 1분이 지났을 때 실행
          lcd.clear(); //LCD 지우기
          Serial.println("clear");
        }
        else {
          lcd.setCursor(0, 0);
          lcd.print("This station is");
          lcd.setCursor(0, 1);
          lcd.print("Ulsan");
          Serial.println("Ulsan");
        }
        break;
      case BEFORE_DAEJEON: //대전역 도착 3분 전
        l1 = l2; //기준 시간 변수 값 재할당 (3분 전->도착역으로 바뀌는 시점)
        lcd.setCursor(0, 0);
        lcd.print("We'll arrive at");
        lcd.setCursor(0, 1);
        lcd.print("Daejeon station");
        Serial.println("Daejeon Before");
        break;
      case DAEJEON: //대전역 도착
        if(l2 - l1 >= interval) { //LCD 출력 후 1분이 지났을 때 실행
          lcd.clear(); //LCD 지우기
          Serial.println("clear");
        }
        else {
          lcd.setCursor(0, 0);
          lcd.print("This station is");
          lcd.setCursor(0, 1);
          lcd.print("Daejeon");
          Serial.println("Daejeon");
        }
        break;
      case SEOUL: //종착역-서울 도착
        lcd.setCursor(0, 0);
        lcd.print("This station is");
        lcd.setCursor(0, 1);
        lcd.print("Seoul, last stop");
        Serial.println("Seoul");
        break;
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void receiveEvent(int howMany) {
  inputData = Wire.read(); //inputData에 읽어들인 값 할당
  printLCD(inputData);
  l2 = millis(); //l2에 현재 시간 할당
}
