
#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
#define DEBUG_SERIAL soft_serial
#define DXL_SERIAL   Serial

const uint8_t DXL_DIR_PIN = 2; // DYNAMIXEL Shield DIR PIN
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const float DXL_PROTOCOL_VERSION = 2.0;
const uint8_t DXL_ID = 1;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup() {

  DEBUG_SERIAL.begin(115200);
  dxl.begin(57600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);

  // Turn off torque when configuring items in EEPROM area
  bool result = dxl.setID(DXL_ID, 2);
  DEBUG_SERIAL.println(result);
}

void loop() {
}
