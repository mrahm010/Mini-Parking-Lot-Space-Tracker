#include <Servo.h>
#include <SPI.h>

//SPI Init---------------
char buff [255];
volatile byte indx;
volatile boolean process;
//-----------------------

Servo myservo;

int pos = 0;
int key = 0;
int key2 = 1;
int val = 0;

void setup() {
  Serial.begin(115200);
  //SPI Init-----------
  pinMode(MISO, OUTPUT);
  SPCR |= _BV(SPE); 
  indx = 0; 
  process = false;
  SPI.attachInterrupt();
  //-------------------
  
  pinMode(3, OUTPUT);
  pinMode(7, INPUT);
  myservo.attach(3);
  myservo.write(180);
  for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
}

ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
   
   if (indx < sizeof(buff)) {
      buff[indx++] = c; // save data in the next index in the array buff
      if (c == '\n') { 
        buff[indx - 1] = 0; // replace newline ('\n') with end of string (0)
        process = true;
      }
   }   
}

void loop() {
  val = digitalRead(7);
  if (key == 0) {
     if (val) {
       myservo.write(10);
       delay(15);
       key = 1;
       key2 = 0;
      if (process) {
        process = false; //reset the process
        Serial.println (buff); //print the array on serial monitor
        indx= 0; //reset button to zero
      }
     }
  }
  if (key2 == 0) {
    if (!val) { //parking lot has space
      myservo.write(90);
      delay(15);
      key = 0;
      key2 = 1;
    }
  }
}
