
#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial
#define DXL_SERIAL   Serial

#define button1Pin 5
#define button2Pin 6
#define button3Pin 12
#define button4Pin 13

const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const float DXL_PROTOCOL_VERSION = 2.0;
const uint8_t motor1_ID = 1;
const uint8_t motor2_ID = 100;

uint32_t button1 = 0;
uint32_t button2 = 0;
uint32_t button3 = 0;
uint32_t button4 = 0;


//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup() 
{
  DEBUG_SERIAL.begin(115200);
  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  
  // Get DYNAMIXEL information
  dxl.ping(motor1_ID);
  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(motor1_ID);
  dxl.setOperatingMode(motor1_ID, OP_PWM); // OP_POSITION, OP_VELOCITY, OP_PWM
  dxl.torqueOn(motor1_ID);

  dxl.ping(motor2_ID);
  dxl.torqueOff(motor2_ID);
  dxl.setOperatingMode(motor2_ID, OP_PWM); 
  dxl.torqueOn(motor2_ID);


  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(button4Pin, INPUT);

  DEBUG_SERIAL.println("*************** DIRECTIONS ***************");
  DEBUG_SERIAL.println("BUTTON1 | Motor1 Count Clockwise Rotation");
  DEBUG_SERIAL.println("BUTTON2 | Motor1 Clockwise Rotation");
  DEBUG_SERIAL.println("BUTTON3 | Motor2 Count Clockwise Rotation");
  DEBUG_SERIAL.println("BUTTON4 | Motor2 Clockwise Rotation");
  DEBUG_SERIAL.println("******************************************");
  DEBUG_SERIAL.println("button1|button2|button3|button4");
 }

void loop() 
{
  button1 = digitalRead(button1Pin);
  button2 = digitalRead(button2Pin);
  button3 = digitalRead(button3Pin);
  button4 = digitalRead(button4Pin);

  display();
  //    delay(100);

  if (button1) 
  {
    dxl.setGoalPWM(motor1_ID, 200);
  }
  else if (button2) 
  {
    dxl.setGoalPWM(motor1_ID, -200);
  }
  else 
  {
    dxl.setGoalPWM(motor1_ID, 0);
  }

  if (button3) 
  {
    dxl.setGoalPWM(motor2_ID, 200);
  }
  else if (button4) 
  {
    dxl.setGoalPWM(motor2_ID, -200);
  }
  else 
  {
    dxl.setGoalPWM(motor2_ID, 0);
  }
}

void display() {
  DEBUG_SERIAL.print(button1); 
  DEBUG_SERIAL.print("\t");
  DEBUG_SERIAL.print(button2); 
  DEBUG_SERIAL.print("\t");
  DEBUG_SERIAL.print(button3); 
  DEBUG_SERIAL.print("\t");
  DEBUG_SERIAL.print(button4); 
  DEBUG_SERIAL.print("\r");
}


/*
   for velocity control
       dxl.setGoalVelocity(DXL_ID, vel); vel = "value", UNIT_RPM, UNIT_PERCENT(-100~100)
       DEBUG_SERIAL.println(dxl.getPresentVelocity(DXL_ID));
   for position control
       dxl.setGoalPosition(DXL_ID, pos); pos = "value", UNIT_DEGREE
*/
