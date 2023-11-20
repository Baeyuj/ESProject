#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5);       //UART

int pinFSR402 = A0;                  // 압력센서 핀
int FSRValue = 0;                    // 압력 값

volatile byte seatOccupied = 0;      // 현재 자리에 사람이 있는지 여부
volatile byte command = 0;           // 마스터로부터 받는 명령

void setup(){
  mySerial.begin(9600); 
  
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE);
  SPCR &= ~_BV(MSTR);
  SPCR |= _BV(SPIE);
}

void loop(){
  FSRValue = analogRead(pinFSR402); // 압력센서 읽음
  if (FSRValue > 500){              // 압력이 500보다 클 때
     mySerial.write('Y');           // aisleLCD에 송신
     seatOccupied = 1;              // 자리에 앉아있다고 판단
  } else {                          // 압력이 500보다 작을 때
    mySerial.write('N');            // aisleLCD에 송신
    seatOccupied = 0;               // 자리에 앉아있지 않다고 판단
  }
  
  if(digitalRead(SS) == HIGH)       // 명령 상태 해제
     command = 255;    
}

ISR(SPI_STC_vect){
  byte c = SPDR;
  
  switch(command){
  case 255:                         // 쓰레기 값을 받았을 때
    command = c;
    SPDR = 255;
    break;
  case 's':                         // 센서값을 요청받았을 때
    SPDR = seatOccupied;
    break;
  }
}
