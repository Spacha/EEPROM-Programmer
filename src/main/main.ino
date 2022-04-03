#include "Programmer.h"

/**
 * Pins:
 * D2,D3,D4:  Address shift register
 * D5..D12:   Data input/output
 * A0,A1,A2:  Chip control
 */
#define MAX_ADDR    65535  // 2^16 - 1
#define DATA_BITS   8

#define SHIFT_DATA  2  // DS
#define SHIFT_CLOCK 3  // SHCP
#define SHIFT_LATCH 4  // STCP
#define DATA_PINS   5, 6, 7, 8, 9, 10, 11, 12

const uint8_t dataPins[DATA_BITS] = {5, 6, 7, 8, 9, 10, 11, 12};
uint8_t ledOn = false;

spc::EEPROMProgrammer programmer;

/**
 * Address SHR pins: D[2..4].
 * Data pins: D[5..12] = Q[0..7].
 * Control pins: CTRL_nCE, CTRL_ERASE, CTRL_PROGRAM.
 *  Add yet another shift register for control
 * 
 * Total of 3 + 8 + 3 pins needed.
 */

void pulse(int pin)
{
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
  delayMicroseconds(100);
  digitalWrite(pin, LOW);
}

void writeAddr(uint16_t addr, bool set = false)
{
  shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, (addr >> 8) & 0xff);  // high byte
  shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, addr & 0xff);         // low byte

  if (set)
    pulse(SHIFT_LATCH);
}

uint8_t readData()
{
  // ASSUMPTION: Data pins are in INPUT mode

  uint8_t data = 0;
  for (int pinNum = 0; pinNum < DATA_BITS; pinNum++)
  {
    data |= (digitalRead(dataPins[pinNum]) << pinNum);
  }

  return data;
}

void setup()
{
  // set up the programmer
  spc::PinConfig pinConfig = {
    .addrDataPin = SHIFT_DATA,
    .addrClockPin = SHIFT_CLOCK,
    .addrLatchPin = SHIFT_LATCH,
    .nChipEnablePin = A0,
    .nOutputEnablePin = A1,
    //.modePin = A2,
    .dataPins = {5,6,7,8,9,10,11,12}
  };
  programmer.setPinConfig(pinConfig);

  // initialize address pins
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(SHIFT_CLOCK, OUTPUT);

  // initialize data pins
  for (auto pin : dataPins)
    pinMode(pin, INPUT);

  writeAddr(0x0000, true);  // clear the buffer

  Serial.begin(115200);
}

void loop()
{
  /* Count from 0 to 65535. */
  for (uint16_t addr = 0; addr <= MAX_ADDR; addr++)
  {
    if ((addr % 4) == 0)
    {
      //digitalWrite(A0, ledOn);
      ledOn = !ledOn;
    }
      
    writeAddr(addr, true);
    //delayMicroseconds(1);  // T_ACC <= 45ns
    uint8_t data = readData();

    char sbuf[32];
    sprintf(sbuf, "0x%04x: %u", addr, data);
    //Serial.println(sbuf);
    Serial.println(sbuf);
    
    delay(500);
  }
}
