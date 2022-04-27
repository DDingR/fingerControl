
/*
  motor controller
*/

// pin for control buttons
#define STOP 9
#define FRONT 8
#define BACK 7

// constants

void setup() 
{
  // put yor setup code here, to run once:
  
  // control buttons
  pinMode(STOP, INPUT);
  pinMode(FRONT, INPUT);
  pinMode(BACK, INPUT);

  // Serial setting
  Serial.begin(9600);

  // others
  now = millis();

  // System report
  Serial.println("****** INFO *******");
  Serial.print("WHAT DO YOU WANT TO REPORT"); Serial.print("\n");
  Serial.println("*******************");

  // start
  Serial.println("\n\nStarted");
  delay(1000);
}

void loop() 
{

  // emergency stop
  if (digitalRead(STOP))
  {
    
  }

  // main control
  if (digitalRead(FRONT) && digitalRead(BACK))
  {
    // wrong input

  }
  else if (digitalRead(FRONT))
  {
    // front

  }
  else if (digitalRead(BACK))
  {
    // back

  }


  // report
  display();
  delay(100);
  
}

void display() 
{

}
