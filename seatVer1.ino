#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //시리얼 통신

const int pinFSR402 = A0; //센서, 액추에이터 선언
const int pinLED = 8;
const int pinVibM = 3;

int n; //SPI 통신 변수
volatile boolean done = false;

int pressure; //압력을 입력받는 변수
boolean seatOccupied = false; //현재 자리에 누가 있는지 확인하는 변수
int state; //현재 역이 어딘지 확인하는 변수

unsigned long l1, l2; //시간 체크
unsigned long interval = 10000; //진동이 켜지는 시간

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLED, OUTPUT);
  mySerial.begin(9600);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  SPCR &= ~_BV(MSTR);
}

void loop() {
  // put your main code here, to run repeatedly:
  pressure = analogRead(pinFSR402);
  l2 = millis();

  if(done){ //SPI통신 되었을 때 state를 변경함
    done = false;
    state = n;
    n = 0;
  }
  
  if(state == 3 || state == 4) { //울산역~대전역 구간에는 빈자리임
    if(pressure > 500){ //압력이 500보다 클 때 누군가 앉았다고 판단, 아두이노 2에 해당 정보 전달
      seatOccupied = true;
      mySerial.write('Y');
    } else {
      seatOccupied = false;
      mySerial.write('N');
    }
  }
  if (state == 4) { //대전역 도착하기 전에 자리에서 비켜야함을 알려줌
    seatOut();
  }
}

void seatOut(){ //자리에서 비켜야함을 알려주는 함수
  if(seatOccupied){ //아직 자리에 있다면 LED를 켜고 일정시간 뒤에 진동을 울림
    digitalWrite(pinLED, HIGH);
    if(l2 - l1 >= interval) analogWrite(pinVibM, 255);
  } else { //자리에서 비켰다면 진동과 led를 끔
    l1 = millis();
    digitalWrite(pinLED, LOW);
    analogWrite(pinVibM, 0);
  }
}

ISR(SPI_STC_vect) { //SPI 통신 인터럽트 함수
  n = SPDR;
  if(n == 4) l1 = millis();
  done = true;
}
