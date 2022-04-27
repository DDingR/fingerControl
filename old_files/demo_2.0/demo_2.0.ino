
/*
  for version 2.0
  change how to control # serail command to buttons
*/

#define DIR 3
#define PUL 2
#define ENA 4

// need to set the pin
#define END_pin 10
#define GO_pin 9
#define DIR_pin 8
#define STOP_pin 7

uint32_t count = 0;
uint32_t pulsePerRev = 400;
uint32_t test_length = 22; // [mm]
uint32_t pitch_length = 5;
// test_length = pithch_length * (count_limit/2) / (pulse/revolution)
uint32_t count_limit = float(test_length) / float(pitch_length) * pulsePerRev * 2;
byte toggle0 = 0;

uint32_t cycle_oneway = 0;
uint32_t cycle = 0;
uint32_t test_cycle = 100; // [ ]
bool END_flag = 0;
bool cycle_GO_ = 0;
bool DIR_ = 0;
bool pre_DIR_INPUT = 0;
uint32_t DIR_change_delay_time = 50;

float force = 0.0;

unsigned long now;
void init_timer2()
{
  cli();//stop interrupts

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 128; // edited
  // = (16*10^6) / (8000*8) - 1 (must be <256) // 8kHz 252; 20kHz :99
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= 0b00000110;
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  sei();//allow interrupts

}

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

void setup() {
  // put yor setup code here, to run once:
  pinMode(DIR, OUTPUT);
  pinMode(PUL, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(END_flag, INPUT);
  pinMode(GO_pin, INPUT);
  pinMode(DIR_pin, INPUT);
  pinMode(STOP_pin, INPUT);
  
  digitalWrite(ENA, LOW);
  digitalWrite(DIR, LOW);

  Serial.begin(9600);

  init_timer2();
  now = millis();

  Serial.println("****** INFO *******");
  Serial.print("test_length [mm]: "); Serial.println(test_length);
  Serial.print("test_cycle: "); Serial.println(test_cycle);
  Serial.print("pulsePerRev: "); Serial.println(pulsePerRev);
  Serial.print("count_limit: "); Serial.println(count_limit);
  Serial.println("*******************");

  Serial.println("Press GO button to Start");
  while (!(digitalRead(GO_pin))) {}
  Serial.println("Wait...");
  delay(1000);
}

void loop() {

  int sensorValue = analogRead(A1);
  float voltage = sensorValue * (5000 / 1023.0);
  force = 10 * voltage / 1000;

  display();
  delay(100);

  if (cycle >= test_cycle) {
    display();
    Serial.println("finished! press GO button");
    while (!(digitalRead(GO_pin))) {}
    cycle = 0;
    count = 0;
  }

  if (digitalRead(END_pin)) {
    END_flag = 1;
    cycle_GO_ = 0;
  }
  else if (digitalRead(STOP_pin)) {
    cycle_GO_ = 0;
    count = 0;
    cycle = 0;
  }
  else if (digitalRead(GO_pin)) {
    cycle_GO_ = 1;
  }

  if (digitalRead(DIR_pin)) {
    if (!pre_DIR_INPUT) {
      DIR_ = !DIR_;
      digitalWrite(DIR, DIR_);
    }
    pre_DIR_INPUT = 1;
  }
  else {
    pre_DIR_INPUT = 0;
  }
  
}

void display() {
  //  Serial.print("cycle_GO_: ");
  Serial.print(cycle_GO_); Serial.print("\t");
  //  Serial.print("DIR: ");
  Serial.print(DIR_); Serial.print("\t");
  //  Serial.print("CNT: ");
  Serial.print(float(count + 1) / count_limit * 100, 2); Serial.print("\t");
  //  Serial.print("cycle: ");
  Serial.print(cycle);
  //  Serial.print("/"); Serial.print(test_cycle);
  Serial.print("\t");
  //  Serial.print("displacement[mm]: ");
  Serial.print(float(pitch_length) / 2 / pulsePerRev * float(count)); Serial.print("\t");
  //  Serial.print("FORCE: ");
  Serial.print(force, 2);
  Serial.print("\t");
    Serial.print(END_flag);
  Serial.println("\t");


}
