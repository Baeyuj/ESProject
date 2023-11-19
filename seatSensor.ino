#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //UART 통신 핀 선언

int pinFsr = A0;//압력센서
int fsrValue;//압력입력값

volatile boolean data = 0;
volatile byte command = 0;

void setup()
{
  mySerial.begin(9600);//시리얼통신 보드레이트
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPCR &= ~_BV(MSTR);
  SPCR |= _BV(SPIE);
  
  pinMode(pinFsr, INPUT);
}

void loop()
{
  fsrValue = analogRead(pinFsr);
  if (fsrValue > 500){
     mySerial.write('Y'); //시리얼 통신으로 송신 보냄
     data = 1;
  }else{
    mySerial.write('N'); //시리얼 통신으로 송신 보냄
    data = 0;
  }
  
if(digitalWrite(S[0], HIGH)//마스터로부터 명령해제상태 받음
     command = 0;    
}//loop 끝

ISR(SPI_STC_vect)//아두이노 1과 SPI 통신
{
  byte c = SPDR;
  switch(command){
  case 99:
    command = c;
    SPDR = 0;
    break;
  case 's':
    SPDR = data;
    break;
  }
}
