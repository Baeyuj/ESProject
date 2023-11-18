volatile boolean seatOccupied = 0; //좌석 착석 여부

const int pinLED = 2;
const int pinVibM = 3;

unsigned long l1, l2; //시간 체크
unsigned long interval = 10000; //진동이 켜지는 시간

void setup() {
  pinMode(pinLED, OUTPUT);
  SPCR |= _BV(SPE);
  SPCR |= _BV(SPIE);
  SPCR &= ~_BV(MSTR);
}

void loop() {
  // put your main code here, to run repeatedly:
  l2 = millis();
  if (seatOccupied == true){ //좌석에 누가 착석했을 때
    digitalWrite(pinLED, HIGH);
    if(l2 - l1 >= interval) analogWrite(pinLED, 255);
  } else { //좌석에 누가 착석하지 않았을 때
    digitalWrite(pinLED, LOW);
    analogWrite(pinVibM, 0);
  }
}

ISR(SPI_STC_vect) {
  byte b = SPDR;
  seatOccupied = b;
  l1 = millis();
}
