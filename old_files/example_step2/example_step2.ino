#define DIR 3
#define PUL 2
#define ENA 4
uint32_t count=0;
byte toggle0=0;

unsigned long now;
byte ch = 0; 
bool GO_=0;

void init_timer0()
{
  cli();//stop interrupts

//set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 24;// = (16*10^6) / (2000*64) - 1 (must be <256) //124 for 2khz; 24 for 10kHz
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  
  sei();//allow interrupts
  
}
//---------------------------------------
void init_timer2()
{
  cli();//stop interrupts

//set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 49;// = (16*10^6) / (8000*8) - 1 (must be <256) // 8kHz 252; 20kHz :99
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= 0b00000110;   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  sei();//allow interrupts
  
}
//---------------------------------------
ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
if (count<32000)
  {
  if (toggle0){
    digitalWrite(PUL,HIGH);
    toggle0 = 0;
  }
  else{
    digitalWrite(PUL,LOW);
    toggle0 = 1;
  }
 count++;
 }
}
//------------------------------------------------------------------------
ISR(TIMER2_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
//generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
if ((count<32000)&&(GO_))
  {
  if (toggle0){
    digitalWrite(PUL,HIGH);
    toggle0 = 0;
  }
  else{
    digitalWrite(PUL,LOW);
    toggle0 = 1;
  }
 count++;
 }
 if (count>=32000)
 {
   GO_=0;
   count=0;
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
  
  init_timer2();
  now=millis();

}
//---------------------------------------------------------------
//
void loop() {
   
  int sensorValue = analogRead(A1);
  float voltage= sensorValue * (5000 / 1023.0);
  // put your main code here, to run repeatedly:
  float force = 10 * voltage / 1000;
 
  Serial.println(force, 2);
  delay(100);
  
  if (Serial.available() > 0) {
    ch=Serial.read();
    if (ch=='G') {
       GO_=1;
    }
    if (ch=='R'){
       digitalWrite(DIR, LOW); 
    }
    if (ch=='L'){
       digitalWrite(DIR, HIGH); 
    }   
    if (ch=='S'){
       GO_=0;
    }   

    if (ch=='F')
    { 
      OCR2A = 8;
    }
    if (ch=='D')
    { 
      OCR2A = 252;
    }    
  }
}
