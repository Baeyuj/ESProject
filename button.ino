#include <state.h>
#include <Wire.h>
#include <SPI.h>

#define seat 7  // I2C 좌석 LCD 주소
#define aisle 8 // I2C 복도 LCD 주소

byte seatOccupied;              // 좌석 착석 여부 
int button[5] = {2, 3, 4, 5 ,6};   // 버튼 핀
byte state = 0;                 // 현재 상태

int S[2] = { 9, 10 }; // SPI 통신: seatSensor, seatActuator

unsigned long l1 = 0, l2 = 0, l3 = 0, l4 = 0; // 시간 체크
unsigned long interval = 10000;                // state 변경을 위한 시간

void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  Wire.begin();
  
  for (int i = 0; i < 2; i++) { // SPI 통신 설정
    pinMode(S[i], OUTPUT);
    digitalWrite(S[i], HIGH);
  }
  
  for (int i = 0; i < 5; i++) { // 버튼 설정
    pinMode(button[i], INPUT);
  }
  
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  Serial.begin(9600);
}

byte transferAndWait(const byte what) { // SPI 통신 요청 함수 
  byte a = SPI.transfer(what);
  delayMicroseconds(20);
  return a;
}

void readSensor() {             // seatSensor에게 값 요청
  digitalWrite(S[0], LOW);      // S[0] == seatSensor
  
  Serial.println("Sensor OK");
  transferAndWait('s');         // 해당 좌석에 누가 앉아있는지 값 요청
  transferAndWait(255);
  seatOccupied = transferAndWait(255);
  
  digitalWrite(S[0], HIGH);
}

void controlActuator() {        // seatActuator 컨트롤
  digitalWrite(S[1], LOW);      // S[1] == seatActuator
  
  Serial.println("Actuator OK");
  SPI.transfer(seatOccupied);   // 착석 여부 액추에이터에 전송
  
  digitalWrite(S[1], HIGH);
}

void checkButton(){
  if(digitalRead(button[0]) == HIGH)        // 0번 버튼일 때 SEOUL
    state = BUSAN;
  else if(digitalRead(button[1]) == HIGH){  // 1번 버튼일 때 BEFORE_ULSAN
    state = BEFORE_ULSAN;
    l1 = millis();
  }
  else if(digitalRead(button[2]) == HIGH){  // 2번 버튼일 때 BEFORE_DAEJEON
    state = BEFORE_DAEJEON;
    l3 = millis();
    controlActuator();                      // 액추에이터 컨트롤
  }
  else if(digitalRead(button[3]) == HIGH)   // 3번 버튼일 때 SEOUL
    state = SEOUL; 
  else if(digitalRead(button[4]) == HIGH)   // 4번 버튼일 때 초기화
    state = DEFAULT_STATE;  
}

void writeI2C(byte data){         // I2C 통신하는 함수
  Wire.beginTransmission(seat);
  Wire.write(data);
  Wire.endTransmission();

  Wire.beginTransmission(aisle);
  Wire.write(data);
  Wire.endTransmission();
}

void setMode() {                                // 상태를 설정해 I2C 통신 및 센서, 액추에이터 컨트롤
  switch(state){
    case BUSAN:                 
      Serial.println("BUSAN");
      writeI2C(BUSAN);
      break;
    case BEFORE_ULSAN:
      Serial.println("BEFORE_ULSAN");
      writeI2C(BEFORE_ULSAN);
      if(l2 - l1 >= interval) state = ULSAN;    // 일정 시간이 지나면 state를 ULSAN으로 변경
      break;
    case ULSAN:
      Serial.println("ULSAN");
      writeI2C(ULSAN);
      readSensor();                             // 센서 읽어옴
      break;
    case BEFORE_DAEJEON:
      Serial.println("BEFORE_DAEJEON");
      writeI2C(BEFORE_DAEJEON);
      readSensor();                             // 센서 읽어옴
      if(l4 - l3 >= interval) state = DAEJEON;  // 일정 시간이 지나면 state를 DAEJEON으로 변경
      break;
    case DAEJEON:
      Serial.println("DAEJEON");
      writeI2C(DAEJEON);
      seatOccupied=0;
      controlActuator();
      break;
    case SEOUL:
      Serial.println("SEOUL");
      writeI2C(SEOUL);
      break;
    case DEFAULT_STATE:
      Serial.println("DEFAULT_STATE");
      writeI2C(DEFAULT_STATE);
    
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  l2 = millis(); l4 = millis();                 // 현재 시간 기록
  checkButton();                                // 버튼 읽어옴
  setMode();                                    // 모드 설정
}
