// 10k 저항
// 센서에는 극성없음
// 그라운드와 아날로그 핀이랑 연결


#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial
#define DXL_SERIAL   Serial

#define L_pin 13
#define R_pin 12
//#define FSR0_pin A0
//#define FSR1_pin A1
//#define FSR2_pin A2

const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const float DXL_PROTOCOL_VERSION = 2.0;
const uint8_t DXL_ID = 1;

uint32_t L_button = 0;
uint32_t R_button = 0;


//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup() {

  DEBUG_SERIAL.begin(115200);
  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_PWM); // OP_POSITION, OP_VELOCITY, OP_PWM
  dxl.torqueOn(DXL_ID);

  pinMode(L_pin, INPUT);
  pinMode(R_pin, INPUT);

  DEBUG_SERIAL.println("  *** started *** ");
  DEBUG_SERIAL.println("  command LEFT button for GRASP ");
  DEBUG_SERIAL.println("  command RIGHT button for RELEASE ");
  DEBUG_SERIAL.println("  any commands for PAUSE ");
}

void loop() {
  L_button = digitalRead(L_pin);
  R_button = digitalRead(R_pin);

  display();
//    delay(100);

  if (L_button && R_button) {
    DEBUG_SERIAL.print("State: ERROR");
  }
  else if (L_button) {
    dxl.setGoalPWM(DXL_ID, 200);
    DEBUG_SERIAL.print("State: GRASP        ");
  }
  else if (R_button) {
    dxl.setGoalPWM(DXL_ID, -200);
    DEBUG_SERIAL.print("State: RELEASE       ");
  }
  else { 
    dxl.setGoalPWM(DXL_ID, 0);
    DEBUG_SERIAL.print("State: pause        ");
  }
  DEBUG_SERIAL.println("");
}

void display() {
  DEBUG_SERIAL.print("L: "); DEBUG_SERIAL.print(L_button); DEBUG_SERIAL.print("\t");
  DEBUG_SERIAL.print("R: "); DEBUG_SERIAL.print(R_button); DEBUG_SERIAL.print("\t");
}


/*
   for velocity control
       dxl.setGoalVelocity(DXL_ID, vel); vel = "value", UNIT_RPM, UNIT_PERCENT(-100~100)
       DEBUG_SERIAL.println(dxl.getPresentVelocity(DXL_ID));
   for position control
       dxl.setGoalPosition(DXL_ID, pos); pos = "value", UNIT_DEGREE
*/
