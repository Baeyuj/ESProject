#include <state.h>
#include <Wire.h>
#include <SPI.h>

#define seat 7 // 아이씨
#define aisle 8 // 아이씨

byte seatOccupied; // 압력 
int button[4] = {2, 3, 4, 5};
// 버튼 디바운싱을 위한 현재, 마지막 버튼 상황
boolean lastButton[4] = { false, false, false, false };
boolean currentButton[4] = { false, false, false, false };
byte state = 0;

int S[2] = { 9, 10 }; // spi 아두이노 4, 5

unsigned long l1 = 0, l2 = 0, l3 = 0, l4 = 0; 
unsigned long interval = 3000; 

void setup() {
  // put your setup code here, to run once:
  SPI.begin();
  Wire.begin();
  
  for (int i = 0; i < 2; i++) { // 통신
    pinMode(S[i], OUTPUT);
    digitalWrite(S[i], HIGH);
  }
  
  for (int i = 0; i < 4; i++) { // 버튼
    pinMode(button[i], INPUT);
  }
  
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  Serial.begin(9600);
}

byte transferAndWait(const byte what) { // sip 신호 
  byte a = SPI.transfer(what);
  delayMicroseconds(20);
  return a;
}

void readSensor() { // 아두이노 4에게 압력값 받기
  digitalWrite(S[0], LOW);// S[0]-> 아두이노 4
  Serial.println("Sensor OK");
  transferAndWait('s'); // 센서에 사람이 있는지 데이터 전송 요청
  transferAndWait(99);
  seatOccupied = transferAndWait(99);
  
  digitalWrite(S[0], HIGH);
}

void controlActuator() { // 아두이노 5에게 압력 신호 보내기
  digitalWrite(S[1], LOW); // S[1]-> 아두이노 5
  Serial.println("Actuator OK");
  SPI.transfer(seatOccupied); // 좌석에 사람이 있는지 여부를 액추에이터에 전송함 
  digitalWrite(S[1], HIGH);
}

void checkButton(){
  if(digitalRead(button[0]) == HIGH)
    state = BUSAN;
  else if(digitalRead(button[1]) == HIGH){
    state = BEFORE_ULSAN;
    l1 = millis();
  }
  else if(digitalRead(button[2]) == HIGH){
    state = BEFORE_DAEJEON;
    l3 = millis();
  }
  else if(digitalRead(button[3]) == HIGH)
    state = SEOUL; 
 
}

void writeI2C(byte data){
  Wire.beginTransmission(seat);
  Wire.write(data);
  Wire.endTransmission();

  Wire.beginTransmission(aisle);
  Wire.write(data);
  Wire.endTransmission();
}

void setMode() {
  switch(state){
    case BUSAN:
      Serial.println("BUSAN");
      writeI2C(BUSAN);
      break;
    case BEFORE_ULSAN:
      Serial.println("BEFORE_ULSAN");
      writeI2C(BEFORE_ULSAN);
      if(l2 - l1 >= interval) state = ULSAN;
      break;
    case ULSAN:
      Serial.println("ULSAN");
      writeI2C(ULSAN);
      readSensor();
      break;
    case BEFORE_DAEJEON:
      Serial.println("BEFORE_DAEJEON");
      writeI2C(BEFORE_DAEJEON);
      readSensor();
      controlActuator();
      if(l4 - l3 >= interval) state = DAEJEON;
      break;
    case DAEJEON:
      Serial.println("DAEJEON");
      writeI2C(DAEJEON);
      break;
    case SEOUL:
      Serial.println("SEOUL");
      writeI2C(SEOUL);
      break;
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  l2 = millis(); l4 = millis();
  checkButton();
  setMode();
  delay(100);
}
