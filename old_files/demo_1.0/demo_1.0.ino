/*
   G: start cycle
   C: change direction
   S: stop
*/


#define DIR 3
#define PUL 2
#define ENA 4
uint32_t count = 0;
uint32_t pulsePerRev = 400;
uint32_t test_length = 27;
uint32_t pitch_length = 5;
// test_length = pithch_length * (count_limit/2) / (pulse/revolution)
uint32_t count_limit = float(test_length) / float(pitch_length) * pulsePerRev * 2;
byte toggle0 = 0;

uint32_t cycle_oneway = 0;
uint32_t cycle = 0;
uint32_t test_cycle = 10000;
bool cycle_GO_ = 0;
bool DIR_ = 0;

unsigned long now;
byte ch = 0;
//bool GO_ = 0;

//void init_timer0()
//{
//  Serial.println("init timer0");
//
//  //  cli();//stop interrupts
//
//  //set timer0 interrupt at 2kHz
//  TCCR0A = 0;// set entire TCCR0A register to 0
//  TCCR0B = 0;// same for TCCR0B
//  TCNT0  = 0;//initialize counter value to 0
//  // set compare match register for 2khz increments
//  OCR0A = 49;// = (16*10^6) / (2000*64) - 1 (must be <256) //124 for 2khz; 24 for 10kHz
//  // turn on CTC mode
//  TCCR0A |= (1 << WGM01);
//  // Set CS01 and CS00 bits for 64 prescaler
//  TCCR0B |= 0b00000110; // (1 << CS01) | (1 << CS00);
//  //  TCCR0B |= (1 << CS01) | (1 << CS00);
//  // enable timer compare interrupt
//  TIMSK0 |= (1 << OCIE0A);
//
//  //  sei();//allow interrupts
//
//}
//---------------------------------------
void init_timer2()
{
//  Serial.println("init timer2");

  cli();//stop interrupts

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 16; // edited
  // = (16*10^6) / (8000*8) - 1 (must be <256) // 8kHz 252; 20kHz :99
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= 0b00000110;
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  sei();//allow interrupts

}
//---------------------------------------
//ISR(TIMER0_COMPA_vect) {
//  Serial.println("TIMER0 vect activated!");
//  if ((count < 32000) && (cycle_GO_))
//  {
//    if (toggle0) {
//      digitalWrite(PUL, HIGH);
//      toggle0 = 0;
//    }
//    else {
//      digitalWrite(PUL, LOW);
//      toggle0 = 1;
//    }
//    count++;
//  }
//  if (cycle > 10000)
//  {
//    cycle_GO_ = 0;
//    count = 0;
//    cycle = 0;
//  }
//  else if (count >= 32000/100) {
//    count = 0;
//    cycle++;
//    DIR_=!DIR_;
////    Serial.println("DIR changed!");
//    digitalWrite(DIR, DIR_);
//  }
//}
//------------------------------------------------------------------------
ISR(TIMER2_COMPA_vect) { //timer1 interrupt 8kHz toggles pin 9
  //generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
  //    Serial.println("TIMER2 vec activated!");
  if ((count < count_limit) && (cycle_GO_))
  {
    if (toggle0) {
      digitalWrite(PUL, HIGH);
      toggle0 = 0;
    }
    else {
      digitalWrite(PUL, LOW);
      toggle0 = 1;
    }
    count++;
  }

  if (cycle >= test_cycle)
  {
    cycle_GO_ = 0;
    //    count = 0;
    //    cycle = 0;
  }
  else if (count >= count_limit) {
    count = 0;
    cycle_oneway++;
    cycle = cycle_oneway / 2;
    DIR_ = !DIR_;
    //    Serial.println("DIR changed!");
    digitalWrite(DIR, DIR_);
    delay(100);
  }

}
//-------------------------------------------------------------------

void setup() {
  // put yor setup code here, to run once:
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
  pinMode(ENA, OUTPUT);

  digitalWrite(ENA, LOW);
  digitalWrite(DIR, LOW);

  Serial.begin(9600);

  //  cli();
  //  init_timer0();
  init_timer2();
  //  sei();
  now = millis();

  Serial.println("****** INFO *******");
  Serial.print("test_length [mm]: ");Serial.println(test_length);
  Serial.print("test_cycle: ");Serial.println(test_cycle);
  Serial.print("pulsePerRev: ");Serial.println(pulsePerRev);
  Serial.print("count_limit: ");Serial.println(count_limit);
  Serial.println("*******************");
  
  Serial.println("Insert any key to start");
  while (!Serial.available()) {}

}
//---------------------------------------------------------------
//
void loop() {

  int sensorValue = analogRead(A1);
  float voltage = sensorValue * (5000 / 1023.0);
  // put your main code here, to run repeatedly:
  float force = 10 * voltage / 1000;

  display(force);
  delay(100);

  if (cycle >= test_cycle) {
    display(force);
    Serial.println("finished! insert any key");
    while (!Serial.available()) {}
    cycle = 0;
    count = 0;
  }

  if (Serial.available() > 0) {
    ch = Serial.read();
    //    if (ch == 'G') {
    //      GO_ = 1;
    //    }
    if (ch == 'G') {
      cycle_GO_ = 1;
    }
    if (ch == 'C') {
      DIR_ = !DIR_;
      digitalWrite(DIR, DIR_);
    }
    if (ch == 'S') {
      //      GO_ = 0;
      cycle_GO_ = 0;
      count = 0;
      cycle = 0;
    }

    //    if (ch == 'F')
    //    {
    //      OCR2A = 8;
    //    }
    //    if (ch == 'D')
    //    {
    //      OCR2A = 252;
    //    }
  }
}

void display(float force) {
  Serial.print("cycle_GO_: "); Serial.print(cycle_GO_); Serial.print("\t");
  Serial.print("DIR: "); Serial.print(DIR_); Serial.print("\t");
  Serial.print("CNT[%]: "); Serial.print(float(count + 1) / count_limit * 100, 2); Serial.print("\t");
  Serial.print("cycle: ");
  Serial.print(cycle); Serial.print("/"); Serial.print(test_cycle);
  Serial.print("\t");
  Serial.print("displacement[mm]: "); Serial.print(float(pitch_length) / 2 / pulsePerRev * float(count)); Serial.print("\t");
  Serial.print("FORCE: "); Serial.println(force, 2);

}
