#pragma once

#define DATA_BITS 8

namespace spc
{
  typedef uint8_t Pin;
  
  struct PinConfig
  {
    Pin addrData;
    Pin addrClock;
    Pin addrLatch;
    Pin nChipEnable;
    Pin nOutputEnable;
    //Pin modeSelect;
    Pin data[DATA_BITS];
  };

  class EEPROMProgrammer
  {
  public:
    PinConfig pins;
  
    Programmer() { };
    
    void setPinConfig(PinConfig &pinConfig)
    {
      pins = pinConfig;
    };

    bool initialize()
    {
      pinMode(pins.addrData, OUTPUT);
      pinMode(pins.addrClock, OUTPUT);
      pinMode(pins.addrLatch, OUTPUT);
      pinMode(pins.nChipEnable, OUTPUT);
      pinMode(pins.nOutputEnable, OUTPUT);
      //pinMode(pins.modeSelect, OUTPUT);

      for (auto pin : pins.data)
        pinMode(pin, INPUT);

      return true;
    }

    byte read(int16_t addr)
    {
      byte data = 0;
      for (int pinNum = 0; pinNum < DATA_BITS; pinNum++)
        data |= (digitalRead(pins.data[pinNum]) << pinNum);
      return data;
    }
  };





  /*
  // initialize address pins
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(SHIFT_CLOCK, OUTPUT);

  // initialize data pins
  for (auto pin : dataPins)
    pinMode(pin, INPUT);

  writeAddr(0x0000, true);  // clear the buffer

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
  */
};

#if 0

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

#endif
