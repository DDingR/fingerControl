
/*
  force reporter
*/

// pin for force sensor
#define SENSER_pin A1

// constants
float forceMultiplier = 10;

void setup() 
{
  // put yor setup code here, to run once:

  // sensor pin
  pinMode(SENSER_pin, INPUT);

  // Serial setting
  Serial.begin(9600);

  // others
  // now = millis();

  // System report
  Serial.println("****** INFO *******");
  Serial.print("WHAT DO YOU WANT TO REPORT"); Serial.print("\n");
  Serial.println("*******************");

  // started
  Serial.println("\n\nStarted");
  delay(1000);
}

void loop() 
{
  // force observe
  int sensorValue = analogRead(SENSER_pin);

  // report
  Serial.println(sensorValue)
}
