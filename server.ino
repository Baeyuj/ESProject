#include <state.h>
#include <Wire.h>
#include <SPI.h>

#define arduino1 7 // 아이씨
#define arduino2 8 // 아이씨

byte seatOccupied; // 압력 
int button[4] = {2, 3, 4, 5};
// 버튼 디바운싱을 위한 현재, 마지막 버튼 상황
int last_button[4] = {0};
int current_button[4] = {0};

int S[2] = {10, 9}; // spi 아두이노 4, 5

unsigned long l1 = 0,l2 =0,l4=0, l3 = 0; // millis()
unsigned long interval = 1000; // 3분 1000*60*3 180000

//--------------------------------------------------------//
void setup() {
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

//----------------------------------------------------------//SIP 통신
byte transferAndWait(const byte what) { // sip 신호 
  byte a = SPI.transfer(what);
  delayMicroseconds(20);
  return a;
}

void controlActuator() { // 아두이노 5에게 압력 신호 보내기
  digitalWrite(S[1], LOW); // S[1]-> 아두이노 5
  Serial.println("죽을거같다.");
  SPI.transfer(seatOccupied); // 좌석에 사람이 있는지 여부를 액추에이터에 전송함 
  digitalWrite(S[1], HIGH);
}
//-----------------------------------------------------------//
boolean readSensor() { // 아두이노 4에게 압력값 받기
  digitalWrite(S[0], LOW);// S[0]-> 아두이노 4
  Serial.println("나한테 왜이래.");
  transferAndWait('s'); // 센서에 사람이 있는지 데이터 전송 요청
  transferAndWait(99);
  seatOccupied = transferAndWait(99);
  
  digitalWrite(S[0], HIGH);
}


//--------------------------------------------------------------//I2C 통신
void begin_I2C(int state) {
  sendDataToSlave(arduino1, state); // arduino1에게 값 전송
  sendDataToSlave(arduino2, state); // arduino2에게 값 전송
}

void sendDataToSlave(int slaveAddress, int data) {
  Wire.beginTransmission(slaveAddress); // 특정 슬레이브에 대한 I2C 통신 시작
  Wire.write((uint8_t*)&data, sizeof(data)); // 데이터를 바이트로 변환하여 전송
  Wire.endTransmission(); // I2C 통신 종료
}
//-----------------------------------------------------------//
void setMode(int mode) // 신호 전송
{
  readSensor();
  controlActuator();
  
  if (mode == 0) {  
    Serial.println(0); 
    begin_I2C(BUSAN);
  }
  if (mode == 1) {
    l2 = millis();
    SPI.transfer(BEFORE_ULSAN);
    begin_I2C(BEFORE_ULSAN);
    Serial.println(1);
    if (l2 - l1 >= interval) {
      Serial.println(2);
      begin_I2C(ULSAN);
      l1 = l2;
    }
  }
  if (mode == 2) {
    Serial.println(3);
    l4 = millis();
    SPI.transfer(BEFORE_DAEJEON);
    begin_I2C(BEFORE_DAEJEON);
    if (l4 - l3 >= interval) {
      Serial.println(4);
      begin_I2C(DAEJEON);
      l3 = l4;
    }
  }
  if (mode == 3) {
    Serial.println(5);
    begin_I2C(SEOUL);
  }
}
 
//-----------------------------------------------------------// 버튼아 왜 안되는거니 이 개자식아 조 좀 되라고
boolean debounce(int pin) { // 버튼 디바운싱 함수
  boolean current = digitalRead(button[pin]);

  if (current != last_button[pin]) {
    delay(5);
    current = digitalRead(button[pin]);
  }
  return current;
}

void Button() // 버튼 입력
{
  for (int i = 0; i < 4; i++) {
    current_button[i] = debounce(i);
    if (current_button[i] == HIGH && last_button[i] == LOW) {
      for (int j = 0; j < 2; j++) {
        digitalWrite(S[j], LOW);
        setMode(i);
        digitalWrite(S[j], HIGH);
      }
    }
    last_button[i] = current_button[i];
  }
}
void loop() {
  Button();
}
