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

static const spc::Pin dataPins[DATA_BITS] = { 5, 6, 7, 8, 9, 10, 11, 12 };

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

void setup()
{
  Serial.begin(115200);

  // set up the programmer
  spc::PinConfig programmerPins;

  programmerPins.addrData = SHIFT_DATA;
  programmerPins.addrClock = SHIFT_CLOCK;
  programmerPins.addrLatch = SHIFT_LATCH;
  programmerPins.nChipEnable = A0;
  programmerPins.nOutputEnable = A1;
  //programmerPins.modeSelect = A2;
  memcpy(programmerPins.data, dataPins, DATA_BITS);
  
  programmer.setPinConfig(programmerPins);
  if (!programmer.initialize())
  {
    Serial.println("Failed to initialize spc::EEPROMProgrammer.");
    return 0;
  }
}

void loop()
{
  // programmer.erase();
  // programmer.write(int16_t addr, int8_t data);
  int16_t addr = 0x1234;
  byte data = programmer.read(addr);

  char sbuf[32];
  sprintf(sbuf, "0x%04x: %u", addr, data);
  //Serial.println(sbuf);
  Serial.println(sbuf);

  delay(500);
}
