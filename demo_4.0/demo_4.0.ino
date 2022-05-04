// button -> potentiometer

#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial
#define DXL_SERIAL   Serial

#define SENSOR1_PIN A1
#define SENSOR2_PIN A0

const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const float DXL_PROTOCOL_VERSION = 2.0;
const uint8_t motor1_ID = 1;
const uint8_t motor2_ID = 100;

int sensor1;
int sensor2;

char str[30];

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


  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);

  DEBUG_SERIAL.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  DEBUG_SERIAL.println("*************** DIRECTIONS ***************");
  DEBUG_SERIAL.println("SENSOR1 | Motor1 Control");
  DEBUG_SERIAL.println("SENSOR2 | Motor2 Control");
  DEBUG_SERIAL.println("******************************************");

  DEBUG_SERIAL.println("To Start, Place Sensors to the Value Zero (Tolerance 5)");
  DEBUG_SERIAL.println("sensor1|sensor2|");

  do
  {
    sensor1 = analogRead(SENSOR1_PIN);
    sensor2 = analogRead(SENSOR2_PIN);

    //  [1023:10] -> [-200:200]
    sensor1 = map(sensor1, 1023, 10, -200, 200);
    sensor2 = map(sensor2, 1023, 10, -200, 200);

    sprintf(str, "%4d    %4d\r", sensor1, sensor2);
    DEBUG_SERIAL.print(str);


  } while (abs(sensor1) > 5 || abs(sensor2) > 5);

  DEBUG_SERIAL.print("...ok wait       \r");
  delay(2000);
}

void loop()
{
  sensor1 = analogRead(SENSOR1_PIN);
  sensor2 = analogRead(SENSOR2_PIN);

  //  [1023:10] -> [-200:200]
  sensor1 = map(sensor1, 1023, 10, -200, 200);
  sensor2 = map(sensor2, 1023, 10, -200, 200);

  sprintf(str, "%4d    %4d\r", sensor1, sensor2);
  DEBUG_SERIAL.print(str);

  dxl.setGoalPWM(motor1_ID, sensor1);
  dxl.setGoalPWM(motor2_ID, sensor2);
}


/*
   for velocity control
       dxl.setGoalVelocity(DXL_ID, vel); vel = "value", UNIT_RPM, UNIT_PERCENT(-100~100)
       DEBUG_SERIAL.println(dxl.getPresentVelocity(DXL_ID));
   for position control
       dxl.setGoalPosition(DXL_ID, pos); pos = "value", UNIT_DEGREE
*/
