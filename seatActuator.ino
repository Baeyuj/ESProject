volatile byte seatOccupied = 0; //좌석 착석 여부

const int pinLED[3] = { 7, 8, 9 };
const int pinVibM = 3;
int cnt = 0;

unsigned long l1, l2; //시간 체크
unsigned long interval = 5000; //진동이 켜지는 시간

void setup() {
  for(int i=0; i<3; i++){
    pinMode(pinLED[i], OUTPUT); 
  }
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  SPCR &= ~_BV(MSTR);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (cnt == 1) l1 = millis();                          //SPI 통신으로 처음으로 누가 앉아있다는 받았을 때 시간을 잼
  if (seatOccupied == 0) cnt = 0;                       //아무도 앉아있지 않을 때 cnt 초기화
  l2 = millis();
  if (seatOccupied == 1){                               //좌석에 누가 착석했을 때
    Serial.println("Seat");
    for(int i=0; i<3; i++)                              //LED
      digitalWrite(pinLED[i], HIGH); 
    if(l2 - l1 >= interval) analogWrite(pinVibM, 255);  //일정 시간 후 진동 켬
  } else {                                              //좌석에 누가 착석하지 않았을 때
    Serial.println("no Seat");
    for(int i=0; i<3; i++)                              //LED
      digitalWrite(pinLED[i], LOW); 
    analogWrite(pinVibM, 0);                            //진동
  }
}

ISR(SPI_STC_vect) {
  byte b = SPDR;
  seatOccupied = b;
  Serial.println(b);
  cnt++;
}
